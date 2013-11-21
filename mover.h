/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file is inherited from the skeleton
 provided by Charles Kelly, heavily modified by Chris
 for COMP 441 Program 1.
*/ 

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 planet.h v1.0
// Modified by Chris Prosser

#ifndef _MOVER2_H               // Prevent multiple definitions if this 
#define _MOVER2_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "object.h"
#include "constants.h"

namespace moverNS
{
    const int   WIDTH = 128;                // image width
    const int   HEIGHT = 128;               // image height
    const int   COLLISION_RADIUS = 120/2;   // for circular collision
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/2 - HEIGHT/2;
    const int   TEXTURE_COLS = 2;       // texture has 2 columns
    const int   START_FRAME = 1;        // starts at frame 1
    const int   END_FRAME = 1;          // no animation
	const int	PIXELS_PER_SECOND = 150;	// speed of the mover's movements
	enum DIR {LEFT, RIGHT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, NONE};
}

class Mover : public Object           // inherits from Entity class
{
public:
    Mover();

	//Getters
	moverNS::DIR getDir()			{return dir;}
	double getDistToMove()			{return distToMove;}

	//Setters
	void setDir(moverNS::DIR d);
	void setDistToMove(double d)	{distToMove = d; }

	//Other Actions
	void move(float frameTime);	
	
protected:	
	moverNS::DIR dir;
	double distToMove;
};

#endif