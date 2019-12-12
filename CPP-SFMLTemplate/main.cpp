#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "ZeoFlow_SFML.h"
#include "MD2.h"
#include "Collision.hpp"
#include "CarB.cpp"

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
int raceLvl = 1;
const float pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679;

int pointsPerLvl, cpPerLvl;

const int pointsLvl1 = 12;
const int pointsLvl2 = 22;
const int pointsLvl3 = 46;

int pLvl1[pointsLvl1][2] = {
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

int pLvl2[pointsLvl2][2] = {
	221, 747,
	364, 323,
	584, 238,
	807, 196,
	1092, 228,
	1298, 342,
	1397, 572,
	1199, 972,
	1420, 1218,
	1687, 1192,
	1874, 1281,
	1973, 1464,
	1991, 1665,
	1977, 1950,
	1878, 2193,
	1491, 2323,
	1125, 2310,
	761, 2340,
	466, 2240,
	270, 2011,
	189, 1692,
	260, 1310
};

int pLvl3[pointsLvl3][2] = {
	1451, 1930,
	785, 2013,
	498, 1997,
	272, 1834,
	198, 1701,
	143, 1473,
	220, 1225,
	349, 1056,
	643, 991,
	1007, 1023,
	1373, 1134,
	1735, 1125,
	1985, 990,
	2182, 708,
	2249, 531,
	2355, 214,
	2537, 140,
	2700, 120,
	2962, 186,
	3087, 303,
	3276, 466,
	3716, 593,
	3919, 677,
	4003, 916,
	3934, 1113,
	3764, 1262,
	3536, 1285,
	3330, 1110,
	3079, 868,
	2890, 751,
	2651, 707,
	2452, 788,
	2352, 916,
	2328, 1092,
	2459, 1307,
	2655, 1315,
	2846, 1281,
	3154, 1381,
	3499, 1498,
	3644, 1654,
	3642, 1858,
	3494, 2001,
	3287, 2038,
	2995, 1956,
	2686, 1888,
	2203, 1925
};

int userCar;
int lapsPerLvl[3] = {
	//lvl 1
	9,
	//lvl 2
	12,
	//lvl 3
	15
};

int carsPerLvl[3] = {
	//lvl 1
	4,
	//lvl 2
	6,
	//lvl 3
	6
};

const int cpLvl1 = 7;
int checkpointsLvl1[cpLvl1][4] = {
	149, 802, 507, 802,
	915, 97, 915, 454,
	1323, 802, 1681, 802,
	1323, 2350, 1681, 2350,
	915, 2710, 915, 3069,
	149, 2366, 507, 2366,
	149, 1748, 507, 1748
};

const int cpLvl2 = 9;
int checkpointsLvl2[cpLvl2][4] = {
	118, 759, 477, 759,
	800, 133, 800, 491,
	1118, 806, 1476, 806,
	1717, 1635, 2075, 1635,
	1712, 1864, 2071, 1864,
	1400, 2054, 1400, 2411,
	796, 2053, 796, 2411,
	118, 1669, 476, 1669,
	118, 1140, 476, 1140
};

const int cpLvl3 = 13;
int checkpointsLvl3[cpLvl3][4] = {
	740, 1749, 740, 2105,
	88, 1473, 446, 1473,
	755, 847, 755, 1205,
	1454, 847, 1454, 1205,
	1980, 474, 2293, 666,
	2883, 479, 3012, 139,
	3550, 795, 3728, 477,
	3308, 1295, 3421, 947,
	2854, 1068, 3015, 740,
	2696, 1484, 2822, 1143,
	3149, 1657, 3269, 1313,
	3014, 1749, 3014, 2105,
	2118, 1749, 2118, 2105
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
	int health;
	Sprite sCar;
	
	Car() {
		restart();
	}

	void restart() {
		speed=0;
		angle=0;
		n=0;
		health=200;
		lap=0;
	}

	int totalCheckPoints()
	{
		int carPlace = lap * cpPerLvl + currentCheckPoint;
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
		if(raceLvl == 1) {
			int x1 = checkpointsLvl1[currentCheckPoint][0];
			int x2 = checkpointsLvl1[currentCheckPoint][2];
			int y1 = checkpointsLvl1[currentCheckPoint][1];
			int y2 = checkpointsLvl1[currentCheckPoint][3];
			int carX = (int) x;
			int carY = (int) y;
			if(checkpointsLvl1[currentCheckPoint][1] == checkpointsLvl1[currentCheckPoint][3]) {
				if(carY >= y1 - 15 && carY <= y1 + 15 && carX >= x1 - 15 && carX <= x2 + 15) {
					currentCheckPoint++;
					increaseSpeed();
				}
			} else {
				if(carX >= x1 - 15 && carX <= x1 + 15 && carY >= y1 - 15 && carY <= y2 + 15) {
					currentCheckPoint++;
					increaseSpeed();
				}
			}
		} else if(raceLvl == 2) {
			int x1 = checkpointsLvl2[currentCheckPoint][0];
			int x2 = checkpointsLvl2[currentCheckPoint][2];
			int y1 = checkpointsLvl2[currentCheckPoint][1];
			int y2 = checkpointsLvl2[currentCheckPoint][3];
			int carX = (int) x;
			int carY = (int) y;
			if(checkpointsLvl2[currentCheckPoint][1] == checkpointsLvl2[currentCheckPoint][3]) {
				if(carY >= y1 - 15 && carY <= y1 + 15 && carX >= x1 - 15 && carX <= x2 + 15) {
					currentCheckPoint++;
					increaseSpeed();
				}
			} else {
				if(carX >= x1 - 15 && carX <= x1 + 15 && carY >= y1 - 15 && carY <= y2 + 15) {
					currentCheckPoint++;
					increaseSpeed();
				}
			}
		} else if(raceLvl == 3) {
			int x1 = checkpointsLvl3[currentCheckPoint][0];
			int x2 = checkpointsLvl3[currentCheckPoint][2];
			int y1 = checkpointsLvl3[currentCheckPoint][1];
			int y2 = checkpointsLvl3[currentCheckPoint][3];
			int carX = (int) x;
			int carY = (int) y;
			if(y1>y2) {
				int y3 = y1;
				y1 = y2;
				y2 = y3;
			}
			if(carY >= y1 - 30 && carY <= y2 + 30 && carX >= x1 - 30 && carX <= x2 + 30) {
				currentCheckPoint++;
				increaseSpeed();
			}
		}
		
		if (currentCheckPoint == cpPerLvl) {
			currentCheckPoint = 0;
			lap++;
		}
	}

	void move()
	{
		x += sin(angle) * speed;
		y -= cos(angle) * speed;
		if(raceLvl == 1) {
			if(x<75) x=75;
			if(x>1728) x=1728;
			if(y<80) y=80;
			if(y>3108) y=3108;
		} else if(raceLvl == 2) {
			if(x<75) x=75;
			if(x>2095) x=2095;
			if(y<80) y=80;
			if(y>2444) y=2444;
		} else if(raceLvl == 3) {
			if(x<75) x=75;
			if(x>4030) x=4030;
			if(y<80) y=80;
			if(y>2130) y=2130;
		}
		if(userCar != carId) {
			findTarget();
		}
		checkCheckpoint();
	}

	void findTarget()
	{
		float tx, ty;
		if(raceLvl == 1) {
			tx=pLvl1[n][0];
			ty=pLvl1[n][1];
		} else if(raceLvl == 2) {
			tx=pLvl2[n][0];
			ty=pLvl2[n][1];
		} else if(raceLvl == 3) {
			tx=pLvl3[n][0];
			ty=pLvl3[n][1];
		}

		float beta = angle-atan2(tx-x,-ty+y);

		if (sin(beta)<0) angle+=0.005*speed;
		else angle-=0.005*speed;

		if ((x-tx)*(x-tx)+(y-ty)*(y-ty)<25*25) n=(n+1)%pointsPerLvl;
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
TracksBackground tracksBackgroundMask[3];
TrackObjects trackObjects[2];
CarModels carModels[2];
Car car[8];
CarB cars[6];
sf::Font font1(zfSFML.loadFont("Assets/fonts/", "big_space", "otf"));
MD2 btnLvl;

float speed=0,angle=0;
float maxSpeed=15;
float turnSpeed=0.05;
int offsetX=0,offsetY=0;
bool raceStarted, outOfTrack = false;
sf::Text inRaceText, inRaceTime, inRaceLap, inRacePlace;
sf::Vector2i lastPos;

void showGameScreen() {

	bool Up=false,Right=false,Down=false,Left=false;

	if (Keyboard::isKeyPressed(Keyboard::Up)) Up=true;
	if (Keyboard::isKeyPressed(Keyboard::Right)) Right=true;
	if (Keyboard::isKeyPressed(Keyboard::Down)) Down=true;
	if (Keyboard::isKeyPressed(Keyboard::Left)) Left=true;
	if (Keyboard::isKeyPressed(Keyboard::Key::E)) currentScreen = SCENE_GAME_MENU_SCREEN;

	float acc = 0.2, dec = 0.2;
	
	srand(time(NULL));
	
	if(outOfTrack) {
		maxSpeed = 1;
		turnSpeed = 0.01;
	} else {
		maxSpeed = 15;
		turnSpeed = 0.05;
	}

	if(outOfTrack && raceStarted) {
		if (Up && speed<maxSpeed + rand()%2 + 1) {
			if (speed < 0) {
				speed += dec;
			} else {
				speed += acc + rand()%3/10;
			}
		} else {
			if(speed > 10) {
				turnSpeed = 0.002;
				speed -= 0.1;
			} else if(speed > 6) {
				turnSpeed = 0.004;
				speed -= 0.05;
			} else {
				speed -= 0.1;
			}
		}
	} else {
		if (Up && speed<maxSpeed + rand()%2 + 1 && raceStarted) {
			if (speed < 0) {
				speed += dec;
			} else {
				speed += acc + rand()%3/10;
			}
		}
	}

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
	for(int i=0;i<carsPerLvl[raceLvl - 1];i++) {
		if(i != userCar) {
			car[i].move();
		}
	}
			
	for(int i=0;i<carsPerLvl[raceLvl - 1];i++){
		if(i != userCar) {
			car[i].findTarget();
		}
	}

	srand(time(NULL));
	for(int i=0;i<carsPerLvl[raceLvl - 1];i++)
		for(int j=0;j<carsPerLvl[raceLvl - 1];j++) {
			int dx=0, dy=0;
			if (PixelPerfectDetection(car[i].sCar, car[j].sCar) && i != j) {
				dx = car[i].x-car[j].x;
				dy = car[i].y-car[j].y;
				if(car[i].speed != 0) {
					car[i].x+=dx*car[i].speed/50.0;
					car[i].y+=dy*car[i].speed/50.0;
				}
				if(i != userCar && j == userCar) {
					car[i].health-=10;
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
		for(int i=0; i<carsPerLvl[raceLvl - 1]; i++) {
			if(i != userCar && car[i].speed < maxSpeed + carsPerLvl[raceLvl - 1] - i) {
				car[i].speed += acc;
			}
		}
	}
	
	if(raceLvl == 1) {
		if (car[userCar].x>window.getSize().x/2 && car[userCar].x<1330) {
			offsetX = car[userCar].x-window.getSize().x/2;
		}
		if (car[userCar].y>window.getSize().y/2 && car[userCar].y<2928) {
			offsetY = car[userCar].y-window.getSize().y/2;
		}
	} else if(raceLvl == 2) {
		if (car[userCar].x>window.getSize().x/2 && car[userCar].x<1705) {
			offsetX = car[userCar].x-window.getSize().x/2;
		}
		if (car[userCar].y>window.getSize().y/2 && car[userCar].y<2300) {
			offsetY = car[userCar].y-window.getSize().y/2;
		}
	} else if(raceLvl == 3) {
		if (car[userCar].x>window.getSize().x/2 && car[userCar].x<3670) {
			offsetX = car[userCar].x-window.getSize().x/2;
		}
		if (car[userCar].y>window.getSize().y/2 && car[userCar].y<1930) {
			offsetY = car[userCar].y-window.getSize().y/2;
		} else if (car[userCar].y>1930) {
			offsetY = 1930-window.getSize().y/2;
		}
	}
	
	tracksBackground[raceLvl - 1].backgroundTrack.setColor(sf::Color(255, 255, 255, 255));
	tracksBackground[raceLvl - 1].backgroundTrack.setScale(1, 1);
	tracksBackground[raceLvl - 1].backgroundTrack.setPosition(-offsetX,-offsetY);
	window.draw(tracksBackground[raceLvl - 1].backgroundTrack);
	
	tracksBackgroundMask[raceLvl - 1].backgroundTrack.setPosition(-offsetX,-offsetY);
	if (PixelPerfectDetection(car[userCar].sCar, tracksBackgroundMask[raceLvl - 1].backgroundTrack)) {
		outOfTrack = true;
	} else {
		outOfTrack = false;
	}

	sf::RectangleShape menuSqr;
	menuSqr.setSize(sf::Vector2f(20, 20));
	menuSqr.setOrigin(10, 10);
	menuSqr.setFillColor(sf::Color(255,193,7));
	if(raceLvl == 1) {
		for(int i=0; i<cpLvl1; i++) {
			menuSqr.setPosition(checkpointsLvl1[i][0]-offsetX, checkpointsLvl1[i][1]-offsetY);
			window.draw(menuSqr);
			menuSqr.setPosition(checkpointsLvl1[i][2]-offsetX, checkpointsLvl1[i][3]-offsetY);
			window.draw(menuSqr);
		}
	} else if(raceLvl == 2) {
		for(int i=0; i<cpLvl2; i++) {
			menuSqr.setPosition(checkpointsLvl2[i][0]-offsetX, checkpointsLvl2[i][1]-offsetY);
			window.draw(menuSqr);
			menuSqr.setPosition(checkpointsLvl2[i][2]-offsetX, checkpointsLvl2[i][3]-offsetY);
			window.draw(menuSqr);
		}
	} else if(raceLvl == 3) {
		for(int i=0; i<cpLvl3; i++) {
			menuSqr.setPosition(checkpointsLvl3[i][0]-offsetX, checkpointsLvl3[i][1]-offsetY);
			window.draw(menuSqr);
			menuSqr.setPosition(checkpointsLvl3[i][2]-offsetX, checkpointsLvl3[i][3]-offsetY);
			window.draw(menuSqr);
		}
	}

	sf::Vector2i MouseCursorLocation = sf::Mouse::getPosition(window);
	menuSqr.setPosition(MouseCursorLocation.x, MouseCursorLocation.y);
	window.draw(menuSqr);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if(lastPos.x != MouseCursorLocation.x && lastPos.y != MouseCursorLocation.y) {
			//cout<<MouseCursorLocation.x + offsetX<<", "<<MouseCursorLocation.y + offsetY<<'\n';
			lastPos = sf::Mouse::getPosition(window);
			//currentScreen = SCENE_GAME_MENU_SCREEN;
		}
	}

	for(int i=0;i<carsPerLvl[raceLvl - 1];i++)
	{
		car[i].sCar.setPosition(car[i].x-offsetX,car[i].y-offsetY);
		car[i].sCar.setRotation(car[i].angle*180/pi);
		window.draw(car[i].sCar);
	}
	
	if(raceLvl == 1) {
		for(int i=0; i<2; i++) 
		{
			trackObjects[i].sprite.setPosition(trackObjects[i].x-offsetX,trackObjects[i].y-offsetY);
			trackObjects[i].sprite.setRotation(trackObjects[i].angle*180/pi);
			window.draw(trackObjects[i].sprite);
		}
	}
	
	int userPlace = 1;
	for(int i=0; i<carsPerLvl[raceLvl - 1]; i++) {
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
	
		sf::RectangleShape carLocation;
		carLocation.setSize(sf::Vector2f(4, 4));
		carLocation.setOrigin(2, 2);
		
		int bgInitialWidth = tracksBackground[raceLvl - 1].backgroundTrack.getGlobalBounds().width;
		int bgInitialHeight = tracksBackground[raceLvl - 1].backgroundTrack.getGlobalBounds().height;

		tracksBackground[raceLvl - 1].backgroundTrack.setColor(sf::Color(255, 255, 255, 200));
		tracksBackground[raceLvl - 1].backgroundTrack.setScale(0.045, 0.045);
		tracksBackground[raceLvl - 1].backgroundTrack.setPosition(window.getSize().x - tracksBackground[raceLvl - 1].backgroundTrack.getGlobalBounds().width - 10, 10);
		window.draw(tracksBackground[raceLvl - 1].backgroundTrack);
		
		int miniMapW = tracksBackground[raceLvl - 1].backgroundTrack.getGlobalBounds().width;
		int miniMapH = tracksBackground[raceLvl - 1].backgroundTrack.getGlobalBounds().height;
		int miniMapLeft = tracksBackground[raceLvl - 1].backgroundTrack.getGlobalBounds().left;
		int miniMapTop = tracksBackground[raceLvl - 1].backgroundTrack.getGlobalBounds().top;

		for(int i=0; i<carsPerLvl[raceLvl - 1]; i++) {
			float carMapX = miniMapW * car[i].x / bgInitialWidth;
			float carMapY = miniMapH * car[i].y / bgInitialHeight;

			if(i!=userCar) {
				carLocation.setFillColor(sf::Color(21, 21, 21));
			} else {
				carLocation.setFillColor(sf::Color(198,255,0));
			}

			carLocation.setPosition(miniMapLeft + carMapX, miniMapTop + carMapY);
			window.draw(carLocation);
		}

		string lap;
		if(car[userCar].lap!=0) {
			lap = to_string(car[userCar].lap);
		} else {
			lap = to_string(1);
		}
		
		inRaceLap.setString("Lap: " + lap + "/" + to_string(lapsPerLvl[raceLvl - 1]));
		inRaceLap.setFont(font1);
		inRaceLap.setCharacterSize(30);
		inRaceLap.setOutlineColor(sf::Color::Black);
		inRaceLap.setOutlineThickness(3);
		inRaceLap.setColor(sf::Color::White);
		inRaceLap.setPosition(20, 10);
		window.draw(inRaceLap);
		
		string seconds;
		seconds = to_string((int) inGameClock.getElapsedTime().asSeconds() - 5);

		inRaceTime.setString("Time: " + seconds);
		inRaceTime.setFont(font1);
		inRaceTime.setCharacterSize(30);
		inRaceTime.setOutlineColor(sf::Color::Black);
		inRaceTime.setOutlineThickness(3);
		inRaceTime.setColor(sf::Color::White);
		inRaceTime.setPosition(20, 40);
		window.draw(inRaceTime);

		inRacePlace.setString("Place: " + to_string(userPlace) + "/" + to_string(carsPerLvl[raceLvl - 1]));
		inRacePlace.setFont(font1);
		inRacePlace.setCharacterSize(30);
		inRacePlace.setOutlineColor(sf::Color::Black);
		inRacePlace.setOutlineThickness(3);
		inRacePlace.setColor(sf::Color::White);
		inRacePlace.setPosition(20, 70);
		window.draw(inRacePlace);
	}

}

void loadGameAssets() {

	Sprite bg1 = zfSFML.loadSpriteFromTexture("Assets/", "track_1", "png");
	bg1.setScale(1, 1);
	tracksBackground[0].backgroundTrack = bg1;

	Sprite bg1Mask = zfSFML.loadSpriteFromTexture("Assets/", "track_1_mask", "png");
	bg1Mask.setScale(1, 1);
	tracksBackgroundMask[0].backgroundTrack = bg1Mask;

	Sprite bg2 = zfSFML.loadSpriteFromTexture("Assets/", "track_2", "png");
	bg2.setScale(1, 1);
	tracksBackground[1].backgroundTrack = bg2;

	Sprite bg2Mask = zfSFML.loadSpriteFromTexture("Assets/", "track_2_mask", "png");
	bg2Mask.setScale(1, 1);
	tracksBackgroundMask[1].backgroundTrack = bg2Mask;

	Sprite bg3 = zfSFML.loadSpriteFromTexture("Assets/", "track_3", "png");
	bg3.setScale(1, 1);
	tracksBackground[2].backgroundTrack = bg3;

	Sprite bg3Mask = zfSFML.loadSpriteFromTexture("Assets/", "track_3_mask", "png");
	bg3Mask.setScale(1, 1);
	tracksBackgroundMask[2].backgroundTrack = bg3Mask;

	Sprite sCar1(zfSFML.loadSpriteFromTexture("Assets/", "car1", "png"));
	sCar1.setOrigin(61, 128);
	sCar1.scale(0.7, 0.7);
	carModels[0].carSprite = sCar1;
	Sprite sCar2(zfSFML.loadSpriteFromTexture("Assets/", "car2", "png"));
	sCar2.setOrigin(128, 128);
	sCar2.scale(0.7, 0.7);
	carModels[1].carSprite = sCar2;

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

void selectLvl(int lvl, int points, int cp)
{
	inGameClock.restart();
	raceLvl = lvl;
	raceStarted = false;
	pointsPerLvl = points;
	cpPerLvl = cp;
	userCar = carsPerLvl[raceLvl - 1] - 1;
	speed = 0;

	if(raceLvl == 1) {
		angle = 0;
		for(int i=0;i<carsPerLvl[raceLvl - 1];i++)
		{
			car[i].restart();
			car[i].x= 260 + i%2*135;
			if(i<2) {
				car[i].y=1920;
			} else if(i<4) {
				car[i].y=1920 + 200;
			}
			car[i].sCar = carModels[0].carSprite;
			car[i].carId = i;
			car[i].currentCheckPoint = cpPerLvl - 1;
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
		angle = 0;
		for(int i=0;i<carsPerLvl[raceLvl - 1];i++)
		{
			car[i].restart();
			car[i].x= 229 + i%2*135;
			if(i<2) {
				car[i].y=1300;
			} else if(i<4) {
				car[i].y=1300 + 200;
			} else if(i<6) {
				car[i].y=1300 + 400;
			}
			car[i].sCar = carModels[0].carSprite;
			car[i].carId = i;
			car[i].currentCheckPoint = cpPerLvl - 1;
		}
	} else if(raceLvl == 3) {
		angle = 4.71867;
		for(int i=0;i<carsPerLvl[raceLvl - 1];i++)
		{
			car[i].restart();
			car[i].y= 1996 - i%2*135;
			if(i<2) {
				car[i].x=2307;
			} else if(i<4) {
				car[i].x=2307 + 200;
			} else if(i<6) {
				car[i].x=2307 + 400;
			} else if(i<8) {
				car[i].x=2307 + 800;
			}
			car[i].sCar = carModels[0].carSprite;
			car[i].carId = i;
			car[i].currentCheckPoint = cpPerLvl - 1;
			car[i].angle = angle;
		}
	}
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
		selectLvl(1, pointsLvl1, cpLvl1);
		currentScreen = SCENE_GAME_SCREEN;
	}

	btnLvl.setLocation(window.getSize().x/2, window.getSize().y/2);
	btnLvl.setUnlocked(isLvlUnlocked(2));
	btnTxt = btnLvl.drawBtnString("LVL 2", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "LVL 2", 30, font1);
	if(btnLvl.btnClicked(window)) {
		selectLvl(2, pointsLvl2, cpLvl2);
		currentScreen = SCENE_GAME_SCREEN;
	}

	btnLvl.setLocation(window.getSize().x/2 + 150, window.getSize().y/2 + 100);
	btnLvl.setUnlocked(isLvlUnlocked(3));
	btnTxt = btnLvl.drawBtnString("LVL 3", 30);
	btnTxt.setFont(font1);
	window.draw(btnTxt);
	btnLvl.drawBtn(window, "LVL 3", 30, font1);
	if(btnLvl.btnClicked(window)) {
		selectLvl(3, pointsLvl3, cpLvl3);
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
	
	loadGameAssets();

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