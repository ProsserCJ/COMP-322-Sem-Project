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
			gameFont->setFontColor(SETCOLOR_ARGB(255 - 3*fontDisplayFrameCount,255,0,0));
			gameFont->print(buffer, getX()+getWidth()/2-5, getY() - .125*fontDisplayFrameCount);		
		}
		else{
			fontDisplayFrameCount = 0;
			lostHealth = 0;
		}
	}
}