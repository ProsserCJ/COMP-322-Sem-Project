/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines a T class, which signifies 
 one space on the displayed grid, and a Grid class, which is an intelligent
 wrapper class for an array of Ts.  Functionality of the Grid includes
 handling user clicks, selecting/deselecting Ts, and storing/reporting
 what type of object is occupying a given T.
*/ 

#ifndef __GRID_H
#define __GRID_H

#include "image.h"
#include "game.h"
#include "constants.h"
#include "input.h"
#include "object.h"
#include "importantTypes.h"
#include <vector>
using std::vector;

//Our tile class to use with Grid
class Tile: public Image{
public:
	Tile() {}
	Tile(int x, int y, Space s);
	void toggle();
	Space getType()			{return type;}
	void setType(Space t)	{type = t;}
	GridLoc getGridLoc()	{return pos;}
	bool isSelected()		{return selected;}

private:
	bool selected;
	Space type;
	GridLoc pos;
};

//wrapper for a 2D array
template<class T>
class Grid{
private:
	int numKips;
	vector<T*> temp;
	vector<GridLoc> nimkipLocations;
	Task primaryTask;
	Task secondaryTask;
	GridLoc destination;
	T** grid;
	Input* input;
	int width, height;
	bool nimkipSelected;
	bool goalSelected;

public:
	//Constructors
	Grid(){primaryTask = IDLE; secondaryTask = IDLE; nimkipSelected = false; goalSelected = false;destination = GridLoc();}
	~Grid(){for(int i = 0; i < width; i++) delete [] grid[i]; }	

	//Setters
	void setType(int x, int y, Space t)	{grid[x][y].setType(t);}
	void setType(GridLoc& g, Space t)	{grid[g.x][g.y].setType(t);}
	
	//Getters
	int getWidth()				 {return width;}
	int getHeight()				 {return height;}
	T getTile(GridLoc location)	 {return grid[location.x][location.y];}
	vector<GridLoc> getNimkips() {return nimkipLocations;}
	Task getPTask()				 {return primaryTask;}
	Task getSTask()				 {return secondaryTask;}
	GridLoc getDestination()	 {return destination;}

	//More complex functions
	bool initialize(Game* game, int w, int h, int ncols, TextureManager *t){
		width = w; height = h;
		grid = new T*[width];
	
		for (int i=0; i<width; i++){
			grid[i] = new T[height];
		
			for(int j=0; j<height; j++){ 
				grid[i][j] = T(i,j,EMPTY);
				if (!grid[i][j].initialize(game->getGraphics(), GRID_SIZE, GRID_SIZE, 2, t))
					return false;
				setType(i,j,EMPTY);
			}
		}
		input = game->getInput();

		return true;
	}

	void draw(){
		  for(int i=0; i<width; i++)
			for(int j=0; j<height; j++)
				grid[i][j].draw(); 
	}

	void update(){	
		int x = input->getMouseX()/GRID_SIZE;
		int y = input->getMouseY()/GRID_SIZE;
		if (x < width && y < height){	
			switch(grid[x][y].getType()){
				 case NIMKIP: 
					 if(!grid[x][y].isSelected())
					 {
						nimkipSelected = true;
						numKips++;
						grid[x][y].toggle();
					 }
					 else
					 {
						 numKips--;
						 if(numKips==0)
						 {
							nimkipSelected = false;
							goalSelected = false;
							clearSelected();
						 }
						 else
							 grid[x][y].toggle();
					 }
					 break;
				 case BROBLUB:
				 case COIN:
				 case EMPTY:
					 grid[x][y].toggle();
					 if(nimkipSelected && !goalSelected)//if the use selected an nimkip then let them click elsewhere
					 {	 
						 primaryTask = WALK;
						 secondaryTask = IDLE;
						 destination = GridLoc(x,y);
						 goalSelected = true;
					 }
					 else
					 {
						 grid[x][y].toggle();
					 }
					 break;
				 case FOOD:
					 grid[x][y].toggle();
					 if(nimkipSelected && !goalSelected)//if the use selected an nimkip then let them click elsewhere
					 {	 
						 primaryTask = WALK;
						 secondaryTask = LIFT;
						 destination = GridLoc(x,y);
						 goalSelected = true;
					 }
					 else
					 {
						 //goalSelected = false;
						 grid[x][y].toggle();
					 }
					 break;
			}
			input->setMouseLButton(false);
		}
	}

	vector<T*>& getTiles(){	
		temp.clear();

		for (int i=0; i<width; i++)
			for (int j=0; j<height; j++)		
					temp.push_back(&grid[i][j]);	
	
		return temp;
	}

	vector<T*>& getSelectedTiles(){	
		temp.clear();

		for (int i=0; i<width; i++)
			for (int j=0; j<height; j++)
				if (grid[i][j].isSelected())
					temp.push_back(&grid[i][j]);	
	
		return temp;
	}

	vector<T*>& getUnselectedTiles(){	
		temp.clear();

		for (int i=0; i<width; i++)
			for (int j=0; j<height; j++)
				if (!grid[i][j].isSelected())
					temp.push_back(&grid[i][j]);	
	
		return temp;
	}

	void clearSelected(){
		auto Ts = getSelectedTiles();
		for (auto it = Ts.begin(); it != Ts.end(); ++it)
			(*it)->toggle();	
	}

	void setType(GridLoc& g, textureType t){ 
		switch(t){
		case RN: 
		case BN:
		case YN: grid[g.x][g.y].setType(NIMKIP); break;
		case B:  grid[g.x][g.y].setType(BROBLUB); break;
		case F:  grid[g.x][g.y].setType(FOOD); break;
		case O:  grid[g.x][g.y].setType(OBSTACLE); break;
		case G:  grid[g.x][g.y].setType(GAP); break;
		default: grid[g.x][g.y].setType(EMPTY); break;
		}
	}
};

#endif