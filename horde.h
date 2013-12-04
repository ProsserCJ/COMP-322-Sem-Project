#ifndef __HORDE_H
#define __HORDE_H

#include "level.h"
#include "carriable.h"
#include <vector>
using std::vector;
using namespace moverNS;

class Horde : public Lifeform {//Acts as a group, taking turns and moving all Nimkips within the group
public:
	Horde(Carriable* c, Lifeform* l);
	
	GridLoc takeTurn();

	void move(GridLoc& p);
	void checkSurroundings();
	void addMember(Lifeform* l);
	void dropMember(int i);
	void disband();
	void subHealth(int diff);

protected:
	void moveNormal(GridLoc& p);
	void moveYellow(GridLoc& p);
	void findEmptyAndDrop(Surroundings sur);
	vector<Lifeform*> hordeList;
	int hordeNum;
	int totalStrength;
	Carriable * hordeObject;
	Surroundings surroundings;
	vector<GridLoc> visibleTiles;

	GridLoc homeBase;
	GridLoc destination;
};

#endif