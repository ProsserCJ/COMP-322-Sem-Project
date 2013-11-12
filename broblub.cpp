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
		Surroundings temp;
		GridLoc loc;
		temp = level::getSurroundings(pos);
	
		if(temp.E == NIMKIP) {
			loc.x = getGridLoc().x + 1;
			loc.y = getGridLoc().y;
			level::runAttack(this, loc);
		}
		else if(temp.NE == NIMKIP) {
			loc.x = getGridLoc().x + 1;
			loc.y = getGridLoc().y - 1;
			level::runAttack(this, loc);
		}
		else if(temp.N == NIMKIP) {
			loc.x = getGridLoc().x;
			loc.y = getGridLoc().y - 1;
			level::runAttack(this, loc);
		}
		else if(temp.NW == NIMKIP) {
			loc.x = getGridLoc().x - 1;
			loc.y = getGridLoc().y - 1;
			level::runAttack(this, loc);
		}
		else if(temp.W == NIMKIP) {
			loc.x = getGridLoc().x - 1;
			loc.y = getGridLoc().y;
			level::runAttack(this, loc);
		}
		else if(temp.SW == NIMKIP) {
			loc.x = getGridLoc().x - 1;
			loc.y = getGridLoc().y + 1;
			level::runAttack(this, loc);
		}
		else if(temp.S == NIMKIP) {
			loc.x = getGridLoc().x;
			loc.y = getGridLoc().y + 1;
			level::runAttack(this, loc);
		}
		else if(temp.SE == NIMKIP) {
			loc.x = getGridLoc().x + 1;
			loc.y = getGridLoc().y + 1;
			level::runAttack(this, loc);
		}
		else moveRandom(temp);
	}
	else
	{
		die();
	}
	
	return pos;
}

void Broblub::moveRandom(Surroundings temp) 
{
	int r;
	GridLoc loc;
	r = rand()%8;  //rgen?

	if(r == 1 && temp.N == EMPTY) {
			/*loc.x = getGridLoc().x;
			loc.y = getGridLoc().y - 1;
			move(loc);*/
			setDir(UP);
	}
	else if(r == 2 && temp.NW == EMPTY) {
			/*loc.x = getGridLoc().x - 1;
			loc.y = getGridLoc().y - 1;
			move(loc); */
			setDir(UP_LEFT);
	}
	else if(r == 3 && temp.NE == EMPTY) {
			/*loc.x = getGridLoc().x + 1;
			loc.y = getGridLoc().y - 1;
			move(loc); */ 
			setDir(UP_RIGHT);
	}
	else if(r == 4 && temp.W == EMPTY) {
			/*loc.x = getGridLoc().x - 1;
			loc.y = getGridLoc().y;
			move(loc);*/
			setDir(LEFT);
	}
	else if(r == 5 && temp.E == EMPTY) {
			/*loc.x = getGridLoc().x + 1;
			loc.y = getGridLoc().y;
			move(loc);  */
			setDir(RIGHT);
	}
	else if(r == 6 && temp.S == EMPTY) {
		/*	loc.x = getGridLoc().x;
			loc.y = getGridLoc().y + 1;
			move(loc);  */
			setDir(DOWN);
	}
	else if(r == 7 && temp.SW == EMPTY) {
			/*loc.x = getGridLoc().x - 1;
			loc.y = getGridLoc().y + 1;
			move(loc); */ 
			setDir(DOWN_LEFT);
	}
	else if(r == 0 && temp.SE == EMPTY) {
		/*	loc.x = getGridLoc().x + 1;
			loc.y = getGridLoc().y + 1;
			move(loc);  */
			setDir(DOWN_RIGHT);
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