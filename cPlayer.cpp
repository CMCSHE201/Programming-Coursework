/*
=================
cPlayer.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlayer.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cPlayer::cPlayer() : cSprite()
{
	this->playerVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPlayer::update(double deltaTime)
{
	auto rads = PI / 180.0f * (this->getSpriteRotAngle() - 90.0f);

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (float)(sin(rads));
	direction.Y = (float)(cos(rads)) * (-1);

	SDL_Rect currentSpritePos = this->getSpritePos();
	
	currentSpritePos.x += (int)(this->playerVelocity * direction.X * this->move * deltaTime * 3);

	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
Sets the velocity for the player
=================================================================
*/
void cPlayer::setPlayerVelocity(int playerVel)
{
	playerVelocity = playerVel;
}
/*
=================================================================
Gets the player velocity
=================================================================
*/
int cPlayer::getPlayerVelocity()
{
	return playerVelocity;
}
/*
=================================================================
Sets the move value for the player
=================================================================
*/
void cPlayer::setPlayerMove(int playerMove)
{
	move = playerMove;
}
/*
=================================================================
Gets the player move value
=================================================================
*/
int cPlayer::getPlayerMove()
{
	return move;
}