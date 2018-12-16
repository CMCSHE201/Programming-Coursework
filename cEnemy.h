/*
=================
cEnemy.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CENEMY_H
#define _CENEMY_H
#include "cSprite.h"

class cEnemy : public cSprite
{
private:
	int enemyVelocity;

public:
	cEnemy();
	void update(double deltaTime);		// update method
	void setEnemyVelocity(int enemyVel);   // Sets the velocity for the enemy
	int getEnemyVelocity();				 // Gets the enemy velocity
};
#endif