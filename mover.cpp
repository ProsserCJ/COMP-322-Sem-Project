/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file is inherited from the skeleton
 provided by Charles Kelly, heavily modified by Chris
 for COMP 441 Program 1.
*/ 

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 version 1.0

#include "mover.h"
using namespace moverNS;

//=============================================================================
// default constructor
//=============================================================================
Mover::Mover() : Object()
{
    spriteData.x    = moverNS::X;              // location on screen
    spriteData.y    = moverNS::Y;
    radius          = moverNS::COLLISION_RADIUS;    
    startFrame      = moverNS::START_FRAME;    // first frame of ship animation
    endFrame        = moverNS::END_FRAME;      // last frame of ship animation
    setCurrentFrame(startFrame);
	distToMove = 0;
}

void Mover::chooseDir(GridLoc start, GridLoc end)
{
	if(end.x > start.x)
		{
			if(end.y > start.y)
			{
				setDir(DOWN_RIGHT);
			}
			else if(end.y < start.y)
			{
				setDir(UP_RIGHT);
			}
			else
			{
				setDir(RIGHT);
			}
		}
		else if(end.x < start.x)
		{
			if(end.y > start.y)
			{
				setDir(DOWN_LEFT);
			}
			else if(end.y < start.y)
			{
				setDir(UP_LEFT);
			}
			else
			{
				setDir(LEFT);
			}
		}
		else
		{
			if(end.y > start.y)
			{
				setDir(DOWN);
			}
			else if(end.y < start.y)
			{
				setDir(UP);
			}
		}
}

void Mover::setDir(moverNS::DIR d){
	if (distToMove > 0) return;
	GridLoc temp = getGridLoc();
	int x = temp.x; int y = temp.y;

	switch(d){
		case LEFT:		setPosRaw(x-1,y); break;
		case RIGHT:		setPosRaw(x+1,y); break;
		case UP:		setPosRaw(x,y-1); break;
		case DOWN:		setPosRaw(x,y+1); break;
		case UP_RIGHT:	setPosRaw(x+1,y-1); break;
		case UP_LEFT:	setPosRaw(x-1,y-1); break;
		case DOWN_LEFT:	setPosRaw(x-1,y+1); break;
		case DOWN_RIGHT:setPosRaw(x+1,y+1); break;
	}

	dir = d;
	distToMove = GRID_SIZE*getScale();
}

void Mover::move(float frameTime){	
	if (distToMove <= 0) return;
	float change = PIXELS_PER_SECOND*frameTime;
	if (distToMove < PIXELS_PER_SECOND*frameTime) change = distToMove;

	switch(dir){
	case LEFT:
		setX(getX() - change);		
		break;

	case RIGHT:
		setX(getX() + change);
		break;

	case UP:
		setY(getY() - change);
		break;

	case DOWN:	
		setY(getY() + change);
		break;

	case UP_RIGHT:
		setY(getY() - change);
		setX(getX() + change);
		break;

	case UP_LEFT:
		setY(getY() - change);
		setX(getX() - change);
		break;

	case DOWN_LEFT:
		setY(getY() + change);
		setX(getX() - change);
		break;

	case DOWN_RIGHT:
		setY(getY() + change);
		setX(getX() + change);
		break;
	}
	distToMove -= change;
}

