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
	surroundings = level::getSurroundings(getGridLoc());
	if(surroundings.N==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x,pos.y-1);
		return;
	}
	if(surroundings.S==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x,pos.y+1);
		return;
	}
	if(surroundings.E==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x+1,pos.y);
		return;
	}
	if(surroundings.W==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x-1,pos.y);
		return;
	}
	if(surroundings.NE==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x+1,pos.y-1);
		return;
	}
	if(surroundings.NW==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x-1,pos.y-1);
		return;
	}
	if(surroundings.SE==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x+1,pos.y+1);
		return;
	}
	if(surroundings.SW==BROBLUB)
	{
		attackTask = task;
		task = ATTACK;
		target = GridLoc(pos.x-1,pos.y+1);
		return;
	}
}

GridLoc Nimkip::takeTurn()
{
	if(this->getHealth()>0)
	{
		getUserInput();//check to see if the use gave them something to do
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
		if(!this->getHolding())
		{
			level::transferObject(this,target);
			getHeldObject()->changeCarrierStrength(this->getStrength());
		}
		else
		{//if they can't pick it up then they need help
			needHelp = true;
		}
		//if the combined strength is enough to pick it up say they don't need help and set them to carry it
		if(getHeldObject()->getWeight() <= getHeldObject()->getCarrierStrength())
		{
			needHelp = false;
			task = CARRY;
			destination = homeBase;
		}
		break;
	case ATTACK:
		attack(target);
		task = attackTask;//puts their task back to normal
		break;
	case CARRY:
		//if the object becomes too heavy, ie another nimkip dies then they have to stop moving
		if(getHeldObject()->getCarrierStrength()<getHeldObject()->getWeight())
		{
			task = LIFT;
			needHelp = true;
		}
		else
		{
			move(destination);
		}
		break;
	}
	//if they make it home
	if(this->pos.x ==destination.x && pos.y == destination.y)
	{
		task = IDLE;
		//perhaps hoard disband
		//dont do it again if it is already done
		if(this->getHeldObject()->getActive())
		{
			this->getHeldObject()->setVisible(false);
			this->getHeldObject()->setActive(false);
			//used to update the level score
			setScored(true);
			setScorePoints(50);
		}
	}
	return GridLoc();
}

void Nimkip::checkOthers()
{
	GridLoc loc;
	if(surroundings.E == NIMKIP)
	{
		loc.x = pos.x+1;
		loc.y = pos.y;
		if(helpNimkip(loc))
			return;
	}

	if(surroundings.NE == NIMKIP)
	{
		loc.x = pos.x+1;
		loc.y = pos.y-1;
		if(helpNimkip(loc))
			return;
	}

	if(surroundings.N == NIMKIP)
	{
		loc.x = pos.x;
		loc.y = pos.y-1;
		if(helpNimkip(loc))
			return;
	}
	
	if(surroundings.NW == NIMKIP)
	{
		loc.x = pos.x-1;
		loc.y = pos.y-1;
		if(helpNimkip(loc))
			return;
	}
	
	if(surroundings.W == NIMKIP)
	{
		loc.x = pos.x-1;
		loc.y = pos.y;
		if(helpNimkip(loc))
			return;
	}
	
	if(surroundings.SW == NIMKIP)
	{
		loc.x = pos.x-1;
		loc.y = pos.y+1;
		if(helpNimkip(loc))
			return;
	}
	
	if(surroundings.S == NIMKIP)
	{
		loc.x = pos.x;
		loc.y = pos.y+1;
		if(helpNimkip(loc))
			return;
	}
	
	if(surroundings.SE == NIMKIP)
	{
		loc.x = pos.x+1;
		loc.y = pos.y+1;
		if(helpNimkip(loc))
			return;
	}
}

bool Nimkip::helpNimkip(GridLoc nimkip)
{
	/*NimkipInfo info = level::getNimkipInfo(nimkip);
	if(info.needHelp)
	{
		this->task = info.task;
		this->destination = info.goal;
		this->needHelp = info.needHelp;
		return true;
	}*/
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

void RedKip::move(GridLoc& p) {  
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