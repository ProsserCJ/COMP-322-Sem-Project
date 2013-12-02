/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file implements parts of the Lifeform class
*/ 

#include "lifeform.h"
#include <string>
#include <stdio.h>
using std::string;

void Lifeform::draw()
{
	Mover::draw();
	if (lostHealth > 0)
	{
		if (++fontDisplayFrameCount < 80)
		{
			char buffer[5];
			sprintf(buffer,"-%i",lostHealth);
			float X = getX() + (getWidth()*getScale())/2-6*getScale();
			float Y = getY() - .125*fontDisplayFrameCount/getScale();
			gameFont->setFontColor(SETCOLOR_ARGB(255 - 3*fontDisplayFrameCount,255,0,0));
			gameFont->print(buffer, X, Y);		
		}
		else{
			fontDisplayFrameCount = 0;
			lostHealth = 0;
		}
	}
}