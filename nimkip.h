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

class Nimkip : public Lifeform
{
public:
	Nimkip() { target = GridLoc(); task = IDLE; homeBase = GridLoc(0,1);}
	GridLoc takeTurn();
	virtual void move(GridLoc& p) {}	
	virtual void move(int x, int y) {}	
	void attack(GridLoc& p);
	void checkSurroundings();
	//returns a struct of their status info like needing help and their destination and task
	NimkipInfo getInfo();
	//has the nimkip try to complete their task
	GridLoc goTowardsGoal();
	void setPTask(Task t) {task = t;}
	void setSTask(Task t) {secondaryTask = t;}
	void setDestination(GridLoc g) {destination = g;}

private:
	//checks its surroundings to see if there is a nimkip around it
	void checkOthers();
	//sets its goals and destination to a nimkip that needs help
	//returns true if they find one that needs help
	bool helpNimkip(GridLoc nimkip);
	//gets input from user if they do not have a task
	void die();

	void getUserInput();

protected:
	Surroundings surroundings;
	bool needHelp;//determines if they need help with something
	Task task;
	Task secondaryTask;//what to do after they get there
	Task attackTask;//only when attacking
	//position of what they are picking up or attacking
	GridLoc target;
	GridLoc destination;
	//the location it should take things it picks up
	GridLoc homeBase;

};

class RedKip : public Nimkip
{
public:
	RedKip() : Nimkip() {image = RN; this->setHealth(15);this->setAttackStrength(10); this->setStrength(5);}
	void move(GridLoc& p);
	void move(int x, int y) {move(GridLoc(x,y));}
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
	void move(GridLoc& p);
	void move(int x, int y) {move(GridLoc(x,y));}
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
	void move(GridLoc& p);
	void move(int x, int y) {move(GridLoc(x,y));}
	virtual void setNormal();
	virtual void setHurt();
	virtual void setAtk();
	virtual bool isNormal();
	virtual bool isAtk();
};

#endif