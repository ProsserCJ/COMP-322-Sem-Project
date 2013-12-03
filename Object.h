/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines the Object class, an abstract
 base class for all objects used in our Nimkip simulation.  Object
 derives from Kelly's Entity class, which handles all graphical
 display functionality through DirectX
*/ 

#ifndef __OBJECT_H
#define __OBJECT_H

#include "entity.h"
#include "importantTypes.h"

//for nimkips of every color, broblub, coin, food, object, and gap respectively
enum textureType {RN,BN,YN,RB,BB,C,F,O,G,BS,S};

class Object: public Entity{
public:
	Object(): Entity()				{pos.x = getX()/GRID_SIZE*getScale(); pos.y = getY()/GRID_SIZE*getScale();}

	//Getters
	int getXCoord()					{return pos.x;}
	int getYCoord()					{return pos.y;}
	GridLoc getGridLoc()			{return pos;}
	textureType getImage()			{return image;}

	//Setters
	void setGridLoc(int x, int y)	{setX(x*GRID_SIZE*getScale()); setY(y*GRID_SIZE*getScale()); pos = GridLoc(x,y);}
	void setGridLoc(GridLoc& p)		{setX(pos.x*GRID_SIZE*getScale()); setY(pos.y*GRID_SIZE*getScale()); pos = GridLoc(p.x,p.y);}
	void setPosRaw(int x, int y)	{pos = GridLoc(x,y);}	
	void setImage(textureType i)	{image = i;}

protected:
	GridLoc pos;
	textureType image;
};

#endif