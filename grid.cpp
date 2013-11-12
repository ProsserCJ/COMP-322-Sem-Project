/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file implements the Grid and Tile classes
*/ 

#include "grid.h"
#include <vector>
using std::vector;

Tile::Tile(int x=0, int y=0, Space s=EMPTY){
	setX(x*GRID_SIZE); 
	setY(y*GRID_SIZE);
	pos = GridLoc(x,y);
	selected = false;
}

void Tile::toggle(){
	if (type == OBSTACLE) return;
	selected = !selected;
	if (getCurrentFrame() == 0) setCurrentFrame(1);
	else setCurrentFrame(0);
}