#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "ZeoFlow_SFML.h"
#include "Collision.hpp"

//Compiler Directives
using namespace std;
using namespace zeoFlow;
using namespace sf;
using namespace Collision;

RenderWindow window(VideoMode(800, 500), "Dangerous Racing");
ZeoFlow_SFML zfSFML;

int raceLvl = 1;
const float pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;
const int num=12; //checkpoints
int points[num][2] = {
	261, 825,
	396, 436,
	664, 235,
	899, 197,
	1153, 235,
	1449, 436,
	1580, 825,
	1580, 2324,
	1295, 2849,
	899, 2956,
	536, 2849,
	261, 2324
};

int userCar;
int lapsLvl1 = 9;
int checkpointsLvl1[7][4] = {
	161, 780, 522, 780,
	884, 121, 884, 433,
	1331, 780, 1672, 780,
	1331, 2350, 1672, 2350,
	884, 2731, 884, 3064,
	161, 2417, 522, 2417,
	179, 1748, 498, 1748
};

struct TrackObjects
{
	float x, y, angle;
	Sprite sprite;
	
	TrackObjects() {

	}
};

struct Car
{
	float x,y,speed,angle;
	int carId, currentCheckPoint, lap, n;
	Sprite sCar;
	
	Car() {
		speed=0;
		angle=0;
		n=0;
		lap=0;
		currentCheckPoint=6;
	}

	int place()
	{
		int carPlace = lap * 7 + currentCheckPoint;
		return carPlace;
	}
	
	void increaseSpeed()
	{
		srand(time(NULL));
		speed = speed - rand()%10/10 * 1.5 + rand()%10/10 * 3;
	}
	
	void decreaseSpeed()
	{
		srand(time(NULL));
		speed = speed - rand()%10/10 * 3;
	}
	
	void checkCheckpoint()
	{
		int x1 = checkpointsLvl1[currentCheckPoint][0];
		int x2 = checkpointsLvl1[currentCheckPoint][2];
		int y1 = checkpointsLvl1[currentCheckPoint][1];
		int y2 = checkpointsLvl1[currentCheckPoint][3];
		int carX = (int) x;
		int carY = (int) y;
		if(checkpointsLvl1[currentCheckPoint][1] == checkpointsLvl1[currentCheckPoint][3]) {
			if(carY >= y1 - 10 && carY <= y1 + 10 && carX >= x1 && carX <= x2) {
				currentCheckPoint++;
				increaseSpeed();
			}
		} else {
			if(carX >= x1 - 10 && carX <= x1 + 10 && carY >= y1 && carY <= y2) {
				currentCheckPoint++;
				increaseSpeed();
			}
		}
		
		if (currentCheckPoint == 7) {
			currentCheckPoint = 0;
			lap++;
			cout<<"Lap: "<<lap<<"/"<<lapsLvl1<<" for car no "<<carId<<" speed: "<<speed<<'\n';
		}
	}

	void move()
	{
		x += sin(angle) * speed;
		if(x<75) x=75;
		if(x>1728) x=1728;
		y -= cos(angle) * speed;
		if(y<80) y=80;
		if(y>3108) y=3108;
		if(userCar != carId) {
			findTarget();
		}
		checkCheckpoint();
	}

	void findTarget()
	{
		float tx=points[n][0];
		float ty=points[n][1];
		float beta = angle-atan2(tx-x,-ty+y);

		if (sin(beta)<0) angle+=0.005*speed;
		else angle-=0.005*speed;

		if ((x-tx)*(x-tx)+(y-ty)*(y-ty)<25*25) n=(n+1)%num;
	}
};

struct TracksBackground
{
	Sprite backgroundTrack;

	TracksBackground() {

	}
};

struct CarModels
{
	Sprite carSprite;

	CarModels() {

	}
};

TracksBackground tracksBackground[3];
TrackObjects trackObjects[2];
CarModels carModels[2];
const int carsPerLvl = 4;
Car car[carsPerLvl];

float speed=0,angle=0;
float maxSpeed=15;
float turnSpeed=0.05;
int offsetX=0,offsetY=0;
void showGameScreen() {
	bool Up=false,Right=false,Down=false,Left=false;

	if (Keyboard::isKeyPressed(Keyboard::Up)) Up=true;
	if (Keyboard::isKeyPressed(Keyboard::Right)) Right=true;
	if (Keyboard::isKeyPressed(Keyboard::Down)) Down=true;
	if (Keyboard::isKeyPressed(Keyboard::Left)) Left=true;

	float acc = 0.2, dec = 0.2;
	
	srand(time(NULL));
	
	if (Up && speed<maxSpeed + rand()%2 + 1)
		if (speed < 0)  speed += dec;
		else  speed += acc + rand()%3/10;
				
	if (Down && speed>-maxSpeed)
		if (speed > 0) speed -= dec * 2;
		else  speed -= acc;

	if (!Up && !Down)
		if (speed - dec > 0) speed -= dec;
		else if (speed + dec < 0) speed += dec;
		else speed = 0;

	if (Right && speed!=0)  angle += turnSpeed * speed/maxSpeed;
		if (Left && speed!=0)   angle -= turnSpeed * speed/maxSpeed;

	car[userCar].speed = speed;
	car[userCar].angle = angle;
			
	car[userCar].move();
	for(int i=0;i<carsPerLvl;i++) {
		if(i != userCar) {
			car[i].move();
		}
	}
			
	for(int i=0;i<carsPerLvl;i++){
		if(i != userCar) {
			car[i].findTarget();
		}
	}

	srand(time(NULL));
	for(int i=0;i<carsPerLvl;i++)
		for(int j=0;j<carsPerLvl;j++) {
			int dx=0, dy=0;
			if (PixelPerfectDetection(car[i].sCar, car[j].sCar) && i != j) {
				dx = car[i].x-car[j].x;
				dy = car[i].y-car[j].y;
				if(car[i].speed != 0) {
					car[i].x+=dx*car[i].speed/50.0;
					car[i].y+=dy*car[i].speed/50.0;
				}
			}
		}

	for(int i=0; i<2; i++) {
		int dx=0, dy=0;
		if (PixelPerfectDetection(car[userCar].sCar, trackObjects[i].sprite)) {
			dx = car[userCar].x-trackObjects[i].x;
			dy = car[userCar].y-trackObjects[i].y;
			if(car[userCar].speed != 0) {
				car[userCar].x+=dx*car[userCar].speed/50.0;
				car[userCar].y+=dy*car[userCar].speed/50.0;
			}
		}
	}

	for(int i=0; i<carsPerLvl; i++) {
		if(i != userCar && car[i].speed < maxSpeed + carsPerLvl - i) {
			car[i].speed += acc;
		}
	}
	
	if (car[userCar].x>320 && car[userCar].x<1330) offsetX = car[userCar].x-320;
	if (car[userCar].y>240 && car[userCar].y<2928) offsetY = car[userCar].y-240;

	tracksBackground[raceLvl - 1].backgroundTrack.setPosition(-offsetX,-offsetY);
	window.draw(tracksBackground[raceLvl - 1].backgroundTrack);

	for(int i=0;i<carsPerLvl;i++)
	{
		car[i].sCar.setPosition(car[i].x-offsetX,car[i].y-offsetY);
		car[i].sCar.setRotation(car[i].angle*180/pi);
		window.draw(car[i].sCar);
	}
	
	for(int i=0; i<2; i++) 
	{
		trackObjects[i].sprite.setPosition(trackObjects[i].x-offsetX,trackObjects[i].y-offsetY);
		trackObjects[i].sprite.setRotation(trackObjects[i].angle*180/pi);
		window.draw(trackObjects[i].sprite);
	}

}

void initialiseGameData()
{
	Sprite bg1 = zfSFML.loadSpriteFromTexture("Assets/", "track_1", "png");
	bg1.setScale(1, 1);
	tracksBackground[0].backgroundTrack = bg1;

	Sprite bg2 = zfSFML.loadSpriteFromTexture("Assets/", "track_2", "png");
	bg2.setScale(1, 1);
	tracksBackground[1].backgroundTrack = bg2;

	Sprite bg3 = zfSFML.loadSpriteFromTexture("Assets/", "track_3", "png");
	bg3.setScale(1, 1);
	tracksBackground[2].backgroundTrack = bg3;

	Sprite sCar1(zfSFML.loadSpriteFromTexture("Assets/", "car1", "png"));
	sCar1.setOrigin(61, 128);
	sCar1.scale(0.7, 0.7);
	carModels[0].carSprite = sCar1;
	Sprite sCar2(zfSFML.loadSpriteFromTexture("Assets/", "car2", "png"));
	sCar2.setOrigin(128, 128);
	sCar2.scale(0.7, 0.7);
	carModels[1].carSprite = sCar2;
	if(raceLvl == 1) {
		userCar = 3;
		for(int i=0;i<carsPerLvl;i++)
		{
			car[i].x= 260 + i%2*135;
			if(i<2) {
				car[i].y=1920;
			} else if(i<4) {
				car[i].y=1920 + 200;
			} else if(i<6) {
				car[i].y=1920 + 400;
			}
			car[i].sCar = carModels[0].carSprite;
			car[i].carId = i;
		}
		Sprite sAmbulance(zfSFML.loadSpriteFromTexture("Assets/", "ambulance", "png"));
		sAmbulance.setOrigin(128, 128);
		sAmbulance.scale(0.9, 0.9);
		for(int i=0; i<2; i++) {
			trackObjects[i].sprite = sAmbulance;
			trackObjects[i].x = 640;
			trackObjects[i].y = 1780 + i*120;
			trackObjects[i].angle = 45;
		}
	} else if(raceLvl == 2) {
		userCar = 3;
		for(int i=0;i<carsPerLvl;i++)
		{
			car[i].x= 260 + i%2*135;
			if(i<2) {
				car[i].y=1920;
			} else if(i<4) {
				car[i].y=1920 + 200;
			} else if(i<6) {
				car[i].y=1920 + 400;
			}
			car[i].sCar = carModels[0].carSprite;
			car[i].carId = i;
		}
	}
}

int main()
{
	window.setFramerateLimit(60);
	
	Sprite zeoFlowSprite(zfSFML.loadSpriteFromTexture("Assets/", "zeoflow_logo", "png"));

	int currentScreen = 0;
	Clock clock;
	int clockState = 0;
	
	RectangleShape splashScreenBg;
	splashScreenBg.setSize(Vector2f(window.getSize().x, window.getSize().y));
	Color color(16, 16, 16);
	splashScreenBg.setFillColor(color);
	
	initialiseGameData();

    while (window.isOpen())
    {
		Event event;
        while (window.pollEvent(event))
        {
			if (event.type == Event::Closed || event.key.code == Keyboard::Escape)
			{
                window.close();
			}
		}

        window.clear();

		if (clockState == 0)
		{
			clock.restart();
			clockState++;
		}

		if (currentScreen == 0)
		{
			float sec = clock.getElapsedTime().asSeconds();
			if (sec < 2.0)
			{
				window.draw(splashScreenBg);
				zeoFlowSprite.setScale(0.15, 0.15);
				zeoFlowSprite = zfSFML.spriteInMiddle(zeoFlowSprite, window.getSize().x, window.getSize().y);
				window.draw(zeoFlowSprite);
			} 
			else
			{
				clock.restart();
				currentScreen = 1;
			}
		} else if(currentScreen == 1) {

			showGameScreen();

		}
        window.display();
	}

	getchar();

	return 0;
}