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

const float SCROLL_SPEED = 320.f;
const int SCROLL_DETECT_OFFSET = 75;

//Our tile class to use with Grid
class Tile: public Image{
public:
	Tile() {}
	Tile(int x, int y, Space s);

	//Getters
	Object* getObject()			{return object;}
	Space getType()				{return type;}
	GridLoc getGridLoc()		{return pos;}
	bool isSelected()			{return selected;}

	//Setters
	void setType(Space t)		{type = t;}	
	void setObject(Object* o)	{object = o;}	
	
	//Other Actions
	void scroll(VECTOR2& center);
	void toggle();

private:
	bool selected;
	Space type;
	GridLoc pos;
	Object* object;
};

//wrapper for a 2D array
template<class T>
class Grid{
public:
	//Constructors
	Grid()
	{
		primaryTask = IDLE; 
		secondaryTask = IDLE; 
		nimkipSelected = false; 
		goalSelected = false;
		destination = GridLoc();
		center = VECTOR2((GAME_WIDTH-HUD_WIDTH)/2.f, GAME_HEIGHT/2.f);
	}
	~Grid(){for(int i = 0; i < width; i++) delete [] grid[i]; }	

	//Setters
	void setType(int x, int y, Space t)	{grid[x][y].setType(t);}
	void setType(GridLoc& g, Space t)	{grid[g.x][g.y].setType(t);}
	
	//Getters
	int getWidth()				 {return width;}
	int getHeight()				 {return height;}
	T& getTile(GridLoc location) {return grid[location.x][location.y];}
	vector<GridLoc> getNimkips() {return nimkipLocations;}
	Task getPTask()				 {return primaryTask;}
	Task getSTask()				 {return secondaryTask;}
	GridLoc getDestination()	 {return destination;}

	//More complex functions
	bool initialize(Game* game, int w, int h, int ncols, TextureManager *t, Image* bg){
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
		background = bg;

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

	void setObject(Object* o)
	{
		getTile(o->getGridLoc()).setObject(o);
	}

	void scroll(float frameTime)
	{	
		float X = input->getMouseX(), Y = input->getMouseY();
		VECTOR2 diff = VECTOR2(0,0);
		if (X < SCROLL_DETECT_OFFSET && center.x > (GAME_WIDTH-HUD_WIDTH)/2) 
			diff.x += SCROLL_SPEED*frameTime;
		if (X > GAME_WIDTH - HUD_WIDTH - SCROLL_DETECT_OFFSET && X < GAME_WIDTH - HUD_WIDTH && center.x < ((width+width%2)*GRID_SIZE)/2) 
			diff.x -= SCROLL_SPEED*frameTime;
		if (Y < SCROLL_DETECT_OFFSET && center.y > GAME_HEIGHT/2) 
			diff.y += SCROLL_SPEED*frameTime;
		if (Y > GAME_HEIGHT - SCROLL_DETECT_OFFSET && center.y < ((height+height%2)*GRID_SIZE)/2) 
			diff.y -= SCROLL_SPEED*frameTime;
		
		if (diff == VECTOR2(0,0)) return;

		for (int i=0; i<width; i++)
			for (int j=0; j<height; j++)
			{			
				grid[i][j].setX(grid[i][j].getX() + diff.x);
				grid[i][j].setY(grid[i][j].getY() + diff.y);
				
				Object* O;
				if (O = grid[i][j].getObject())
				{		
					O->setX(O->getX() + diff.x);
					O->setY(O->getY() + diff.y);
				}
			}

			background->setX(background->getX() + diff.x);
			background->setY(background->getY() + diff.y);
			center -= diff;
	}

	void zoom(float frameTime)
	{
		float mult;
		if (input->wasKeyPressed('W')) mult = 1.01;
		else if (input->wasKeyPressed('S')) mult = .99;
		else return;

		for (int i=0; i<width; i++)
			for (int j=0; j<height; j++)
			{			
				grid[i][j].setScale(grid[i][j].getScale() * mult);
				grid[i][j].setX(grid[i][j].getX() * mult);
				grid[i][j].setY(grid[i][j].getY() * mult);

				Object* O;
				if (O = grid[i][j].getObject())
				{		
					O->setScale(O->getScale() * mult);
					O->setX(O->getX() * mult);
					O->setY(O->getY() * mult);
				}
			}

			background->setScale(background->getScale()*mult);
			background->setX(background->getX() * mult);
			background->setY(background->getY() * mult);
			//center *= mult;
	}
	
private:
	T** grid;
	int width, height;
	int numKips;
	vector<T*> temp;
	vector<GridLoc> nimkipLocations;
	
	//Tasks
	Task primaryTask;
	Task secondaryTask;
	GridLoc destination;
	bool goalSelected;
	
	//Scroll, zoom, and selection
	Input* input;
	Image* background; //pointer to the background image to update on scroll and zoom
	VECTOR2 center;
	bool nimkipSelected;	
	
};

#endif