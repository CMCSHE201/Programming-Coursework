/*
=================
cEnemy.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cEnemy.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cEnemy::cEnemy() : cSprite()
{
	this->enemyVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cEnemy::update(double deltaTime)
{
	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += (int)(this->getSpriteTranslation().x * deltaTime * 10);

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cEnemy::setEnemyVelocity(int EnemyVel)
{
	enemyVelocity = EnemyVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
int cEnemy::getEnemyVelocity()
{
	return enemyVelocity;
}
