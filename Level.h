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

//currently when the nimkip tries to go to lifting mode everything dies

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
#include "statue.h"

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
	static bool runAttack(Lifeform* attacker, GridLoc target);	//returns true if target is completely destroyed
	static vector<GridLoc> getSurroundings(GridLoc currentLocation, int sightRadius);
	static Space getTileType(GridLoc tile);
	static void collectCoin(GridLoc coinLocation);
	static Surroundings getSurroundings(GridLoc currentLocation);
	static void getUserInput();
	static int getGridHeight();
	static int getGridWidth();
	static NimkipInfo getNimkipInfo(GridLoc nimkip);

	bool isRunning(){return !paused;}
	void pause(){paused = true; hud.setCurrentFrame(0); grid.clearInput();}
	void unpause(){paused = false;}
	TextDX* getFont() {return gameFont;}

private:
    // game items
    TextureManager gridTexture;   // grid texture
	TextureManager backgroundTexture; // orion texture;
    TextureManager gameTextures;    // game texture
	TextureManager hudTexture;
	TextureManager winScreenTexture, gameOverScreenTexture;
	ofstream debug;
	static Grid<Tile> grid;
	Image	background, winScreen, gameOverScreen;	
	HUD		hud;
	TextDX*	gameFont;
	std::stringstream ss;
	Random rgen;
	bool paused;
	int frameCount;
	int turns;
	int score;
	int numEnemies, numKips, maxKips, mapSizeX, mapSizeY;
	int foodCollected;
	
	//end game stuff
	bool win;
	bool lose;
	bool endGame;

	//vector of carriable items
	static vector<Carriable*> carriableItems;
	//vector of noncarriables ie obstacles and such
	static vector<Object*> unCarriables;
	//vector that contains both so that it can control both at once
	static vector<Lifeform*> lifeForms;
	
	//void createEmptyLevel();
	void fillLevel();
	
	//finds the lifeform pointer associated with a position
	static Lifeform* identifyLifeForm(GridLoc pos);
	//same thing but for an object
	static Carriable* identifyObject(GridLoc pos);
};

#endif