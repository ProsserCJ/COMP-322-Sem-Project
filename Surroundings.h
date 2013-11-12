#ifndef _SURROUNDINGS_H
#define _SURROUNDINGS_H

//this is a type that defines what is in a given space
//current options are a nimkip, an enemy, food or coin, obstacle or gap, and EMPTY
enum Space {NIMKIP,BROBLUB,COIN,FOOD,OBSTACLE,GAP,EMPTY,OVER};


struct Surroundings
{
	Space N,S,E,W,NE,NW,SE,SW;
};

//lift is when they are trying but failing to pick up something
//carry is when they are moving something from one place to another
//idle is if they are without a task
enum Task {WALK,LIFT,ATTACK,CARRY,IDLE};

struct Position{
	int x, y;
	Position(int X=0, int Y=0){ x = X; y = Y; }
};

struct NimkipInfo
{
	bool needHelp;
	Position location;
	Task task;
	Position goal;
};


#endif