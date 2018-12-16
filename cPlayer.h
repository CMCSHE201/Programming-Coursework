/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CPLAYER_H
#define _CPLAYER_H
#include "cSprite.h"

class cPlayer : public cSprite
{
private:
	int playerVelocity;
	int move;

public:
	cPlayer();
	void update(double deltaTime);		// Rocket update method
	void setPlayerVelocity(int playerVel);   // Sets the velocity for the rocket
	int getPlayerVelocity();				 // Gets the rocket velocity
	void setPlayerMove(int playerMove);   // Sets the rocket move Value
	int getPlayerMove();				 // Gets the rocket move value
}; 
#endif