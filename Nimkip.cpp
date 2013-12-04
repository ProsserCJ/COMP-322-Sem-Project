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
	case CHICK:
		return 'CH';
		break;
	case CANDY:
		return 'CC';
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
	case STATUE:
		return 'S';
		break;
	}
}

void Nimkip::checkSurroundings()
{
	visibleTiles = level::getSurroundings(getGridLoc(),getSightRadius());

	//used for outdated methods
	surroundings = level::getSurroundings(getGridLoc());

	if(!this->getHurtBoolean())
	{
		for(int i = 0; i < visibleTiles.size(); i++)
		{
			//only set to attack if they aren't already attacking
			if(visibleTiles[i].type == BROBLUB && task != ATTACK)
			{
				attackTask = task;
				task = ATTACK;
				//saving old target is necessary in the event that they are switching from picking up something to fighting.
				secondaryTarget = target;
				target = visibleTiles[i];
				return;
			}

		}
	}
}

void Nimkip::heal()
{
	for(int i = 0; i < visibleTiles.size(); i++)
	{
		if(visibleTiles[i].type == STATUE)
		{
			//adds half of their health's distance from max
			this->setHealth(getHealth()+(NIMKIP_MAXHEALTH-getHealth())/2);
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
		heal();
		if(task != IDLE)//if they have a task do it
		{
			goTowardsGoal();
		}
		else
		{
			if(needHelp)
				needHelp = false;
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
	info.sTask = this->secondaryTask;
	return info;
}

GridLoc Nimkip::goTowardsGoal()
{
	switch(this->task)
	{
	case WALK:
		if(needHelp)
			needHelp = false;
		if(pos.x==destination.x && pos.y==destination.y)
		{
			task=secondaryTask;
		}
		else
			move(destination);
		//check if anyone needs help
		//only stop for fights if they have another assigned task
		if(!this->getHurtBoolean())
		{
			if(secondaryTask != IDLE)
			{
				checkOthers(true);
			}
			else
				checkOthers();
		}
		break;
	case LIFT:
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
			//different hoard images
			/*if(getHeldObject()->getType()==Carriable::COI)
				this->setCarryCoin();*/
			if(getHeldObject()->getType()==Carriable::CAND)
				this->setCarryCandy();
			if(getHeldObject()->getType()==Carriable::CHICK)
				this->setCarryFood();
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
		needHelp = true;
		if(attack(target))//if the enemy is dead then they can go back to their old goal
		{
			task = attackTask;//puts their task back to normal
			target = secondaryTarget;//puts their target back to normal if they had another target
			needHelp = false;
			if(helping)
			{
				helping = false;
				destination = this->helpDestination;
			}
		}
		if(this->getHurtBoolean())
		{
			secondaryTask = ATTACK;
			task = WALK;
			auto baseSurroundings = level::getSurroundings(homeBase,3);
			for(int i = 0; i < baseSurroundings.size(); i++)
			{
				if(baseSurroundings[i].type==EMPTY)
				{
					destination = baseSurroundings[i];
					break;
				}
				else
					destination = homeBase;
			}
		}

		break;
	case CARRY:
		//different hoard images
		/*if(getHeldObject()->getType()==Carriable::COI)
			this->setCarryCoin();*/
		if(getHeldObject()->getType()==Carriable::CAND)
			this->setCarryCandy();
		if(getHeldObject()->getType()==Carriable::CHICK)
			this->setCarryFood();
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

void Nimkip::checkOthers(bool fightsOnly)
{
	GridLoc loc;
	auto nimkipTiles = level::getSurroundings(this->getGridLoc(),this->nimkipCommunication);
	for(int i = 0; i < nimkipTiles.size(); i++)
	{
		if(nimkipTiles[i].type  == NIMKIP)
		{
			//if that one needs help then stop checking others
			if(helpNimkip(nimkipTiles[i],fightsOnly))
				return;
		}
	}
	//if they were on their way to help but the help is no longer necessary
	if(helping)
	{
		helping = false;
		secondaryTask = attackTask;
		target = this->secondaryTarget;
		destination = helpDestination;
	}
}

bool Nimkip::helpNimkip(GridLoc nimkip, bool fightsOnly)
{
	NimkipInfo info = level::getNimkipInfo(nimkip);
	//makes it if they only want to help fight they only stop for fights
	if(fightsOnly && info.task != ATTACK)
		return false;
	if(info.needHelp && (info.sTask!=IDLE || info.task!=IDLE))
	{
		this->secondaryTarget = target;
		this->target = info.target;
		this->task = WALK;
		//saves their actual task to be switched to after the fight
		this->attackTask = this->secondaryTask;
		this->secondaryTask = info.task;
		auto goalSurroundings = level::getSurroundings(info.target,1);
		for(int i = 0; i < goalSurroundings.size(); i++)
		{
			if(goalSurroundings[i].type == EMPTY)
			{
				this->destination = goalSurroundings[i];
				break;
			}
			//if they dont find an open space just go as close as possible
			this->destination = info.goal;
		}
		this->helpDestination = this->destination;
		helping = true;
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

bool Nimkip::attack(GridLoc& p)
{
	return level::runAttack(this, target);
}

void YellowKip::move(GridLoc& p) { 
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
       //GridLoc curPos = getGridLoc();
       //VECTOR2 dirVec = VECTOR2((float)(p.x - curPos.x),(float)(p.y - curPos.y));

       //       surroundings = level::getSurroundings(curPos);
       //       Surroundings nSur, eSur, wSur, sSur;
       //   if(surroundings.N != OVER) 
       //              nSur = level::getSurroundings(GridLoc(curPos.x,curPos.y - 1));
       //   else {
       //              nSur.E = OVER;
       //              nSur.N = OVER;
       //              nSur.NE = OVER;
       //              nSur.NW = OVER;
       //              nSur.S = OVER;
       //              nSur.SE = OVER;
       //              nSur.SW = OVER;
       //              nSur.W = OVER;
       //   }
       //   if(surroundings.E != OVER) 
       //              eSur = level::getSurroundings(GridLoc(curPos.x + 1,curPos.y));
       //   else {
       //              eSur.E = OVER;
       //              eSur.N = OVER;
       //              eSur.NE = OVER;
       //              eSur.NW = OVER;
       //              eSur.S = OVER;
       //              eSur.SE = OVER;
       //              eSur.SW = OVER;
       //              eSur.W = OVER;
       //   }
       //   if(surroundings.S != OVER)
       //              sSur = level::getSurroundings(GridLoc(curPos.x - 1,curPos.y + 1));
       //   else {
       //              sSur.E = OVER;
       //              sSur.N = OVER;
       //              sSur.NE = OVER;
       //              sSur.NW = OVER;
       //              sSur.S = OVER;
       //              sSur.SE = OVER;
       //              sSur.SW = OVER;
       //              sSur.W = OVER;
       //   }
       //   if(surroundings.W != OVER)
       //              wSur = level::getSurroundings(GridLoc(curPos.x,curPos.y));
       //   else {
       //              wSur.E = OVER;
       //              wSur.N = OVER;
       //              wSur.NE = OVER;
       //              wSur.NW = OVER;
       //              wSur.S = OVER;
       //              wSur.SE = OVER;
       //              wSur.SW = OVER;
       //              wSur.W = OVER;
       //   }

       //if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x > 0)  && (dirVec.y > 0)) {//1st quadrant movement
       //       if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       } else if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       } else if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       } else if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       }
       //       return;
       //} else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x > 0)  && (dirVec.y > 0)) {
       //       if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       } else if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       } else if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       } else if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       }
       //       return;
       //} else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x < 0)  && (dirVec.y > 0)) {//2nd quadrant movement
       //       if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       } else if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       } else if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       } else if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       }
       //       return;
       //} else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x < 0)  && (dirVec.y > 0)) {
       //       if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       } else if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       } else if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       } else if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       }
       //       return;
       //} else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x < 0) && (dirVec.y < 0)) {//3rd quadrant movement
       //       if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       } else if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       } else if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       } else if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       }
       //       return;
       //} else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x < 0) && (dirVec.y < 0)) {
       //       if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       } else if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       } else if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       } else if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       }
       //       return;
       //} else if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x > 0) && (dirVec.y < 0)) {//4th quadrant movement
       //       if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       } else if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       } else if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       } else if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       }
       //       return;
       //} else if((abs(dirVec.x) < abs(dirVec.y)) && (dirVec.x > 0) && (dirVec.y < 0)) {
       //       if(surroundings.N == 6) {
       //              setDir(UP);
       //       } else if (nSur.N == 6) {
       //              setGridLoc(curPos.x, curPos.y - 2);
       //       } else if(surroundings.E == 6) {
       //              setDir(RIGHT);
       //       } else if(eSur.E == 6) {
       //              setGridLoc(curPos.x + 2, curPos.y);
       //       } else if(surroundings.W == 6) {
       //              setDir(LEFT);
       //       } else if(wSur.W == 6) {
       //              setGridLoc(curPos.x - 2, curPos.y);
       //       } else if(surroundings.S == 6) {
       //              setDir(DOWN);
       //       } else if(sSur.S == 6) {
       //              setGridLoc(curPos.x, curPos.y + 2);
       //       }
       //       return;
       //}
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

	if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == EMPTY || level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
		if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
			level::collectCoin(GridLoc(curPos.x + a,curPos.y - b));
			setScored(true);
		}
		chosenDirection = (moverNS::DIR) dirVal;
		setDir(chosenDirection);
		if(getHeldObject())
			getHeldObject()->setDir(chosenDirection);
		return;
	} else {//Obstacle Avoidance
		if((a + b) % 2 != 0) { //Cardinal directions
			if(a == 0) {
				a = 1*(dirVec.x/abs(dirVec.x));
				b = 0;
			} else {
				a = 0;
				b = 1*(dirVec.y/abs(dirVec.y));
			}
		} else { //Non-cardinal directions
			if(abs(dirVec.x) > abs(dirVec.y))
				a = 0;
			else
				b = 0;
		}

		c = a;
		if(c < 0) c = 2;
		d = b;
		if(d < 0) d = 2;

		dirVal = 3*c + d;

		switch(dirVal) {
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

		if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == EMPTY || level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
			if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
				level::collectCoin(GridLoc(curPos.x + a,curPos.y - b));
				setScored(true);
			}
			chosenDirection = (moverNS::DIR) dirVal;
			setDir(chosenDirection);
			if(getHeldObject())
				getHeldObject()->setDir(chosenDirection);
		return;
		}
	}
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
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
	}*/
}

void BlueKip::move(GridLoc& p) {  
	GridLoc curPos = getGridLoc();
	VECTOR2 dirVec = VECTOR2((float)(p.x - curPos.x),(float)(p.y - curPos.y));

    surroundings = level::getSurroundings(curPos);

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

	if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == EMPTY || level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
		if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
			level::collectCoin(GridLoc(curPos.x + a,curPos.y - b));
			setScored(true);
		}
		chosenDirection = (moverNS::DIR) dirVal;
		setDir(chosenDirection);
		if(getHeldObject())
			getHeldObject()->setDir(chosenDirection);
		return;
	} else {//Obstacle Avoidance
		if((a + b) % 2 != 0) { //Cardinal directions
			if(a == 0) {
				a = 1*(dirVec.x/abs(dirVec.x));
				b = 0;
			} else {
				a = 0;
				b = 1*(dirVec.y/abs(dirVec.y));
			}
		} else { //Non-cardinal directions
			if(abs(dirVec.x) > abs(dirVec.y))
				a = 0;
			else
				b = 0;
		}

		c = a;
		if(c < 0) c = 2;
		d = b;
		if(d < 0) d = 2;

		dirVal = 3*c + d;

		switch(dirVal) {
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

		if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == EMPTY || level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
			if(level::getTileType(GridLoc(curPos.x + a,curPos.y - b)) == COIN) {
				level::collectCoin(GridLoc(curPos.x + a,curPos.y - b));
				setScored(true);
			}
			chosenDirection = (moverNS::DIR) dirVal;
			setDir(chosenDirection);
			if(getHeldObject())
				getHeldObject()->setDir(chosenDirection);
		return;
		}
	}

	/*
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
	}*/

	/*
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
       }*/
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