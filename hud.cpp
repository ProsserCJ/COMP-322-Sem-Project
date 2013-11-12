/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file implements the heads up display functionality.
 Most of the work for the HUD is handled by its parent classes,
 called in level.cpp
*/ 

#include "hud.h"

bool HUD::wasClicked(){	
	int x = input->getMouseX();
	int y = input->getMouseY();
	return (x >= GAME_WIDTH - HUD_WIDTH && y > 80 && y <= 160);
}