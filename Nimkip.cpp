/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file implements the Nimkip classes.  
 Nimkips are meant to operate with significant intelligence, 
 including basic cooperation.
*/ 

#include "Nimkip.h"

using namespace moverNS;

//no longer in use
char getSurroundingChar(Space value)
{
	switch(value)
	{
	case NIMKIP:
		return 'N';
		break;
	case BROBLUB:
		return 'B';
		break;
	case COIN:
		return 'C';
		break;
	case FOOD:
		return 'F';
		break;
	case OBSTACLE:
		return 'O';
		break;
	case GAP:
		return 'G';
		break;
	case EMPTY:
		return 'E';
		break;
	case OVER:
		return 'X';
		break;
	}
}

void Nimkip::checkSurroundings()
{
	visibleTiles = level::getSurroundings(getGridLoc(),getSightRadius());

	//used for outdated methods
	surroundings = level::getSurroundings(getGridLoc());

	for(int i = 0; i < visibleTiles.size(); i++)
	{
		if(visibleTiles[i].type == BROBLUB)
		{
			attackTask = task;
			task = ATTACK;
			secondaryTarget = target;
			target = visibleTiles[i];
			return;
		}
	}
}

GridLoc Nimkip::takeTurn()
{
	if(this->getHealth()>0)
	{
		//getUserInput();//check to see if the use gave them something to do
		checkSurroundings();
		if(task != IDLE)//if they have a task do it
		{
			goTowardsGoal();
		}
		else
		{
			checkOthers();//see if there is something to do
			if(task != IDLE)//do it if there is
			{
				goTowardsGoal();
			}
		}
	}
	else
		this->die();
	return pos;
}

NimkipInfo Nimkip::getInfo()
{
	NimkipInfo info;
	info.needHelp = this->needHelp;
	info.location = this->pos;
	info.goal = this->destination;
	info.target = this->target;
	info.task = this->task;
	return info;
}

GridLoc Nimkip::goTowardsGoal()
{
	switch(this->task)
	{
	case WALK:
		if(pos.x==destination.x && pos.y==destination.y)
		{
			task=secondaryTask;
		}
		else
			move(destination);
		break;
	case LIFT:
		//need to get item pointer sharing working.
		if(!this->getHolding())
		{
			level::transferObject(this,target);
			if(getHeldObject()!=0)
				getHeldObject()->changeCarrierStrength(this->getStrength());
			else
			{

				return GridLoc();
			}
		}
		else
		{//if they can't pick it up then they need help
			needHelp = true;
			this->setAtk();
		}
		//if the combined strength is enough to pick it up say they don't need help and set them to carry it
		if(getHeldObject()->getWeight() <= getHeldObject()->getCarrierStrength())
		{
			this->setCarry();
			needHelp = false;
			task = CARRY;
			getHeldObject()->setVisible(false);
			destination = homeBase;
			auto baseSurroundings = level::getSurroundings(destination,1);
			//sets the destination to an empty tile near their base
			for(int i = 0; i < baseSurroundings.size(); i++)
			{
				if(baseSurroundings[i].type==EMPTY)
				{
					destination = baseSurroundings[i];
					break;
				}
			}
		}
		break;
	case ATTACK:
		attack(target);
		task = attackTask;//puts their task back to normal
		target = secondaryTarget;//puts their target back to normal if they had another target
		break;
	case CARRY:
		this->setCarry();
		//if the object becomes too heavy, ie another nimkip dies then they have to stop moving
		if(getHeldObject()->getCarrierStrength()<getHeldObject()->getWeight())
		{
			task = LIFT;
			needHelp = true;
			this->setNormal();
		}
		else
		{
			move(destination);
			//if they make it home
			if(this->pos.x == destination.x && pos.y == destination.y)
			{
				task = IDLE;
				//perhaps hoard disband
				//dont do it again if it is already done
				if(this->getHeldObject()->getActive())
				{
					setScored(true);
					setScorePoints(getHeldObject()->getPoints());
					this->getHeldObject()->setVisible(false);
					this->getHeldObject()->setActive(false);
					this->setHolding(false);
				}
				this->setHeldObject(0);
			}
		}
		break;
	}
	return GridLoc();
}

void Nimkip::checkOthers()
{
	GridLoc loc;
	auto nimkipTiles = level::getSurroundings(this->getGridLoc(),this->nimkipCommunication);
	for(int i = 0; i < nimkipTiles.size(); i++)
	{
		if(nimkipTiles[i].type  == NIMKIP)
		{
			//if that one needs help then stop checking others
			if(helpNimkip(nimkipTiles[i]))
				return;
		}
	}
}

bool Nimkip::helpNimkip(GridLoc nimkip)
{
	NimkipInfo info = level::getNimkipInfo(nimkip);
	if(info.needHelp)
	{
		this->target = info.target;
		this->task = info.task;
		this->destination = info.goal;
		this->needHelp = info.needHelp;
		return true;
	}
	return false;
}

void Nimkip::die()
{
	this->active = false;
	this->visible = false;
	//reduces the carrier strength
	if(this->getHolding())
		getHeldObject()->changeCarrierStrength(this->getStrength()*-1);
}

void Nimkip::getUserInput()
{
	//will set values for the nimkips
    level::getUserInput();
    //if they have a secondary task make it so they stop before they get there
    if(secondaryTask!=IDLE)
    {
        target = destination;
		if(destination.type!=EMPTY)
		{
			Surroundings temp = level::getSurroundings(destination);
			if(temp.N==EMPTY)
			{
				destination = GridLoc(destination.x,destination.y-1);
				return;
			}
			if(temp.S==EMPTY)
			{
				destination = GridLoc(destination.x,destination.y+1);
				return;
			}
			if(temp.E==EMPTY)
			{
				destination = GridLoc(destination.x+1,destination.y);
				return;
			}
			if(temp.W==EMPTY)
			{
				destination = GridLoc(destination.x-1,destination.y);
				return;
			}
			if(temp.NE==EMPTY)
			{
				destination = GridLoc(destination.x+1,destination.y-1);
				return;
			}
			if(temp.NW==EMPTY)
			{
				destination = GridLoc(destination.x-1,destination.y-1);
				return;
			}
			if(temp.SE==EMPTY)
			{
				destination = GridLoc(destination.x+1,destination.y+1);
				return;
			}
			if(temp.SW==EMPTY)
			{
				destination = GridLoc(destination.x-1,destination.y+1);
				return;
			}
		}
    }
}

void Nimkip::attack(GridLoc& p)
{
	level::runAttack(this, target);
}

void YellowKip::move(GridLoc& p) { 
       GridLoc curPos = getGridLoc();
       VECTOR2 dirVec = VECTOR2((float)(p.x - curPos.x),(float)(p.y - curPos.y));

              surroundings = level::getSurroundings(curPos);
              Surroundings nSur, eSur, wSur, sSur;
          if(surroundings.N != OVER) 
                     nSur = level::getSurroundings(GridLoc(curPos.x,curPos.y - 1));
          else {
                     nSur.E = OVER;
                     nSur.N = OVER;
                     nSur.NE = OVER;
                     nSur.NW = OVER;
                     nSur.S = OVER;
                     nSur.SE = OVER;
                     nSur.SW = OVER;
                     nSur.W = OVER;
          }
          if(surroundings.E != OVER) 
                     eSur = level::getSurroundings(GridLoc(curPos.x + 1,curPos.y));
          else {
                     eSur.E = OVER;
                     eSur.N = OVER;
                     eSur.NE = OVER;
                     eSur.NW = OVER;
                     eSur.S = OVER;
                     eSur.SE = OVER;
                     eSur.SW = OVER;
                     eSur.W = OVER;
          }
          if(surroundings.S != OVER)
                     sSur = level::getSurroundings(GridLoc(curPos.x - 1,curPos.y + 1));
          else {
                     sSur.E = OVER;
                     sSur.N = OVER;
                     sSur.NE = OVER;
                     sSur.NW = OVER;
                     sSur.S = OVER;
                     sSur.SE = OVER;
                     sSur.SW = OVER;
                     sSur.W = OVER;
          }
          if(surroundings.W != OVER)
                     wSur = level::getSurroundings(GridLoc(curPos.x,curPos.y));
          else {
                     wSur.E = OVER;
                     wSur.N = OVER;
                     wSur.NE = OVER;
                     wSur.NW = OVER;
                     wSur.S = OVER;
                     wSur.SE = OVER;
                     wSur.SW = OVER;
                     wSur.W = OVER;
          }

       if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x > 0)  && (dirVec.y > 0)) {//1st quadrant movement
              if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x > 0)  && (dirVec.y > 0)) {
              if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              }
              return;
       } else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x < 0)  && (dirVec.y > 0)) {//2nd quadrant movement
              if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x < 0)  && (dirVec.y > 0)) {
              if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              }
              return;
       } else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x < 0) && (dirVec.y < 0)) {//3rd quadrant movement
              if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x < 0) && (dirVec.y < 0)) {
              if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              }
              return;
       } else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x > 0) && (dirVec.y < 0)) {//4th quadrant movement
              if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x > 0) && (dirVec.y < 0)) {
              if(surroundings.N == 6) {
                     setDir(UP);
              } else if (nSur.N == 6) {
                     setGridLoc(curPos.x, curPos.y - 2);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(eSur.E == 6) {
                     setGridLoc(curPos.x + 2, curPos.y);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(wSur.W == 6) {
                     setGridLoc(curPos.x - 2, curPos.y);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(sSur.S == 6) {
                     setGridLoc(curPos.x, curPos.y + 2);
              }
              return;
       }
}

//working on new movement method
//doesn't work with obstacle correction
//shows UI is working though
//has extra stuff to try to move food objects with it but it doesn't quite do anything
void RedKip::move(GridLoc& p) {  
	GridLoc curPos = getGridLoc();
	VECTOR2 dirVec = VECTOR2((float)(p.x - curPos.x),(float)(p.y - curPos.y));

    surroundings = level::getSurroundings(curPos);

	visibleTiles = level::getSurroundings(curPos,getSightRadius());

	//used to get the direction chosen and move the nimkip and objects they hold
	moverNS::DIR chosenDirection;

	//go north or south
	if(dirVec.x==0)
	{
		//go south if possible
		if(dirVec.y>0)
		{
			if(level::getTileType(GridLoc(curPos.x,curPos.y+1)) == EMPTY)
			{
				chosenDirection=DOWN;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x,curPos.y+1)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x,curPos.y+1));
				setScored(true);
				chosenDirection=DOWN;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
		}
		else if(dirVec.y<0)//go north if possible
		{
			if(level::getTileType(GridLoc(curPos.x,curPos.y-1)) == EMPTY)
			{
				chosenDirection=UP;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x,curPos.y-1)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x,curPos.y-1));
				setScored(true);
				chosenDirection=UP;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
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
				chosenDirection=DOWN_RIGHT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y+1)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x+1,curPos.y+1));
				setScored(true);
				chosenDirection=DOWN_RIGHT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
		}
		else if(dirVec.y<0)//go north east if possible
		{
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y-1)) == EMPTY)
			{
				chosenDirection=UP_RIGHT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y-1)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x+1,curPos.y-1));
				setScored(true);
				chosenDirection=UP_RIGHT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
		}
		else if(dirVec.y==0)//go east
		{
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y)) == EMPTY)
			{
				chosenDirection=RIGHT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x+1,curPos.y)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x+1,curPos.y));
				setScored(true);
				chosenDirection=RIGHT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
		}
	}
	else if(dirVec.x<0)//go west, north west, or south west
	{
		if(dirVec.y>0)//go south west if possible
		{
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y+1)) == EMPTY)
			{
				chosenDirection=DOWN_LEFT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y+1)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x-1,curPos.y+1));
				setScored(true);
				chosenDirection=DOWN_LEFT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
		}
		else if(dirVec.y<0)//go north west if possible
		{
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y-1)) == EMPTY)
			{
				chosenDirection=UP_LEFT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y-1)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x-1,curPos.y-1));
				setScored(true);
				chosenDirection=UP_LEFT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
		}
		else if(dirVec.y==0)//go west
		{
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y)) == EMPTY)
			{
				chosenDirection=LEFT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
			if(level::getTileType(GridLoc(curPos.x-1,curPos.y)) == COIN)
			{
				level::collectCoin(GridLoc(curPos.x-1,curPos.y));
				setScored(true);
				chosenDirection=LEFT;
				setDir(chosenDirection);
				if(getHeldObject())
					getHeldObject()->setDir(chosenDirection);
				return;
			}
		}
	}
}

void BlueKip::move(GridLoc& p) {  
       GridLoc curPos = getGridLoc();
       VECTOR2 dirVec = VECTOR2((float)(p.x - curPos.x),(float)(p.y - curPos.y));

       surroundings = level::getSurroundings(curPos);

       if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x > 0)  && (dirVec.y > 0)) {//1st quadrant movement
              if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x > 0)  && (dirVec.y > 0)) {
              if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              }
              return;
       } else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x < 0)  && (dirVec.y > 0)) {//2nd quadrant movement
              if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x < 0)  && (dirVec.y > 0)) {
              if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              }
              return;
       } else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x < 0) && (dirVec.y < 0)) {//3rd quadrant movement
              if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x < 0) && (dirVec.y < 0)) {
              if(surroundings.N == 6) {
                     setDir(UP);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              }
              return;
       } else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x > 0) && (dirVec.y < 0)) {//4th quadrant movement
              if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(surroundings.N == 6) {
                     setDir(UP);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              }
              return;
       } else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x > 0) && (dirVec.y < 0)) {
              if(surroundings.N == 6) {
                     setDir(UP);
              } else if(surroundings.E == 6) {
                     setDir(RIGHT);
              } else if(surroundings.W == 6) {
                     setDir(LEFT);
              } else if(surroundings.S == 6) {
                     setDir(DOWN);
              }
              return;
       }
}

void RedKip::setAtk(){ setCurrentFrame(RNIM_ATK); } 
void RedKip::setNormal(){ setCurrentFrame(RNIM); }
void RedKip::setHurt(){ setCurrentFrame(RNIM_HURT); }

bool RedKip::isNormal(){return (getCurrentFrame()==RNIM);}
bool RedKip::isAtk(){return (getCurrentFrame()==RNIM_ATK);}

void BlueKip::setAtk(){ setCurrentFrame(BNIM_ATK); } 
void BlueKip::setNormal(){ setCurrentFrame(BNIM); }
void BlueKip::setHurt(){ setCurrentFrame(BNIM_HURT); }

bool BlueKip::isNormal(){return (getCurrentFrame()==BNIM);}
bool BlueKip::isAtk(){return (getCurrentFrame()==BNIM_ATK);}

void YellowKip::setAtk(){ setCurrentFrame(YNIM_ATK); } 
void YellowKip::setNormal(){ setCurrentFrame(YNIM); }
void YellowKip::setHurt(){ setCurrentFrame(YNIM_HURT); }

bool YellowKip::isNormal(){return (getCurrentFrame()==YNIM);}
bool YellowKip::isAtk(){return (getCurrentFrame()==YNIM_ATK);}