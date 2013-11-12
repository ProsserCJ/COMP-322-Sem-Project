/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines a heads up display class.  Like Object,
 it is based on Kelly's Entity to handle display
*/ 

#ifndef _HUD_H
#define _HUD_H
#define WIN32_LEAN_AND_MEAN

#include "constants.h"
#include "entity.h"

namespace hudNS{
	const float UPPER_X = GAME_WIDTH - HUD_WIDTH;
	const float UPPER_Y = 0;
}

class HUD: public Entity{
public:
	HUD() : Entity() {};
	bool wasClicked();
private:
};

#endif