#include "horde.h"

Horde::Horde(Carriable* c, Lifeform* l) {
	hordeObject = c;
	hordeList.push_back(l);
	hordeNum = 1;
	l->setHolding(true);
	setGridLoc(hordeObject->getGridLoc());
	totalStrength = l->getStrength();
}


void Horde::disband() {
	if(hordeList.empty())
		return;

	for(int i = 0; i < hordeList.size(); i++) {
		hordeList[i]->setHolding(false);
	}

	int sight = 0;
	while(!hordeList.empty()) {
		sight++;
		visibleTiles = level::getSurroundings(getGridLoc(),sight); //getSightRadius()? -- unsure of use for horde
		for(int i = 0; i < visibleTiles.size(); i++) {
			hordeList[0]->setGridLoc(visibleTiles[i]);
			dropMember(0);
			if(hordeList.empty())
				break;
		}
	}

	/* //Unneed with new surroundings function
	checkSurroundings();
	Surroundings sur = surroundings;
	findEmptyAndDrop(sur);

	sur = level::getSurroundings(GridLoc(pos.x + 1,pos.y));
	findEmptyAndDrop(sur);

	sur = level::getSurroundings(GridLoc(pos.x  + 1,pos.y + 1));
	findEmptyAndDrop(sur);

	sur = level::getSurroundings(GridLoc(pos.x,pos.y + 1));
	findEmptyAndDrop(sur);

	sur = level::getSurroundings(GridLoc(pos.x - 1,pos.y + 1));
	findEmptyAndDrop(sur);

	sur = level::getSurroundings(GridLoc(pos.x - 1,pos.y));
	findEmptyAndDrop(sur);

	sur = level::getSurroundings(GridLoc(pos.x,pos.y - 1));
	findEmptyAndDrop(sur);

	sur = level::getSurroundings(GridLoc(pos.x + 1,pos.y - 1));
	findEmptyAndDrop(sur);*/
}

void Horde::findEmptyAndDrop(Surroundings sur) {
	//No longer in use
	if(hordeList.empty())
			return;

	int sight = 0;
	while(!hordeList.empty()) {
		sight++;
		visibleTiles = level::getSurroundings(getGridLoc(),sight); //getSightRadius()? -- unsure of use for horde
		for(int i = 0; i < visibleTiles.size(); i++) {
			hordeList[0]->setGridLoc(visibleTiles[i]);
			dropMember(0);
			if(hordeList.empty())
				break;
		}
	}

	/*
	int i = 0;
	if(sur.E == 6) {
		hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	} else if(sur.S == 6) {
		hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	} else if(sur.N == 6) {
        hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	} else if(sur.W == 6) {
        hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	} else if(sur.NE == 6) {
		hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	} else if(sur.NW == 6) {
        hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	} else if(sur.SE == 6) {
        hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	} else if(sur.SW == 6) {
        hordeList[i]->setGridLoc(pos.x + 1, pos.y);
		dropMember(i);
		if(hordeList.empty())
			return;
		i++;
	}*/
}

void Horde::addMember(Lifeform* l) {
	hordeList.push_back(l);
	hordeNum++;
	l->setActive(false);
	l->setHolding(true);
	totalStrength += l->getStrength();	
}

void Horde::subHealth(int diff) {
	int i = rand()%hordeList.size();
	hordeList[i]->subHealth(diff);

	if(hordeList[i]->getHealth() <= 0) {
		hordeList[i]->die();
		dropMember(i);
	}
}

void Horde::dropMember(int i) {
	int x = 0;
	vector<Lifeform*>::iterator it;
	for(it = hordeList.begin(); it != hordeList.end(); it++) {
		x++;
		if(x == i)
			break;
	}

	hordeNum--;
	totalStrength -= hordeList[i]->getStrength();
	hordeList.erase(it);
}

GridLoc Horde::takeTurn() {
	//user input?


	move(destination);
	return getGridLoc();
}

void Horde::checkSurroundings() {
	surroundings = level::getSurroundings(getGridLoc());
}

void Horde::move(GridLoc& p) {
	if(totalStrength < hordeObject->getWeight())
		return;

	bool yellowCheck = true;
	for(int i= 0; i < hordeList.size(); i++) {
		if(hordeList[i]->getImage() != YN)
			yellowCheck = false;
	}

	if(yellowCheck)
		moveYellow(p);
	else
		moveNormal(p);
}

void Horde::moveNormal(GridLoc& p) {
	GridLoc curPos = getGridLoc();
    VECTOR2 dirVec = VECTOR2((float)(p.x - curPos.x),(float)(p.y - curPos.y));

    surroundings = level::getSurroundings(curPos);


	/*
    if((abs(dirVec.x) > abs(dirVec.y)) && (dirVec.x > 0)  && (dirVec.y > 0)) {//1st quadrant movement
			if(surroundings.E == 2) {
                    setDir(RIGHT);					
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            }
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
			if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            }
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
			if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            }
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
			if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            }
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
			if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            }
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
			if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            }
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
			if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            }
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
			if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            }

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
	return;
}

void Horde::moveYellow(GridLoc& p) {
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
			if(surroundings.E == 2) {
                    setDir(RIGHT);					
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            }else if(surroundings.E == 6) {
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
		   if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.S == 6) {
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
		   if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.W == 6) {
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
		   if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.S == 6) {
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
		   if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.W == 6) {
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
		   if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.N == 6) {
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
		   if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.E == 6) {
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
		   if(surroundings.N == 2) {
                    setDir(UP);
            } else if(surroundings.E == 2) {
                    setDir(RIGHT);
            } else if(surroundings.W == 2) {
                    setDir(LEFT);
            } else if(surroundings.S == 2) {
                    setDir(DOWN);
            } else if(surroundings.N == 6) {
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