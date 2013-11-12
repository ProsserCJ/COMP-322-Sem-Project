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

class Lifeform: public Mover{
public:
	Lifeform() {holdingObject = false; heldObject = 0;scored = false;}
	int getAttackStrength() {return atk;}
	void setAttackStrength(int a) { atk = a; }
	int getStrength() {return strength;}
	void setStrength(int s) {strength = s;}
	int getHealth() { return health; }
	void setHealth(int h) { health = h; }
    void subHealth(int diff) { health -= diff; }
    bool getHolding() {return holdingObject;}
    void setHeldObject(Carriable* item) {heldObject = item;}
    Carriable* getHeldObject() {return heldObject;}
    void setHolding(bool holding) {holdingObject = holding;}
	virtual void die() {}
	virtual void setNormal()=0;
	virtual void setHurt()=0;
	virtual void setAtk()=0;
	virtual bool isNormal()=0;
	virtual bool isAtk()=0;

    virtual GridLoc takeTurn() = 0;
	bool getScored() {return scored;}
	int getScoredPoints() {return scorePoints;}
	void setScored(bool s) {scored = s;}
	void setScorePoints(int p) {scorePoints = p;}

private:
    int health, atk, strength;
    bool holdingObject;
    Carriable* heldObject;
	//used to communicate with the level that they scored
	bool scored;
	int scorePoints;
};

#endif