/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This defines a few structures to facilidate passing data
*/ 

#ifndef _IMPORTANTTYPES_H
#define _IMPORTANTTYPES_H

enum Space {NIMKIP,BROBLUB,COIN,FOOD,OBSTACLE,GAP,EMPTY,OVER,BASE};

struct GridLoc{
	int x, y;
	GridLoc(int X=0, int Y=0){ x = X; y = Y; }
};

struct Surroundings
{
	Space N,S,E,W,NE,NW,SE,SW;
};

//lift is when they are trying but failing to pick up something
//carry is when they are moving something from one place to another
//idle is if they are without a task
enum Task {WALK,LIFT,ATTACK,CARRY,IDLE};

struct NimkipInfo
{
	bool needHelp;
	GridLoc location;
	Task task;
	GridLoc goal;
};

#endif