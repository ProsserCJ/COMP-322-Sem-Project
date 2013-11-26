/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines the Broblub class,
 the antagonist Lifeform in our simulation.  It inherits
 many virtual functions from the base Lifeform class.
*/ 

#ifndef __BROBLUB_H
#define __BROBLUB_H

#include "Lifeform.h"
#include "level.h"
#include <vector>
using std::vector;

class Broblub: public Lifeform{
public:
	Broblub() {image = B;setHealth(30);setAttackStrength(2);setSightRadius(1);}	

	//Polymorphic functions
	virtual void move(GridLoc& p);	
	virtual void move(int x, int y);
	virtual void attack(GridLoc& p);
	virtual void attack(int x, int y);	
	virtual GridLoc takeTurn();
	virtual void setNormal();
	virtual void setHurt();
	virtual void setAtk();
	virtual bool isNormal();
	virtual bool isAtk();

	//Other Actions
	void moveRandom(vector<GridLoc> temp);

private:
	void die();
};

#endif