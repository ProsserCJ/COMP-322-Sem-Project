/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines the Lifeform class, which derives from
 Mover (a class to provide screen animation for all things that move) and
 uses several pure virtual functions.  Pointers to this type are used heavily
 in level.cpp to reference all types of Nimkips as well as Broblubs
*/ 

#ifndef __LIFEFORM_H
#define __LIFEFORM_H

#include "mover.h"
#include "constants.h"
#include "carriable.h"
#include "textDX.h"

class level;

class Lifeform: public Mover{
public:

	//Init
	Lifeform() {holdingObject = false; heldObject = 0;scored = false;}
	bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM, TextDX* font)
		{gameFont = font; return Mover::initialize(gamePtr, width, height, ncols, textureM);}
	
	//Getters
	int getAttackStrength()					{return atk;}	
	int getStrength()						{return strength;}	
	int getHealth()							{return health;}
	bool getHolding()						{return holdingObject;}
	Carriable* getHeldObject()				{return heldObject;}
	bool getScored()						{return scored;}
	int getScoredPoints()					{return scorePoints;}

	//Setters
	void setAttackStrength(int a)			{atk = a;}
	void setHealth(int h)					{health = h;}
    void subHealth(int diff)				{health -= diff; lostHealth = diff; fontDisplayFrameCount = 0;}
	void setStrength(int s)					{strength = s;}    
    void setHeldObject(Carriable* item)		{heldObject = item;}   
    void setHolding(bool holding)			{holdingObject = holding;}
	void setScored(bool s)					{scored = s;}
	void setScorePoints(int p)				{scorePoints = p;}

	//Polymorphic functions
	virtual void die() {}
	virtual void setNormal()=0;
	virtual void setHurt()=0;
	virtual void setAtk()=0;
	virtual bool isNormal()=0;
	virtual bool isAtk()=0;
	virtual GridLoc takeTurn() = 0;
	virtual void draw();
	
	

private:
    int health, atk, strength, lostHealth;
    bool holdingObject;
    Carriable* heldObject;
	bool scored;
	int scorePoints;

	//Damage font display
	TextDX* gameFont;
	int fontDisplayFrameCount;
};

#endif