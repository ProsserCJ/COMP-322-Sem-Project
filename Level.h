/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file is inherited from the skeleton
 provided by Charles Kelly.  We have modified names, includes,
 and added specific functionality to the level class.
*/ 

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 level.h v1.0

#ifndef _LEVEL_H             // Prevent multiple definitions if this 
#define _LEVEL_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"
#include "hud.h"
#include "object.h"
#include "random.h"
#include "broblub.h"
#include "nimkip.h"
#include "carriable.h"
#include "grid.h"
#include "base.h"

#include <sstream>
#include <vector>
#include <fstream>
using std::ofstream;
using std::vector;

//=============================================================================
// This class is the core of the game
//=============================================================================
class level : public Game
{
private:
    // game items
    TextureManager gridTexture;   // grid texture
	TextureManager backgroundTexture; // orion texture;
    TextureManager gameTextures;    // game texture
	TextureManager hudTexture;
	ofstream debug;
	static Grid<Tile> grid;
	Image	background;	
	HUD		hud;
	TextDX*	gameFont;
	std::stringstream ss;
	Random rgen;
	bool paused;
	int frameCount;
	int turns;
	int score;
	int numEnemies, startKips, maxKips, mapSizeX, mapSizeY;

	//basically only for display purposes
	static vector<Object*> objects;
	//vector that contains both so that it can control both at once
	static vector<Lifeform*> lifeForms;
	
	//void createEmptyLevel();
	void fillLevel();
	
	//finds the lifeform pointer associated with a position
	static Lifeform* identifyLifeForm(GridLoc pos);
	//same thing but for an object
	static Object* identifyObject(GridLoc pos);

	static NimkipInfo getNimkipInfo(GridLoc nimkip);

public:
    // Constructor
    level();
	level(int numEnemies, int numKips, int maxKips, int sizeX, int sizeY);

    // Destructor
    virtual ~level();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
	void restart();
	void gameOver();	
	void runTimeStep();	
	static bool transferObject(Lifeform* receiver, GridLoc item);
	static void runAttack(Lifeform* attacker, GridLoc target);	
	static Surroundings getSurroundings(GridLoc currentLocation);
	static void getUserInput();

	bool isRunning(){return !paused;}
	void pause(){paused = true;}
	void unpause(){paused = false;}
};

#endif