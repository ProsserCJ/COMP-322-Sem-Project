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
	level::runAttack(this, enemy); 
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

    obstacleAvoidanceSurroundings = level::getSurroundings(curPos);

	visibleTiles = level::getSurroundings(curPos,getSightRadius());

	//used to get the direction chosen and move the nimkip and objects they hold
	moverNS::DIR chosenDirection;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//new Move function using ternary direction choices and obstacle avoidance
	
	int dirVal;
	int a,b,c,d;
	
	if(dirVec.x < 0)
		a = -1;
	else if(dirVec.x > 0)
		a = 1;
	else
		a = 0;

	if(-dirVec.y < 0)
		b = -1;
	else if(-dirVec.y > 0)
		b = 1;
	else
		b = 0;
	
	c = a;
	if(c < 0) c = 2;
	d = b;
	if(d < 0) d = 2;

	dirVal = 3*c + d;   //Convert ternary value to decimal

	switch(dirVal) {    //Reassigning to match enum direction values
	case 0: dirVal = 8; break;
	case 1: dirVal = 2; break;
	case 2: dirVal = 3; break;
	case 3: dirVal = 1; break;
	case 4: dirVal = 5; break;
	case 5: dirVal = 7; break;
	case 6: dirVal = 0; break;
	case 7: dirVal = 4; break;
	case 8: dirVal = 6; break;
	}

	if(a == 0 && b == 0)
		return;

	chosenDirection = (moverNS::DIR) dirVal;

	bool yes = true;

	if(yes && dirVal!=this->getPrevDir() && level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == EMPTY) {
		this->setPrevDir(-1);//set their previous direction to nothin useful
		//make all directions available
		for(int i = 0; i < 8; i++)
		{
			availableDirections[i]=true;
		}
		if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
			level::collectCoin(GridLoc(curPos.x + a,curPos.y - b));
			setScored(true);
		}
		setDir(chosenDirection);
		if(getHeldObject())
			getHeldObject()->setDir(chosenDirection);
		return;
	}
	else
	{
		availableDirections[dirVal]=false;
		this->setPrevDir(dirVal);
		switch(chosenDirection)
		{
		case LEFT:
			if(availableDirections[UP_LEFT] && obstacleAvoidanceSurroundings.NW==EMPTY)
				setDir(UP_LEFT);
			else if(availableDirections[DOWN_LEFT] &&obstacleAvoidanceSurroundings.SW==EMPTY)
				setDir(DOWN_LEFT);
			else if(availableDirections[UP] &&obstacleAvoidanceSurroundings.N==EMPTY)
				setDir(UP);
			else if(availableDirections[DOWN] &&obstacleAvoidanceSurroundings.S==EMPTY)
				setDir(DOWN);
		case RIGHT:
			if(availableDirections[UP_RIGHT] &&obstacleAvoidanceSurroundings.NE==EMPTY)
				setDir(UP_RIGHT);
			else if(availableDirections[DOWN_RIGHT] &&obstacleAvoidanceSurroundings.SE==EMPTY)
				setDir(DOWN_RIGHT);
			else if(availableDirections[UP] &&obstacleAvoidanceSurroundings.N==EMPTY)
				setDir(UP);
			else if(availableDirections[DOWN] &&obstacleAvoidanceSurroundings.S==EMPTY)
				setDir(DOWN);
		case UP:
			if(availableDirections[UP_LEFT] &&obstacleAvoidanceSurroundings.NW==EMPTY)
				setDir(UP_LEFT);
			else if(availableDirections[UP_RIGHT] && obstacleAvoidanceSurroundings.NE==EMPTY)
				setDir(UP_RIGHT);
			else if(availableDirections[RIGHT] &&obstacleAvoidanceSurroundings.E==EMPTY)
				setDir(RIGHT);
			else if(availableDirections[LEFT] &&obstacleAvoidanceSurroundings.W==EMPTY)
				setDir(LEFT);
		case DOWN:
			if(availableDirections[DOWN_LEFT] &&obstacleAvoidanceSurroundings.SW==EMPTY)
				setDir(DOWN_LEFT);
			else if(availableDirections[DOWN_RIGHT] &&obstacleAvoidanceSurroundings.SE==EMPTY)
				setDir(DOWN_RIGHT);
			else if(availableDirections[RIGHT] &&obstacleAvoidanceSurroundings.E==EMPTY)
				setDir(RIGHT);
			else if(availableDirections[LEFT] &&obstacleAvoidanceSurroundings.W==EMPTY)
				setDir(LEFT);
		case UP_RIGHT:
			if(availableDirections[UP] &&obstacleAvoidanceSurroundings.N==EMPTY)
				setDir(UP);
			else if(availableDirections[RIGHT] &&obstacleAvoidanceSurroundings.E==EMPTY)
				setDir(RIGHT);
			else if(availableDirections[UP_LEFT] &&obstacleAvoidanceSurroundings.NW==EMPTY)
				setDir(UP_LEFT);
			else if(availableDirections[DOWN_RIGHT] &&obstacleAvoidanceSurroundings.SE==EMPTY)
				setDir(DOWN_RIGHT);
		case DOWN_RIGHT:
			if(availableDirections[DOWN] &&obstacleAvoidanceSurroundings.S==EMPTY)
				setDir(DOWN);
			else if(availableDirections[RIGHT] &&obstacleAvoidanceSurroundings.E==EMPTY)
				setDir(RIGHT);
			else if(availableDirections[DOWN_LEFT] &&obstacleAvoidanceSurroundings.SW==EMPTY)
				setDir(DOWN_LEFT);
			else if(availableDirections[UP_RIGHT] &&obstacleAvoidanceSurroundings.NE==EMPTY)
				setDir(UP_RIGHT);
		case UP_LEFT:
			if(availableDirections[UP] &&obstacleAvoidanceSurroundings.N==EMPTY)
				setDir(UP);
			else if(availableDirections[LEFT] &&obstacleAvoidanceSurroundings.W==EMPTY)
				setDir(LEFT);
			else if(availableDirections[UP_RIGHT] &&obstacleAvoidanceSurroundings.NE==EMPTY)
				setDir(UP_RIGHT);
			else if(availableDirections[DOWN_LEFT] &&obstacleAvoidanceSurroundings.SW==EMPTY)
				setDir(DOWN_LEFT);
		case DOWN_LEFT:
			if(availableDirections[DOWN] &&obstacleAvoidanceSurroundings.S==EMPTY)
				setDir(DOWN);
			else if(availableDirections[LEFT] &&obstacleAvoidanceSurroundings.W==EMPTY)
				setDir(LEFT);
			else if(availableDirections[DOWN_RIGHT] &&obstacleAvoidanceSurroundings.SE==EMPTY)
				setDir(DOWN_RIGHT);
			else if(availableDirections[UP_LEFT] &&obstacleAvoidanceSurroundings.NW==EMPTY)
				setDir(UP_LEFT);
		}
	}
}

void Broblub::move(int x, int y){
	move(GridLoc(x,y));
}

GridLoc Broblub::takeTurn(){

	if(this->getHealth()>0)
	{
		
		chooseAction(); //either sets to attack nimkip, set to move toward a close nimkip, or set to move in pattern
				
		goTowardsGoal(); //attack or move toward a nimkip or in a pattern
			
	}
	else
	{
		die();
	}
	
	return pos;
}

void BlackBroblub::goTowardsGoal()
{
	switch(this->task)
	{
		//right now, the pattern is a counter-clockwise circle, which breaks as soon as a nimkip is in their range of vision
		//I will try to implement different patterns that can be set in the constructor
	case MOVE: 
		if(pos.x==destination.x && pos.y==destination.y)
		{
			if(direction==UP) {
				direction=LEFT;
				if(pos.x-3>0)
					destination = GridLoc(pos.x-3,pos.y);
			}
			else if(direction==LEFT) {
				direction=DOWN;
				if(pos.y+3<level::getGridHeight())
					destination = GridLoc(pos.x,pos.y+3);
			}
			else if(direction==DOWN) {
				direction=RIGHT;
				if(pos.x+3<level::getGridWidth())
					destination = GridLoc(pos.x+3,pos.y);
			}
			else if(direction==RIGHT) {
				direction=UP;
				if(pos.y-3>0)
					destination = GridLoc(pos.x,pos.y-3);
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
	//return GridLoc();
}

void RedBroblub::goTowardsGoal()
{
	switch(this->task)
	{
	case MOVE: 
		moveRandom(surroundings);
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
	//return GridLoc();
}

void BlackBroblub::chooseAction()
{
	visibleTiles = level::getSurroundings(pos,getSightRadius());

	//here?
	surroundings = level::getSurroundings(pos,1);
	//^^^^^^^
	int stuff=9;
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
			setSpotted();
			return;
		}
	}
	
	//if no nimkips in view and not already moving in a pattern, set task as pattern
	if(task != MOVE) 
	{
		task = MOVE;
		direction = UP;
		destination = GridLoc(pos.x,pos.y-2);
		setNormal();
	}
}

void RedBroblub::chooseAction()
{
	visibleTiles = level::getSurroundings(pos,getSightRadius());

	//here?
	surroundings = level::getSurroundings(pos,1);
	//^^^^^^^
	int stuff=9;
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
			setSpotted();
			return;
		}
	}
	
	//if no nimkips in view and not already moving in a pattern, set task as pattern
	if(task != MOVE) 
	{
		task = MOVE;
		setNormal();
	}
}

//specific to the sight radius of 1
//if other enemies were made with a larger radius the movement would have to be altered
void RedBroblub::moveRandom(vector<GridLoc> temp) 
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
		//this->chooseDir(loc,temp[r]);
		destination = temp[r];
	}
}

void Broblub::die()
{
	setActive(false);
	setVisible(false);
}


//Update these as well
void RedBroblub::setAtk(){ setCurrentFrame(RBRO_ATK); } 
void RedBroblub::setNormal(){ setCurrentFrame(RBRO); }
void RedBroblub::setHurt(){ setCurrentFrame(RBRO_HURT); }
void RedBroblub::setSpotted(){ setCurrentFrame(RBRO_SPOT); }

bool RedBroblub::isNormal(){return (getCurrentFrame()==RBRO);}
bool RedBroblub::isAtk(){return (getCurrentFrame()==RBRO_ATK);}

void BlackBroblub::setAtk(){ setCurrentFrame(BBRO_ATK); } 
void BlackBroblub::setNormal(){ setCurrentFrame(BBRO); }
void BlackBroblub::setHurt(){ setCurrentFrame(BBRO_HURT); }
void BlackBroblub::setSpotted(){ setCurrentFrame(BBRO_SPOT); }

bool BlackBroblub::isNormal(){return (getCurrentFrame()==BBRO);}
bool BlackBroblub::isAtk(){return (getCurrentFrame()==BBRO_ATK);}