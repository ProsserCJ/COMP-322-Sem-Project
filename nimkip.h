/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines the base Nimkip class, an extension of Lifeform, and
 several basic functions, as well as classes for all derivations of Nimkips.  Nimkips
 are capable of movement and attack.
*/ 

#ifndef _NIMKIP_H
#define _NIMKIP_H

#include "Lifeform.h"
#include "level.h"
#include <vector>
using std::vector;

class Nimkip : public Lifeform
{
public:
	Nimkip(): target(GridLoc()), task(IDLE), homeBase(GridLoc(0,0)), secondaryTarget(GridLoc()), nimkipCommunication(2), needHelp(false) {}

	//Getters
	//returns a struct of their status info like needing help and their destination and task
	NimkipInfo getInfo();

	//Setters
	void setPTask(Task t)			{task = t;}
	void setSTask(Task t)			{secondaryTask = t;}
	void setDestination(GridLoc g)	{destination = g;}
	void setTarget(GridLoc t)		{target = t;}

	//Polymorphic functions
	virtual void move(GridLoc& p)	{}	
	virtual void move(int x, int y) {}	

	//Other Actions
	void attack(GridLoc& p);
	void checkSurroundings();
	GridLoc takeTurn();
	GridLoc goTowardsGoal();
	void setCarry()					{setCurrentFrame(NIMCARRY);}

private:

	void checkOthers();	
	bool helpNimkip(GridLoc nimkip);	
	void die();
	void getUserInput();

protected:
	vector<GridLoc> visibleTiles;
	Surroundings surroundings;
	bool needHelp;//determines if they need help with something
	Task task;
	Task secondaryTask;//what to do after they get there
	Task attackTask;//only when attacking
	//position of what they are picking up or attacking
	GridLoc target;
	//target value to hold target when switching from picking something up to fighting and vice versa
	GridLoc secondaryTarget;
	GridLoc destination;
	//the location it should take things it picks up
	GridLoc homeBase;
	int nimkipCommunication;

};

class RedKip : public Nimkip
{
public:
	RedKip() : Nimkip() {image = RN; this->setHealth(15);this->setAttackStrength(10); this->setStrength(5);}

	//Actions
	void move(int x, int y)			{move(GridLoc(x,y));}
	void move(GridLoc& p);
	
	//Polymorphic functions
	virtual void setNormal();
	virtual void setHurt();
	virtual void setAtk();
	virtual bool isNormal();
	virtual bool isAtk();
};

class YellowKip : public Nimkip
{
public:
	YellowKip() : Nimkip() {image = YN; this->setHealth(10);this->setAttackStrength(5);this->setStrength(5);}
	
	//Actions
	void move(int x, int y)			{move(GridLoc(x,y));}
	void move(GridLoc& p);
	
	//Polymorphic functions
	virtual void setNormal();
	virtual void setHurt();
	virtual void setAtk();
	virtual bool isNormal();
	virtual bool isAtk();
};

class BlueKip : public Nimkip
{
public:
	BlueKip() : Nimkip() {image = BN; this->setHealth(10); this->setAttackStrength(5); this->setStrength(10);}
	
	//Actions
	void move(int x, int y)			{move(GridLoc(x,y));}
	void move(GridLoc& p);
	
	//Polymorphic functions
	virtual void setNormal();
	virtual void setHurt();
	virtual void setAtk();
	virtual bool isNormal();
	virtual bool isAtk();
};

#endif