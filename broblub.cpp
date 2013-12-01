/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file implements the Broblub class,
 the enemy creature displayed on the screen.  Broblubs
 operate with moderate intelligence, almost always
 choosing to attack the closest Nimkip
*/ 


#include "Broblub.h"

using namespace moverNS;

void Broblub::attack(GridLoc& enemy){
	level::runAttack(this, pos); 
}

void Broblub::attack(int x, int y){
	level::runAttack(this, GridLoc(x,y));
}

//void Broblub::move(GridLoc& pos){
//	this->setGridLoc(pos);
//}

void Broblub::move(GridLoc& p) {  
	GridLoc curPos = getGridLoc();
	VECTOR2 dirVec = VECTOR2((float)(p.x - curPos.x),(float)(p.y - curPos.y));

    surroundings = level::getSurroundings(curPos,1);

	visibleTiles = level::getSurroundings(curPos,getSightRadius());

	//go north or south
	if(dirVec.x==0)
	{
		//go south if possible
		if(dirVec.y>0)
		{
			if(level::getTileType(GridLoc(curPos.x,curPos.y+1)) == EMPTY)
			{
				setDir(moverNS::DOWN);
				return;
			}
		}
		else if(dirVec.y<0)//go north if possible
		{
			if(level::getTileType(GridLoc(curPos.x,curPos.y-1)) == EMPTY)
			{
				setDir(moverNS::UP);
				return;
			}
		}
	}
	else if(dirVec.x>0)//go east, north east, or south east
	{
		if(dirVec.y>0)//go south east if possible
		{
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y+1)) == EMPTY)
			{
				setDir(DOWN_RIGHT);
				return;
			}
		}
		else if(dirVec.y<0)//go north east if possible
		{
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y-1)) == EMPTY)
			{
				setDir(UP_RIGHT);
				return;
			}
		}
		else if(dirVec.y==0)//go east
		{
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y)) == EMPTY)
			{
				setDir(moverNS::RIGHT);
				return;
			}
		}
	}
	else if(dirVec.x<0)//go west, norht west, or south west
	{
		if(dirVec.y>0)//go south west if possible
		{
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y+1)) == EMPTY)
			{
				setDir(DOWN_LEFT);
				return;
			}
		}
		else if(dirVec.y<0)//go north west if possible
		{
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y-1)) == EMPTY)
			{
				setDir(UP_LEFT);
				return;
			}
		}
		else if(dirVec.y==0)//go west
		{
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y)) == EMPTY)
			{
				setDir(moverNS::LEFT);
				return;
			}
		}
	}
	}

void Broblub::move(int x, int y){
	this->setGridLoc(x,y);
}

GridLoc Broblub::takeTurn(){

	if(this->getHealth()>0)
	{
		
		chooseAction(); //either sets to attack nimkip, set to move toward a close nimkip, or set to move in pattern
				
		//moveRandom(temp);

		goTowardsGoal(); //attack or move toward a nimkip or in a pattern
			
	}
	else
	{
		die();
	}
	
	return pos;
}

GridLoc Broblub::goTowardsGoal()
{
	switch(this->task)
	{
		//right now, the pattern is a counter-clockwise circle, which breaks as soon as a nimkip is in their range of vision
		//I will try to implement different patterns that can be set in the constructor
	case PATTERNMOVEMENT: 
		if(pos.x==destination.x && pos.y==destination.y)
		{
			if(direction==UP) {
				direction=LEFT;
				destination = GridLoc(pos.x-2,pos.y);
			}
			else if(direction==LEFT) {
				direction=DOWN;
				destination = GridLoc(pos.x,pos.y+2);
			}
			else if(direction==DOWN) {
				direction=RIGHT;
				destination = GridLoc(pos.x+2,pos.y);
			}
			else if(direction==RIGHT) {
				direction=UP;
				destination = GridLoc(pos.x,pos.y-2);
			}
		}
		move(destination); //requires working move function
	case WALK:
		if(pos.x==destination.x && pos.y==destination.y)
		{
			//I don't think this should ever matter
		}
		else
			move(destination);
		break;
	case ATTACK:
		attack(target);
		break;
	}
	return GridLoc();
}

void Broblub::chooseAction()
{
	visibleTiles = level::getSurroundings(pos,getSightRadius());

	surroundings = level::getSurroundings(pos,1);
	
	for(int i = 0; i < surroundings.size(); i++)
	{
		if(surroundings[i].type==NIMKIP)
		{
			task = ATTACK;
			target = surroundings[i];
			return;
		}
	}

	//if nimkip in view, set it as destination
	for(int i = 0; i < visibleTiles.size(); i++)
	{
		if(visibleTiles[i].type==NIMKIP)
		{
			task = WALK;
			destination = visibleTiles[i];
			//setSpotted();
			return;
		}
	}
	
	//if no nimkips in view and not already moving in a pattern, set task as pattern
	if(task != PATTERNMOVEMENT) 
	{
		task = PATTERNMOVEMENT;
		direction = UP;
		destination = GridLoc(pos.x,pos.y-2);
		setNormal();
	}
}

//specific to the sight radius of 1
//if other enemies were made with a larger radius the movement would have to be altered
void Broblub::moveRandom(vector<GridLoc> temp) 
{
	int r;
	GridLoc loc = pos;
	r = rand()%temp.size();  //rgen?

	//choosing the direction is specific to the sightRadius of 1
	//this makes them automattically move to a space
	//should be modified so that the choice merely chooses where to go and then they have a move function to get to it
	//similar to the nimkip's movement
	if(temp[r].type==EMPTY)
	{
		this->chooseDir(loc,temp[r]);
	}
}

void Broblub::die()
{
	setActive(false);
	setVisible(false);
}

void Broblub::setAtk(){ setCurrentFrame(BRO_ATK); } 
void Broblub::setNormal(){ setCurrentFrame(BRO); }
void Broblub::setHurt(){ setCurrentFrame(BRO_HURT); }
//void Broblub::setSpotted(){ setCurrentFrame(BRO_SPOT); }

bool Broblub::isNormal(){return (getCurrentFrame()==BRO);}
bool Broblub::isAtk(){return (getCurrentFrame()==BRO_ATK);}