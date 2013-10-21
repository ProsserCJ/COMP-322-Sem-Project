/*
	Chris Prosser
	COMP 322 - Semester Project
	carriable.h defines several carriable objects
*/

#ifndef __CARRIABLE_H
#define __CARRIABLE_H

#include "mover.h"

class Carriable: public Mover{
public:	
	int getWeight() { return weight; }

private:
	int weight;
};

class Coin: public Carriable{
public:
	
};

class Food: public Carriable{
public:
	virtual void addMove(){return;};
};

class Obstacle: public Mover{
public:
	virtual void addMove(){return;};
};

#endif
