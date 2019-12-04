#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "ZeoFlow_SFML.h"
#include "MD2.h"
#include "Collision.hpp"

//Compiler Directives
using namespace std;
using namespace zeoFlow;
using namespace sf;
using namespace Collision;

RenderWindow window(VideoMode(800, 500), "Dangerous Racing");
ZeoFlow_SFML zfSFML;

sf::Clock inGameClock;
const int SCENE_SPLASH_SCREEN = 0;
const int SCENE_GAME_MENU_SCREEN = 1;
const int SCENE_GAME_SCREEN = 2;
const int SCENE_OPTIONS_SCREEN = 3;
const int SCENE_SELECT_LVL = 4;

int levesUnlocked = 3;
int currentScreen = SCENE_SPLASH_SCREEN;
int raceLvl;
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

	int totalCheckPoints()
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
sf::Font font1(zfSFML.loadFont("Assets/fonts/", "big_space", "otf"));
MD2 btnLvl;

float speed=0,angle=0;
float maxSpeed=15;
float turnSpeed=0.05;
int offsetX=0,offsetY=0;
bool raceStarted;
sf::Text inRaceText, inRaceTime, inRaceLap, inRacePlace;
void showGameScreen() {

	bool Up=false,Right=false,Down=false,Left=false;

	if (Keyboard::isKeyPressed(Keyboard::Up)) Up=true;
	if (Keyboard::isKeyPressed(Keyboard::Right)) Right=true;
	if (Keyboard::isKeyPressed(Keyboard::Down)) Down=true;
	if (Keyboard::isKeyPressed(Keyboard::Left)) Left=true;

	float acc = 0.2, dec = 0.2;
	
	srand(time(NULL));
	
	if (Up && speed<maxSpeed + rand()%2 + 1 && raceStarted)
		if (speed < 0)  speed += dec;
		else  speed += acc + rand()%3/10;
				
	if (Down && speed>-maxSpeed && raceStarted)
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

	if(raceStarted) {
		for(int i=0; i<carsPerLvl; i++) {
			if(i != userCar && car[i].speed < maxSpeed + carsPerLvl - i) {
				car[i].speed += acc;
			}
		}
	}
	
	if (car[userCar].x>320 && car[userCar].x<1330) offsetX = car[userCar].x-320;
	if (car[userCar].y>240 && car[userCar].y<2928) offsetY = car[userCar].y-240;
	cout<<userCar<<' '<<carsPerLvl<<' '<<car[1].lap<<'\n';

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
	
	int userPlace = 1;
	for(int i=0; i<carsPerLvl; i++) {
		if(car[i].totalCheckPoints() > car[userCar].totalCheckPoints()) {
			userPlace++;
		}
	}
	
	if((int) inGameClock.getElapsedTime().asSeconds() == 5) {
		inRaceText.setString("START!");
		inRaceText.setFont(font1);
		inRaceText.setCharacterSize(120);
		inRaceText.setOutlineColor(sf::Color::Black);
		inRaceText.setOutlineThickness(3);
		inRaceText.setColor(sf::Color::White);
		inRaceText.setOrigin(inRaceText.getGlobalBounds().width/2, inRaceText.getGlobalBounds().height/2);
		inRaceText.setPosition(window.getSize().x/2, window.getSize().y/2);
		window.draw(inRaceText);
		raceStarted = true;
	} else if((int) inGameClock.getElapsedTime().asSeconds() == 4) {
		inRaceText.setString("1");
		inRaceText.setFont(font1);
		inRaceText.setCharacterSize(120);
		inRaceText.setOutlineColor(sf::Color::Black);
		inRaceText.setOutlineThickness(3);
		inRaceText.setColor(sf::Color::White);
		inRaceText.setOrigin(inRaceText.getGlobalBounds().width/2, inRaceText.getGlobalBounds().height/2);
		inRaceText.setPosition(window.getSize().x/2, window.getSize().y/2);
		window.draw(inRaceText);
	} else if((int) inGameClock.getElapsedTime().asSeconds() == 3) {
		inRaceText.setString("2");
		inRaceText.setFont(font1);
		inRaceText.setCharacterSize(120);
		inRaceText.setOutlineColor(sf::Color::Black);
		inRaceText.setOutlineThickness(3);
		inRaceText.setColor(sf::Color::White);
		inRaceText.setOrigin(inRaceText.getGlobalBounds().width/2, inRaceText.getGlobalBounds().height/2);
		inRaceText.setPosition(window.getSize().x/2, window.getSize().y/2);
		window.draw(inRaceText);
	} else if((int) inGameClock.getElapsedTime().asSeconds() == 2) {
		inRaceText.setString("3");
		inRaceText.setFont(font1);
		inRaceText.setCharacterSize(120);
		inRaceText.setOutlineColor(sf::Color::Black);
		inRaceText.setOutlineThickness(3);
		inRaceText.setColor(sf::Color::White);
		inRaceText.setOrigin(inRaceText.getGlobalBounds().width/2, inRaceText.getGlobalBounds().height/2);
		inRaceText.setPosition(window.getSize().x/2, window.getSize().y/2);
		window.draw(inRaceText);
	}

	if(raceStarted) {
		string lap;
		if(car[userCar].lap!=0) {
			lap = to_string(car[userCar].lap);
		} else {
			lap = to_string(1);
		}
		
		inRaceLap.setString("Lap: " + lap + "/" + to_string(lapsLvl1));
		inRaceLap.setFont(font1);
		inRaceLap.setCharacterSize(30);
		inRaceLap.setOutlineColor(sf::Color::Black);
		inRaceLap.setOutlineThickness(3);
		inRaceLap.setColor(sf::Color::White);
		inRaceLap.setPosition(20, 10);
		window.draw(inRaceLap);
		
		string seconds;
		seconds = to_string(inGameClock.getElapsedTime().asSeconds() - 5);

		inRaceTime.setString("Time: " + seconds);
		inRaceTime.setFont(font1);
		inRaceTime.setCharacterSize(30);
		inRaceTime.setOutlineColor(sf::Color::Black);
		inRaceTime.setOutlineThickness(3);
		inRaceTime.setColor(sf::Color::White);
		inRaceTime.setPosition(20, 40);
		window.draw(inRaceTime);

		inRacePlace.setString("Place: " + to_string(userPlace) + "/" + to_string(carsPerLvl));
		inRacePlace.setFont(font1);
		inRacePlace.setCharacterSize(30);
		inRacePlace.setOutlineColor(sf::Color::Black);
		inRacePlace.setOutlineThickness(3);
		inRacePlace.setColor(sf::Color::White);
		inRacePlace.setPosition(20, 70);
		window.draw(inRacePlace);
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

bool isLvlUnlocked(int lvl)
{
	return lvl <= levesUnlocked;
}

void gameMenuScreen()
{

	sf::RectangleShape selectLvlBg;
	selectLvlBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	selectLvlBg.setFillColor(sf::Color(43, 43, 43));
	window.draw(selectLvlBg);
	
	sf::Text btnTxt;

	btnLvl.setLocation(window.getSize().x/2, window.getSize().y/2 - 100);
	btnLvl.setUnlocked(true);
	btnTxt = btnLvl.drawBtnString("PLAY", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "PLAY", 30, font1);
	if(btnLvl.btnClicked(window)) {
		currentScreen = SCENE_SELECT_LVL;
	}

	btnLvl.setLocation(window.getSize().x/2, window.getSize().y/2);
	btnLvl.setUnlocked(true);
	btnTxt = btnLvl.drawBtnString("SELECT CAR", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "SELECT CAR", 30, font1);
	if(btnLvl.btnClicked(window)) {

	}

	btnLvl.setLocation(window.getSize().x/2, window.getSize().y/2 + 100);
	btnLvl.setUnlocked(true);
	btnTxt = btnLvl.drawBtnString("HOW TO PLAY", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "HOW TO PLAY", 30, font1);
	if(btnLvl.btnClicked(window)) {

	}

}

void selectLvl(int lvl)
{
	inGameClock.restart();
	raceLvl = lvl;
	raceStarted = false;
}

void gameSelectLvl()
{

	sf::RectangleShape selectLvlBg;
	selectLvlBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
	selectLvlBg.setFillColor(sf::Color(255,167,38));
	window.draw(selectLvlBg);
	
	sf::Text btnTxt, inGameExit;

	inGameExit.setFont(font1);
	inGameExit.setString("BACK");
	inGameExit.setPosition(window.getSize().x - 100, 50);
	inGameExit.setCharacterSize(26);
	sf::IntRect btnCharactersRect(inGameExit.getPosition().x - inGameExit.getGlobalBounds().width / 2,
		inGameExit.getPosition().y, inGameExit.getGlobalBounds().width, inGameExit.getGlobalBounds().height * 2);
	if (btnCharactersRect.contains(sf::Mouse::getPosition(window))) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			currentScreen = SCENE_GAME_MENU_SCREEN;
		}
		inGameExit.setFillColor(sf::Color(255, 255, 255));
	} else {
		inGameExit.setFillColor(sf::Color(255,224,178));
	}
	inGameExit.setOrigin(inGameExit.getGlobalBounds().width/2, 0);
	window.draw(inGameExit);

	btnLvl.setLocation(window.getSize().x/2 - 150, window.getSize().y/2 - 100);
	btnLvl.setUnlocked(isLvlUnlocked(1));
	btnTxt = btnLvl.drawBtnString("LVL 1", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "LVL 1", 30, font1);
	if(btnLvl.btnClicked(window)) {
		selectLvl(1);
		currentScreen = SCENE_GAME_SCREEN;
	}

	btnLvl.setLocation(window.getSize().x/2, window.getSize().y/2);
	btnLvl.setUnlocked(isLvlUnlocked(2));
	btnTxt = btnLvl.drawBtnString("LVL 2", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "LVL 2", 30, font1);
	if(btnLvl.btnClicked(window)) {
		selectLvl(2);
		currentScreen = SCENE_GAME_SCREEN;
	}

	btnLvl.setLocation(window.getSize().x/2 + 150, window.getSize().y/2 + 100);
	btnLvl.setUnlocked(isLvlUnlocked(3));
	btnTxt = btnLvl.drawBtnString("LVL 3", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "LVL 3", 30, font1);
	if(btnLvl.btnClicked(window)) {
		selectLvl(3);
		currentScreen = SCENE_GAME_SCREEN;
	}
}

int main()
{
	window.setFramerateLimit(60);
	
	Sprite zeoFlowSprite(zfSFML.loadSpriteFromTexture("Assets/", "zeoflow_logo", "png"));

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

		if (currentScreen == SCENE_SPLASH_SCREEN)
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
				currentScreen = SCENE_GAME_MENU_SCREEN;
			}
		} else if(currentScreen == SCENE_GAME_MENU_SCREEN) {

			gameMenuScreen();

		} else if(currentScreen == SCENE_GAME_SCREEN) {

			showGameScreen();

		} else if(currentScreen == SCENE_SELECT_LVL) {

			gameSelectLvl();

		}
        window.display();
	}

	getchar();

	return 0;
}