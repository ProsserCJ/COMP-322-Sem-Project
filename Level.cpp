/* 
 Group #: Team 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andy Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file, heavily based on the skeleton 
 provided by Charles Kelly, implements the level class
 and runs a graphical Nimkip simulation. 
*/ 

//Skeleton provided by:

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 level.cpp v1.0


#include "level.h"

#define SQR(n) ((n)*(n))

//=============================================================================
// Constructors
//=============================================================================

level::level()
{
	gameFont = new TextDX();
	rgen = Random(0,9);
	this->numEnemies = 5;
	this->startKips = 10;
	this->mapSizeX = 50;
	this->mapSizeY = 50;
	this->maxKips = 20;
	frameCount = 0;
	turns = 0;
	score=0;
	
	debug.open("debug.txt");
}


//creating static data
Grid<Tile> level::grid;
vector<Lifeform*> level::lifeForms;
vector<Object*> level::objects;

//=============================================================================
// Destructor
//=============================================================================
level::~level()
{
	debug.close();
    for(int i = 0; i < objects.size(); i++) delete objects[i];	
	for(int i = 0; i < lifeForms.size(); i++) delete lifeForms[i];
			
	
	//releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void level::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // grid texture
    if (!gridTexture.initialize(graphics,GRID_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing grid texture"));

	// background texture
	if (!backgroundTexture.initialize(graphics,BACKGROUND_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

    // main game textures
    if (!gameTextures.initialize(graphics,TEXTURES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));
	
	// hud texture
    if (!hudTexture.initialize(graphics,HUD_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hud texture"));


	// background image
    if (!background.initialize(graphics,mapSizeX*GRID_SIZE,mapSizeY*GRID_SIZE,0,&backgroundTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));  	

	// HUD
	if (!hud.initialize(this, HUD_WIDTH, GAME_HEIGHT, 2, &hudTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hud"));

	// grid 
	if(!grid.initialize(this, mapSizeX, mapSizeY, 1, &gridTexture, &background))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing grid"));

	
	fillLevel();	
	//lifeForms
	for(int i = 0; i < lifeForms.size(); i++)
	{
		if (!lifeForms[i]->initialize(this, GRID_SIZE, GRID_SIZE, 3, &gameTextures, gameFont))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lifeform"));
		
		switch(lifeForms[i]->getImage())
		{
			case RN: 
				lifeForms[i]->setCurrentFrame(RNIM); 
				grid.setType(lifeForms[i]->getGridLoc(), NIMKIP);				
				break;	
			case YN:
				lifeForms[i]->setCurrentFrame(YNIM);
				grid.setType(lifeForms[i]->getGridLoc(),NIMKIP);
				break;
			case BN:
				lifeForms[i]->setCurrentFrame(BNIM);
				grid.setType(lifeForms[i]->getGridLoc(),NIMKIP);
				break;
			case B: 
				lifeForms[i]->setCurrentFrame(BRO); 
				grid.setType(lifeForms[i]->getGridLoc(), BROBLUB);	
				break;			
		}
	}

	for(int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->initialize(this, GRID_SIZE, GRID_SIZE, 3, &gameTextures))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing object"));
		switch(objects[i]->getImage())
		{
		case C:
			objects[i]->setCurrentFrame(MONEY);
			grid.setType(objects[i]->getGridLoc(),COIN);
			break;
		case F:
			objects[i]->setCurrentFrame(CHICKEN);
			grid.setType(objects[i]->getGridLoc(),FOOD);
			break;
		case O:
			objects[i]->setCurrentFrame(ROCK);
			grid.setType(objects[i]->getGridLoc(),OBSTACLE);
			break;
		case G:
			objects[i]->setCurrentFrame(HOLE);
			grid.setType(objects[i]->getGridLoc(),GAP);
			break;
		case BS:
			objects[i]->setCurrentFrame(NIMBASE);
			grid.setType(objects[i]->getGridLoc(),BASE);
			break;
		}
	}
	
	hud.setX(hudNS::UPPER_X);
	
	gameFont->initialize(graphics, 20, false, false, "Calibri");
	gameFont->setFontColor(SETCOLOR_ARGB(255,0,0,0));

	pause();
	
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void level::update()
{  		
	if(input->getMouseRButton()){
		auto tiles = grid.getSelectedTiles();
		for(auto it = tiles.begin(); it != tiles.end(); ++it){
			(*it)->toggle();		
		}
	}
	
	if(input->getMouseLButton()){
		if(isRunning()) pause();		
		if(hud.wasClicked()){ 
			grid.clearSelected();			
			input->setMouseLButton(false);
			unpause();
			hud.setCurrentFrame(1);	
			this->getUserInput();
		} 		
		grid.update();
	}

	if (isRunning() && (frameCount%250 == 0) && (lifeForms[0]->getDistToMove()==0))
		//^sloppy.  should get a better check for if lifeforms are moving
		runTimeStep();
	

	for(int i=0; i<lifeForms.size(); i++)
		lifeForms[i]->move(frameTime);

	//call zoom and scroll, and pause the action is either is taking place.
	grid.scroll(frameTime);
	if(grid.zoom(frameTime)) pause();

	
	frameCount++;
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void level::ai()
{
	
}

//=============================================================================
// Handle collisions
//=============================================================================
void level::collisions()
{		

}

//=============================================================================
// Render game items
//=============================================================================
void level::render()
{
    graphics->spriteBegin();                // begin drawing sprites

       background.draw();
       grid.draw();
     
       for(int i = 0; i < lifeForms.size(); i++)
       {
             if (lifeForms[i]->getActive()) lifeForms[i]->draw();
       }
       for(int i = 0; i < objects.size(); i++)
       {
             objects[i]->draw();
       }
        hud.draw();
		gameFont->setFontColor(SETCOLOR_ARGB(255,0,0,0));
		ss.str(std::string());
		ss << "Turn  " << turns << "\nScore " << score;
		gameFont->print(ss.str(), 830, 410);

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void level::releaseAll()
{
    gameTextures.onLostDevice();
	gridTexture.onLostDevice();
	backgroundTexture.onLostDevice();   
	gameFont->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void level::resetAll()
{
    gameTextures.onResetDevice();
    gridTexture.onResetDevice();
	backgroundTexture.onResetDevice();
	gameFont->onResetDevice();	
    Game::resetAll();
    return;
}

//*********************
//JAKES CODE STARTS HERE
//*********************

//makes all lifeforms take their turn and uses their new positions to update its map
void level::runTimeStep()
{
	grid.clearSelected();
	for(int i = 0; i < lifeForms.size(); i++)
	{
		if(lifeForms[i]->getActive())
		{			
			//changes the levelMap based off of the new location of the lifeForm
			lifeForms[i]->setNormal();
			grid.setType(lifeForms[i]->getGridLoc(),EMPTY);			
			GridLoc newGridLoc = lifeForms[i]->takeTurn();
			grid.setType(newGridLoc, lifeForms[i]->getImage());
			
			
			if(!lifeForms[i]->getActive())//if they are dead tell the level map
			{
				grid.setType(lifeForms[i]->getGridLoc(),EMPTY);	
			}
			//check if they scored
			if(lifeForms[i]->getScored())
			{
				score+=lifeForms[i]->getScoredPoints();
				lifeForms[i]->setScored(false);
			}
		}
	}
	turns++;
}

//transfers item to a Lifeform
//the Lifeform calls the function sending itself and the location where the item is.
bool level::transferObject(Lifeform* receiver, GridLoc item)
{
	//only works if the receiver is not already holding something
	if(!receiver->getHolding())
	{
		receiver->setHeldObject(reinterpret_cast<Carriable*>(identifyObject(item)));
		receiver->setHolding(true);
	}
	else
	{
		return false;
	}
	return true;
}

//takes the attacker and the location of the target, and runs the attack
void level::runAttack(Lifeform* attacker, GridLoc target)
{
	Lifeform* targetLifeForm = identifyLifeForm(target);
	if (attacker->isNormal()) attacker->setAtk(); 	
	targetLifeForm->subHealth(attacker->getAttackStrength());
	if (targetLifeForm->getHealth() < 10) targetLifeForm->setHurt();
}

//fills level with all necessary starting objects
void level::fillLevel()
{
	//create BASE 
       Base* b = new Base();
       //int xLoc, yLoc;
       b->setGridLoc(0,0);	  
       grid.setType(0,0,BASE);
       objects.push_back(reinterpret_cast<Object*>(b));


       //red nimkips
       RedKip* r1 = new RedKip();
       r1->setGridLoc(1, 0);             
       grid.setType(1,0,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(r1));
       r1=0;

       RedKip* r2 = new RedKip();
       r2->setGridLoc(0, 2);             
       grid.setType(0,2,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(r2));
       r2=0;

       RedKip* r3 = new RedKip();
       r3->setGridLoc(4, 0);             
       grid.setType(4,0,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(r3));
       r3=0;

       //blue nimkips
       BlueKip* b1 = new BlueKip();
       b1->setGridLoc(1, 1);             
       grid.setType(1,1,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(b1));
       b1=0;

       BlueKip* b2 = new BlueKip();
       b2->setGridLoc(2, 2);             
       grid.setType(2,2,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(b2));
       b2=0;

       BlueKip* b3 = new BlueKip();
       b3->setGridLoc(3, 1);             
       grid.setType(3,1,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(b3));
       b3=0;

       //yellow nimkips
       YellowKip* y1 = new YellowKip();
       y1->setGridLoc(2, 0);             
       grid.setType(2,0,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(y1));
       y1=0;

       YellowKip* y2 = new YellowKip();
       y2->setGridLoc(0, 1);             
       grid.setType(0,1,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(y2));
       y2=0;

       YellowKip* y3 = new YellowKip();
       y3->setGridLoc(2, 3);             
       grid.setType(2,3,NIMKIP);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(y3));
       y3=0;

       //create enemies
       Broblub* br1 = new Broblub();
       br1->setGridLoc(1,5);
       grid.setType(1,5,BROBLUB);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(br1));

       Broblub* br2 = new Broblub();
       br2->setGridLoc(2,8);
       grid.setType(2,8,BROBLUB);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(br2));

       Broblub* br3 = new Broblub();
       br3->setGridLoc(5,4);
       grid.setType(5,4,BROBLUB);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(br3));

       Broblub* br4 = new Broblub();
       br4->setGridLoc(7,1);
       grid.setType(7,1,BROBLUB);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(br4));

       Broblub* br5 = new Broblub();
       br5->setGridLoc(7,7);
       grid.setType(7,7,BROBLUB);
       lifeForms.push_back(reinterpret_cast<Lifeform*>(br5));

       //create coins
       Coin* c1 = new Coin();
       c1->setGridLoc(3, 5);             
       grid.setType(3,5,COIN);
       objects.push_back(reinterpret_cast<Object*>(c1));

       Coin* c2 = new Coin();
       c2->setGridLoc(5, 9);             
       grid.setType(5,9,COIN);
       objects.push_back(reinterpret_cast<Object*>(c2));

       Coin* c3 = new Coin();
       c3->setGridLoc(6, 6);             
       grid.setType(6,6,COIN);
       objects.push_back(reinterpret_cast<Object*>(c3));

       Coin* c4 = new Coin();
       c4->setGridLoc(7, 0);             
       grid.setType(7,0,COIN);
       objects.push_back(reinterpret_cast<Object*>(c4));

       Coin* c5 = new Coin();
       c5->setGridLoc(8, 4);             
       grid.setType(8,4,COIN);
       objects.push_back(reinterpret_cast<Object*>(c5));

       Coin* c6 = new Coin();
       c6->setGridLoc(9, 7);             
       grid.setType(9,7,COIN);
       objects.push_back(reinterpret_cast<Object*>(c6));

       //create food
       Food* f1 = new Food();
       f1->setGridLoc(1,7);
       grid.setType(1,7,FOOD);
       objects.push_back(reinterpret_cast<Object*>(f1));

       Food* f2 = new Food();
       f2->setGridLoc(4,6);
       grid.setType(4,6,FOOD);
       objects.push_back(reinterpret_cast<Object*>(f2));

       Food* f3 = new Food();
       f3->setGridLoc(5,2);
       grid.setType(5,2,FOOD);
       objects.push_back(reinterpret_cast<Object*>(f3));

       Food* f4 = new Food();
       f4->setGridLoc(9,1);
       grid.setType(9,1,FOOD);
       objects.push_back(reinterpret_cast<Object*>(f4));


       //create obstacles (rocks)
       Obstacle* o1 = new Obstacle();
       o1->setGridLoc(3,4);
       grid.setType(3,4,OBSTACLE);
       objects.push_back(reinterpret_cast<Object*>(o1));

       Obstacle* o2 = new Obstacle();
       o2->setGridLoc(4,8);
       grid.setType(4,8,OBSTACLE);
       objects.push_back(reinterpret_cast<Object*>(o2));

       Obstacle* o3 = new Obstacle();
       o3->setGridLoc(6,1);
       grid.setType(6,1,OBSTACLE);
       objects.push_back(reinterpret_cast<Object*>(o3));

       Obstacle* o4 = new Obstacle();
       o4->setGridLoc(6,5);
       grid.setType(6,5,OBSTACLE);
       objects.push_back(reinterpret_cast<Object*>(o4));

       Obstacle* o5 = new Obstacle();
       o5->setGridLoc(6,8);
       grid.setType(6,8,OBSTACLE);
       objects.push_back(reinterpret_cast<Object*>(o5));

       Obstacle* o6 = new Obstacle();
       o6->setGridLoc(7,3);
       grid.setType(7,3,OBSTACLE);
       objects.push_back(reinterpret_cast<Object*>(o6));

       Obstacle* o7 = new Obstacle();
       o7->setGridLoc(8,6);
       grid.setType(8,6,OBSTACLE);
       objects.push_back(reinterpret_cast<Object*>(o7));

	   for (auto it = objects.begin(); it != objects.end(); ++it) grid.setObject(*it);
	   for (auto it = lifeForms.begin(); it != lifeForms.end(); ++it) grid.setObject(*it);
}

//takes the location of the lifeform and the value for how far they can see
//will not return values for tiles that do not exist, the OVER space value is no longer necessary
vector<GridLoc> level::getSurroundings(GridLoc currentLocation, int sightRadius)
{
	vector<GridLoc> surroundings;
	//goes through and fills the surroundings vector with every visible tile
	for(int i = sightRadius*-1; i <= sightRadius; i++)
	{
		for(int j = sightRadius*-1; j <= sightRadius; j++)
		{
			if(i!=0 || j!=0)
			{
				if(currentLocation.x+i>=0 && currentLocation.x+i<grid.getWidth() && currentLocation.y+j>=0 && currentLocation.y+j<grid.getHeight())
					surroundings.push_back(grid.getTile(GridLoc(currentLocation.x+i,currentLocation.y+j)).getGridLoc());
			}
		}
	}

	return surroundings;
}

Surroundings level::getSurroundings(GridLoc currentLocation)
{
	Surroundings box;

	if(currentLocation.y>0 && currentLocation.x>=0 && currentLocation.x < grid.getWidth())
		//box.N = levelMap[currentLocation.x][currentLocation.y-1];
		box.N = grid.getTile(GridLoc(currentLocation.x,currentLocation.y-1)).getType();
	else
		box.N = OVER;
	
	if(currentLocation.y<grid.getHeight()-1 && currentLocation.x>=0 && currentLocation.x < grid.getWidth())//they are at the bottem
		//box.S = levelMap[currentLocation.x][currentLocation.y+1];
		box.S = grid.getTile(GridLoc(currentLocation.x,currentLocation.y+1)).getType();
	else
		box.S = OVER;
	
	if(currentLocation.x < grid.getWidth()-1 && currentLocation.y>=0 && currentLocation.y < grid.getHeight())
		//box.E = levelMap[currentLocation.x+1][currentLocation.y];
		box.E = grid.getTile(GridLoc(currentLocation.x+1,currentLocation.y)).getType();
	else
		box.E = OVER;
	
	if(currentLocation.x > 0 && currentLocation.y>=0 && currentLocation.y < grid.getHeight())
		//box.W = levelMap[currentLocation.x-1][currentLocation.y];
		box.W = grid.getTile(GridLoc(currentLocation.x-1,currentLocation.y)).getType();
	else
		box.W = OVER;
	
	if(currentLocation.x < grid.getWidth()-1 && currentLocation.y>0)
		//box.NE = levelMap[currentLocation.x+1][currentLocation.y-1];
		box.NE = grid.getTile(GridLoc(currentLocation.x+1,currentLocation.y-1)).getType();
	else
		box.NE = OVER;
	
	if(currentLocation.x > 0 && currentLocation.y > 0)
		//box.NW = levelMap[currentLocation.x-1][currentLocation.y-1];
		box.NW = grid.getTile(GridLoc(currentLocation.x-1,currentLocation.y-1)).getType();
	else
		box.NW = OVER;
	
	if(currentLocation.x < grid.getWidth()-1 && currentLocation.y<grid.getHeight()-1)
		//box.SE = levelMap[currentLocation.x+1][currentLocation.y+1];
		box.SE = grid.getTile(GridLoc(currentLocation.x+1,currentLocation.y+1)).getType();
	else
		box.SE = OVER;
	
	if(currentLocation.x > 0 && currentLocation.y<grid.getHeight()-1)
		//box.SW = levelMap[currentLocation.x-1][currentLocation.y+1];
		box.SW = grid.getTile(GridLoc(currentLocation.x-1,currentLocation.y+1)).getType();
	else
		box.SW = OVER;
	return box;
}

Space level::getTileType(GridLoc tile)
{
	return grid.getTile(tile).getType();
}

//this finds a lifeForm pointer based off of its location
//meant only to be used by the level object
Lifeform* level::identifyLifeForm(GridLoc pos)
{
	for(int i = 0; i < lifeForms.size(); i++)
	{
		if(lifeForms[i]->getGridLoc().x == pos.x && lifeForms[i]->getGridLoc().y == pos.y)
		{
			return lifeForms[i];
		}
	}
	return 0;
}

Object* level::identifyObject(GridLoc pos)
{
	for(int i = 0; i < objects.size(); i++)
	{
		if(objects[i]->getGridLoc().x == pos.x && objects[i]->getGridLoc().y == pos.y)
		{
			return objects[i];
		}
	}
	return 0;
}

NimkipInfo level::getNimkipInfo(GridLoc pos)
{
	Lifeform* temp = identifyLifeForm(pos);
	Nimkip* nimkip = reinterpret_cast<Nimkip*>(temp);
	return nimkip->getInfo();
}

void level::getUserInput()
{
	//if they are to pick something up make them stop before actually getting to it
	if(grid.getSTask()==LIFT)
	{
		auto location = getSurroundings(grid.getDestination(),1);
		for(int i = 0; i < location.size(); i++)
		{
			if(location[i].type==EMPTY)
			{
				grid.setDestination(location[i]);
				break;
			}
		}
	}
	for(int i = 0; i < grid.getNimkips().size(); i++)
	{
		Lifeform* l = identifyLifeForm(grid.getNimkips()[i]);
		Nimkip* n = reinterpret_cast<Nimkip*>(l);
		if(n)
		{
			n->setPTask(grid.getPTask());
			n->setSTask(grid.getSTask());
			n->setDestination(grid.getDestination());
		}
	}
}

void level::collectCoin(GridLoc coinLocation)
{
	auto stuff = identifyObject(coinLocation);
	if(grid.getTile(coinLocation).getType()==COIN)
	{
		identifyObject(coinLocation)->setVisible(false);
	}
}