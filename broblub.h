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
#include "constants.h"
#include "level.h"
#include <vector>
using std::vector;

class Broblub: public Lifeform{
public:
	Broblub() {task=WALK;}	

	//Polymorphic functions
	virtual void move(GridLoc& p);	
	virtual void move(int x, int y);
	virtual void attack(GridLoc& p);
	virtual void attack(int x, int y);	
	virtual GridLoc takeTurn();
	//virtual void setNormal();
	//virtual void setHurt();
	//virtual void setAtk();
	//virtual bool isNormal();
	//virtual bool isAtk();

	//Other Actions
	
	//bool checkImmediateSurroundings();
	virtual void chooseAction() {}
	virtual void goTowardsGoal() {}
	

	//void Broblub::setSpotted(){ setCurrentFrame(BRO_SPOTTED); }

private:
	void die();

protected:
	vector<GridLoc> visibleTiles;
	vector<GridLoc> surroundings;
	Surroundings obstacleAvoidanceSurroundings;
	//Surroundings surroundings;
	GridLoc target;
	GridLoc destination;
	Task task;
	
};

class RedBroblub : public Broblub
{
public:
	RedBroblub() : Broblub() {image = RB; setHealth(30); setMaxHealth(30); setAttackStrength(2);setSightRadius(RED_ENEMY_SIGHT);}

	//Actions
	void moveRandom(vector<GridLoc> temp);
	void chooseAction();
	void goTowardsGoal();
	
	//Polymorphic functions
	virtual void setNormal();
	virtual void setHurt();
	virtual void setAtk();
	virtual void setSpotted();
	virtual bool isNormal();
	virtual bool isAtk();

};

class BlackBroblub : public Broblub
{
public:
	enum PatternDirection {UP, LEFT, RIGHT, DOWN};

	BlackBroblub() : Broblub() {image = BB; setHealth(40);setMaxHealth(40);setAttackStrength(3);setSightRadius(BLACK_ENEMY_SIGHT);}

	//Actions
	void chooseAction();
	void goTowardsGoal();
	
	//Polymorphic functions
	virtual void setNormal();
	virtual void setHurt();
	virtual void setAtk();
	virtual void setSpotted();
	virtual bool isNormal();
	virtual bool isAtk();

protected:
	PatternDirection direction;
};

#endif