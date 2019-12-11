#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Checkpoints.cpp"

class CarB
{
	float x,y,speed,angle;
	int carId, currentCheckPoint, lap, n;
	int cpPerLvl, raceLvl, userCar, pointsPerLvl;
	sf::Sprite sCar;
	Checkpoints checkpoints;

	public:
	  CarB() {
		speed=0;
		angle=0;
		n=0;
		lap=0;
	  }	  

	void restart() {
		speed=0;
		angle=0;
		n=0;
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
			int x1 = checkpoints.checkpointsLvl1[currentCheckPoint][0];
			int x2 = checkpoints.checkpointsLvl1[currentCheckPoint][2];
			int y1 = checkpoints.checkpointsLvl1[currentCheckPoint][1];
			int y2 = checkpoints.checkpointsLvl1[currentCheckPoint][3];
			int carX = (int) x;
			int carY = (int) y;
			if(checkpoints.checkpointsLvl1[currentCheckPoint][1] == checkpoints.checkpointsLvl1[currentCheckPoint][3]) {
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
			int x1 = checkpoints.checkpointsLvl2[currentCheckPoint][0];
			int x2 = checkpoints.checkpointsLvl2[currentCheckPoint][2];
			int y1 = checkpoints.checkpointsLvl2[currentCheckPoint][1];
			int y2 = checkpoints.checkpointsLvl2[currentCheckPoint][3];
			int carX = (int) x;
			int carY = (int) y;
			if(checkpoints.checkpointsLvl2[currentCheckPoint][1] == checkpoints.checkpointsLvl2[currentCheckPoint][3]) {
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
			int x1 = checkpoints.checkpointsLvl3[currentCheckPoint][0];
			int x2 = checkpoints.checkpointsLvl3[currentCheckPoint][2];
			int y1 = checkpoints.checkpointsLvl3[currentCheckPoint][1];
			int y2 = checkpoints.checkpointsLvl3[currentCheckPoint][3];
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
			tx=checkpoints.pLvl1[n][0];
			ty=checkpoints.pLvl1[n][1];
		} else if(raceLvl == 2) {
			tx=checkpoints.pLvl2[n][0];
			ty=checkpoints.pLvl2[n][1];
		} else if(raceLvl == 3) {
			tx=checkpoints.pLvl3[n][0];
			ty=checkpoints.pLvl3[n][1];
		}

		float beta = angle-atan2(tx-x,-ty+y);

		if (sin(beta)<0) angle+=0.005*speed;
		else angle-=0.005*speed;

		if ((x-tx)*(x-tx)+(y-ty)*(y-ty)<25*25) n=(n+1) % pointsPerLvl;
	}

};
