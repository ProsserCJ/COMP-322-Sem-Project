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

void Broblub::move(GridLoc& pos){
	this->setGridLoc(pos);
}

void Broblub::move(int x, int y){
	this->setGridLoc(x,y);
}

GridLoc Broblub::takeTurn(){
	//check surroundings
	//if there's a nimkip, attack
	if(this->getHealth()>0)
	{
		auto temp = level::getSurroundings(pos,getSightRadius());
	
		for(int i = 0; i < temp.size(); i++)
		{
			if(temp[i].type==NIMKIP)
			{
				level::runAttack(this,temp[i]);
				return pos;
			}
		}
		
		moveRandom(temp);
	}
	else
	{
		die();
	}
	
	return pos;
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

bool Broblub::isNormal(){return (getCurrentFrame()==BRO);}
bool Broblub::isAtk(){return (getCurrentFrame()==BRO_ATK);}