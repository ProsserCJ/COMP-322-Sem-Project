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
#include <vector>
using std::vector;

class level;

class Lifeform: public Mover{
public:

	//Init
	Lifeform() {holdingObject = false; heldObject = 0;scored = false; sightRadius = 1;scorePoints = 50;hurt = false;prevDirection = -1;for(int i=0;i<8;i++)availableDirections[i]=true;}
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
	int getSightRadius()					{return sightRadius;}
	int getMaxHealth()						{return maxHealth;}
	bool getHurtBoolean()					{return hurt;}
	int getPrevDir()						{return prevDirection;}

	//Setters
	void setAttackStrength(int a)			{atk = a;}
	void setHealth(int h)					{health = h;}
    void subHealth(int diff)				{health -= diff; lostHealth = diff; fontDisplayFrameCount = 0;}
	void setStrength(int s)					{strength = s;}    
    void setHeldObject(Carriable* item)		{heldObject = item;}   
    void setHolding(bool holding)			{holdingObject = holding;}
	void setScored(bool s)					{scored = s;}
	void setScorePoints(int p)				{scorePoints = p;}
	void setSightRadius(int sight)			{sightRadius = sight;}
	void setMaxHealth(int m)				{maxHealth = m;}
	void setHurtBoolean(bool h = true)		{hurt = h;}
	void setPrevDir(int d)					{prevDirection = d;}

	//Polymorphic functions
	virtual void die() {}
	virtual void setNormal()=0;
	virtual void setHurt()=0;
	virtual void setAtk()=0;
	virtual bool isNormal()=0;
	virtual bool isAtk()=0;
	virtual GridLoc takeTurn() = 0;
	virtual void draw();	

	//used to determine if a direction is a reasonable way to go
	//used in obstacle avoidance
	//LEFT,RIGHT,UP,DOWN,UP_LEFT,UP_RIGHT,DOWN_LEFT,DOWN_RIGHT
	bool availableDirections[8];

private:
    int health, atk, strength, lostHealth, maxHealth;
	int sightRadius;
    bool holdingObject;
    Carriable* heldObject;
	bool scored;
	bool hurt;
	int scorePoints;
	//used in obstacle avoidance
	int prevDirection;

	//Damage font display
	TextDX* gameFont;
	int fontDisplayFrameCount;
	
};

#endif