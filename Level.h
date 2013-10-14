/*
	comp 322 semester project
	Level header file
	Contains declarations for level class

	functionality:
	-creates all necessary objects and lifeforms
	-calls the take turn function for all lifeforms
	-allows for the transfer of an object from one thing to another (untested as of now)
	-does attacking between objects
		-done by using static functions
		-Lifeforms can call functions to get their surroundings
		-also call function that takes themselves and the position of the thing they are attacking and reduces damage accordingly
	-draws the current map
*/

#ifndef _LEVEL_H
#define _LEVEL_H

#include <iostream>
using std::cout;

#include "Object.h"
//nimkip and broblub are just stub files meant for testing
#include "Nimkip.h"
#include "Broblub.h"
#include <vector>
#include <cstdlib>
#include <ctime>

using std::rand;
using std::srand;
using std::time;

//need to work out user input functionality

using std::vector;

//this is a type that defines what is in a given space
//current options are a nimkip, an enemy, food or coin, obstacle or gap, and EMPTY
enum Space {NIMKIP,BROBLUB,COIN,FOOD,OBSTACLE,GAP,EMPTY};

struct Surroundings
{
	Space N,S,E,W,NE,NW,SE,SW;
};

class Level
{
public:
	Level();
	Level(int numEnemies, int numKips, int maxKips, int sizeX, int sizeY);
	~Level();
	void runTimeStep();
	//returns false if transfer fails
	//takes a lifeForm object pointer and the index of the object in the vector
	//specifically for giving a lifeform something held by the level
	bool transferObject(Lifeform* receiver, Position item);
	void drawMap();
	//takes pointers to the lifeforms involved in the attack, it uses only the position of the target
	static void runAttack(Lifeform* attacker, Position target);
	//lifeForm gives its position then it receives the squares around it
	static Surroundings getSurroundings(Position currentLocation);

private:
	//functions called only by constructor
	void createEmptyLevel();
	void fillLevel();
	//returns a pointer to a lifeForm based off of its location
	static Lifeform* identifyLifeForm(Position pos);
	static Object* identifyObject(Position pos);

	int numEnemies, startKips, maxKips, mapSizeX, mapSizeY;
	static vector<vector<Space>> levelMap;
	static vector<Object*> objects;
	static vector<Lifeform*> lifeForms;
};

#endif
