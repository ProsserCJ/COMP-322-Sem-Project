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
	this->numKips = RNIMKIP_COUNT + BNIMKIP_COUNT + YNIMKIP_COUNT;
	this->mapSizeX = 50;
	this->mapSizeY = 50;
	this->maxKips = 20;
	this->lose = false;
	this->win = false;
	this->endGame = false;
	frameCount = 0;
	turns = 0;
	foodCollected = 0;
	score=0;
	debug.open("debug.txt");
}


//creating static data
Grid<Tile> level::grid;
int level::numEnemies = RBROBLUB_COUNT + BBROBLUB_COUNT;
vector<Lifeform*> level::lifeForms;
vector<Carriable*> level::carriableItems;
vector<Object*> level::unCarriables;
vector<Horde*> level::hordeForms;




//=============================================================================
// Destructor
//=============================================================================
level::~level()
{
	debug.close();
    for(int i = 0; i < carriableItems.size(); i++) delete carriableItems[i];
	for(int i = 0; i < unCarriables.size(); i++) delete unCarriables[i];
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

	// win screen textures
    if (!winScreenTexture.initialize(graphics,WINSCREEN_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing win screen textures"));

	// game over screen textures
    if (!gameOverScreenTexture.initialize(graphics,GAMEOVER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game over screen textures"));
	
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

	// win screen 
	if(!winScreen.initialize(graphics, 0, 0, 0, &winScreenTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing win screen"));

	// game over screen 
	if(!gameOverScreen.initialize(graphics, 0, 0, 0, &gameOverScreenTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game over screen"));

	
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
			case RB: 
				lifeForms[i]->setCurrentFrame(RBRO); 
				grid.setType(lifeForms[i]->getGridLoc(), BROBLUB);	
				break;	
			case BB: 
				lifeForms[i]->setCurrentFrame(BBRO); 
				grid.setType(lifeForms[i]->getGridLoc(), BROBLUB);	
				break;	
		}
	}

	for(int i = 0; i < carriableItems.size(); i++)
	{
		if (!carriableItems[i]->initialize(this, GRID_SIZE, GRID_SIZE, 3, &gameTextures))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing object"));
		switch(carriableItems[i]->getImage())
		{
		case C:
			carriableItems[i]->setCurrentFrame(MONEY);
			grid.setType(carriableItems[i]->getGridLoc(),COIN);
			break;
		case CH:
			carriableItems[i]->setCurrentFrame(CHICKEN);
			grid.setType(carriableItems[i]->getGridLoc(),CHICK);
			break;
		case CC:
			carriableItems[i]->setCurrentFrame(CANDY);
			grid.setType(carriableItems[i]->getGridLoc(),CANDYCANE);
			break;
		case O:
			carriableItems[i]->setCurrentFrame(ROCK);
			grid.setType(carriableItems[i]->getGridLoc(),OBSTACLE);
			break;
		case G:
			carriableItems[i]->setCurrentFrame(HOLE);
			grid.setType(carriableItems[i]->getGridLoc(),GAP);
			break;
		case BS:
			carriableItems[i]->setCurrentFrame(NIMBASE);
			grid.setType(carriableItems[i]->getGridLoc(),BASE);
			break;
		}
	}
	
	for(int i = 0; i < unCarriables.size(); i++)
	{
		if (!unCarriables[i]->initialize(this, GRID_SIZE, GRID_SIZE, 3, &gameTextures))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing object"));
		switch(unCarriables[i]->getImage())
		{
		case C:
			unCarriables[i]->setCurrentFrame(MONEY);
			grid.setType(unCarriables[i]->getGridLoc(),COIN);
			break;
		case CH:
			unCarriables[i]->setCurrentFrame(CHICKEN);
			grid.setType(unCarriables[i]->getGridLoc(),CHICK);
			break;
		case O:
			unCarriables[i]->setCurrentFrame(ROCK);
			grid.setType(unCarriables[i]->getGridLoc(),OBSTACLE);
			break;
		case G:
			unCarriables[i]->setCurrentFrame(HOLE);
			grid.setType(unCarriables[i]->getGridLoc(),GAP);
			break;
		case BS:
			unCarriables[i]->setCurrentFrame(NIMBASE);
			grid.setType(unCarriables[i]->getGridLoc(),BASE);
			break;
		case S:
			unCarriables[i]->setCurrentFrame(STAT);
			grid.setType(unCarriables[i]->getGridLoc(),STATUE);
			break;
		}
	}

	hud.setX(hudNS::UPPER_X);
	
	gameFont->initialize(graphics, 20, false, false, "Calibri");
	gameFont->setFontColor(SETCOLOR_ARGB(255,0,0,0));

	//misc.
	
	pause();
	audio->playCue(MUSIC);
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

	if (isRunning() && (frameCount%(int(250*grid.getScale())) == 0) && (lifeForms[0]->getDistToMove()==0))
		//^sloppy.  should get a better check for if lifeforms are moving
		runTimeStep();
	

	for(int i=0; i<lifeForms.size(); i++)
		lifeForms[i]->move(frameTime);

	//call zoom and scroll, and pause the action is either is taking place.
	grid.scroll(frameTime);
	if(grid.zoom(frameTime)) pause();


	if(!endGame && score > 1000) {
		endGame = true;
		win = true;
		audio->playCue(WINSOUND);
		audio->stopCue(MUSIC);
		audio->stopCue(COLLECT);
		audio->stopCue(CANDYSOUND);
		audio->stopCue(ATTACKSOUND);
	}
	if(!endGame && numEnemies == 0) {
		endGame = true;
		win = true;
		audio->playCue(WINSOUND);
		audio->stopCue(MUSIC);
		audio->stopCue(COLLECT);
		audio->stopCue(CANDYSOUND);
		audio->stopCue(ATTACKSOUND);
	}
	if(!endGame && numKips == 0) {
		lose = true;
		endGame = true;
		audio->playCue(LOSESOUND);
		audio->stopCue(MUSIC);
		audio->stopCue(COLLECT);
		audio->stopCue(CANDYSOUND);
		audio->stopCue(ATTACKSOUND);
	}

	
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
	for(int i = 0; i < carriableItems.size(); i++)
	{
		carriableItems[i]->draw();
	}
	for(int i = 0; i < unCarriables.size(); i++)
	{
		unCarriables[i]->draw();
	}
	hud.draw();
	gameFont->setFontColor(SETCOLOR_ARGB(255,0,0,0));
	ss.str(std::string());
	ss << "Turn  " << turns << "\nScore " << score << "\nKips " << numKips;
	gameFont->print(ss.str(), 830, 410);

	if(win)
		winScreen.draw();
	if(lose)
		gameOverScreen.draw();


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
			if(lifeForms[i]->getHurtBoolean())
				lifeForms[i]->setHurt();
			//for after they're healed by statue
			if (lifeForms[i]->getHurtBoolean() && lifeForms[i]->getHealth() >= lifeForms[i]->getMaxHealth()*0.3){
				lifeForms[i]->setNormal();
				lifeForms[i]->setHurtBoolean(false);
			}
			grid.setType(lifeForms[i]->getGridLoc(),EMPTY);			
			GridLoc newGridLoc = lifeForms[i]->takeTurn();

			//make sure their supposed new position is not wrong
			//kind of a quick and dirty fix to stop things from accidentily breaking
			//I suspect the issue is in the move function somewhere
			//if(newGridLoc.x>0 && newGridLoc.x < grid.getWidth() && newGridLoc.y>0 && newGridLoc.y < grid.getHeight())
				grid.setType(newGridLoc, lifeForms[i]->getImage());
			
			
			if(!lifeForms[i]->getActive())//if they are dead tell the level map
			{
				grid.setType(lifeForms[i]->getGridLoc(),EMPTY);	
				if (lifeForms[i]->getImage() <= YN) numKips--;
			}
			//check if they scored
			if(lifeForms[i]->getScored())
			{
				score+=lifeForms[i]->getScoredPoints();
				//simple cheat way to check if they collected food just assume that food will be more than 100 points
				//I dont feel like making other functions to check it for real.
				if(lifeForms[i]->getScoredPoints()>100)
				{
					foodCollected++;
				}
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
		receiver->setHeldObject(identifyObject(item));
		if(receiver->getHeldObject()!=0)
			receiver->setHolding(true);
	}
	else
	{
		return false;
	}
	return true;
}

//takes the attacker and the location of the target, and runs the attack
bool level::runAttack(Lifeform* attacker, GridLoc target)
{
	Lifeform* targetLifeForm = identifyLifeForm(target);
	if(targetLifeForm == 0)
		return true;
	if (attacker->isNormal()) attacker->setAtk(); 	
		targetLifeForm->subHealth(attacker->getAttackStrength());
	if (!targetLifeForm->getHurtBoolean() && targetLifeForm->getHealth() < targetLifeForm->getMaxHealth()*0.3){
		targetLifeForm->setHurt();
		targetLifeForm->setHurtBoolean(true);
	}
	if(targetLifeForm->getHealth()<=0 && targetLifeForm->getActive())
	{
		/*if(target.type == BROBLUB)
			level::numEnemies--;*/
		return true;
	}
	return false;
}

//fills level with all necessary starting objects
void level::fillLevel()
{
       Base* b = new Base();      
       b->setGridLoc(10,0);	  
       grid.setType(10,0,BASE);
       unCarriables.push_back(reinterpret_cast<Object*>(b));
	   grid.setObject(b);

	   Random X(0, RNIMKIP_COUNT);
	   Random Y(0, RNIMKIP_COUNT);

	   for(int i=0; i< RNIMKIP_COUNT; i++){
		   RedKip* temp = new RedKip();		   
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   temp->setDestination(GridLoc(x,y));
		   grid.setType(x,y,NIMKIP);
		   lifeForms.push_back(reinterpret_cast<Lifeform*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i< BNIMKIP_COUNT; i++){
		   BlueKip* temp = new BlueKip();
		    int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   temp->setDestination(GridLoc(x,y));
		   grid.setType(x,y,NIMKIP);
		   lifeForms.push_back(reinterpret_cast<Lifeform*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i< YNIMKIP_COUNT; i++){
		   YellowKip* temp = new YellowKip();
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   temp->setDestination(GridLoc(x,y));
		   grid.setType(x,y,NIMKIP);
		   lifeForms.push_back(reinterpret_cast<Lifeform*>(temp));
		   grid.setObject(temp);
	   }

	   X.range(RNIMKIP_COUNT, mapSizeX-1);
	   Y.range(RNIMKIP_COUNT, mapSizeY-1);

	   for(int i=0; i<RBROBLUB_COUNT; i++){
		   Broblub* temp = new RedBroblub();
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   grid.setType(x,y,BROBLUB);
		   lifeForms.push_back(reinterpret_cast<Lifeform*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i<BBROBLUB_COUNT; i++){
		   Broblub* temp = new BlackBroblub();
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   grid.setType(x,y,BROBLUB);
		   lifeForms.push_back(reinterpret_cast<Lifeform*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i<COIN_COUNT; i++){
		   Coin* temp = new Coin();
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x, y);             
		   grid.setType(x,y,COIN);
		   carriableItems.push_back(reinterpret_cast<Carriable*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i<FOOD_COUNT; i++){
		   Chicken* temp = new Chicken();
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   grid.setType(x,y,CHICK);
		   carriableItems.push_back(reinterpret_cast<Carriable*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i<CANDY_COUNT; i++){
		   CandyCane* temp = new CandyCane();
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   grid.setType(x,y,CANDYCANE);
		   carriableItems.push_back(reinterpret_cast<Carriable*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i<STATUE_COUNT; i++){
		   Statue* temp = new Statue();
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}
		   temp->setGridLoc(x,y);
		   grid.setType(x,y,STATUE);
		   unCarriables.push_back(reinterpret_cast<Object*>(temp));
		   grid.setObject(temp);
	   }

	   for(int i=0; i<OBSTACLE_COUNT/2;){
		   int x = X.next(), y = Y.next();
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}		   
		   
		   //horizontal walls
		   for(int j=0; j<MAX_WALL_SIZE; j++){
			    if (grid.getObject(x-j,y)) break;			
				Obstacle* temp = new Obstacle();
				temp->setGridLoc(x-j,y);
			    grid.setType(x-j,y,OBSTACLE);
			    unCarriables.push_back(reinterpret_cast<Object*>(temp));
			    grid.setObject(temp);
				i++;
		   }
		   
		   while(grid.getObject(x,y)){x = X.next(); y = Y.next();}	
		   //vertical walls
		   for(int j=0; j<MAX_WALL_SIZE; j++){
			    if (grid.getObject(x,y-j)) break;			
				Obstacle* temp = new Obstacle();
				temp->setGridLoc(x,y-j);
			    grid.setType(x,y-j,OBSTACLE);
			    unCarriables.push_back(reinterpret_cast<Object*>(temp));
			    grid.setObject(temp);
				i++;
		   }
	   }
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

Carriable* level::identifyObject(GridLoc pos)
{
	for(int i = 0; i < carriableItems.size(); i++)
	{
		if(carriableItems[i]->getGridLoc().x == pos.x && carriableItems[i]->getGridLoc().y == pos.y)
		{
			return carriableItems[i];
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
	//stores the destination location in case they have a food target
	GridLoc target = grid.getDestination();
	if(grid.getSTask()==LIFT)
	{
		//if their goal is to lift something this makes them stop one square short
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
			if(grid.getSTask() == LIFT)
			{
				n->setTarget(target);
			}
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

int level::getGridHeight()
{
	return grid.getHeight();
}

int level::getGridWidth()
{
	return grid.getWidth();
}

void level::createHorde(GridLoc carPos, GridLoc lifeformPos, Carriable * car) {
	//Create new Horde
	Horde* ptr = new Horde(car, identifyLifeForm(lifeformPos));
	hordeForms.push_back(ptr);
	hordeForms.push_back(new Horde(car, identifyLifeForm(lifeformPos)));
	
	//Add to hordeList



	//Turns



}

void level::joinHorde(GridLoc hordePos, GridLoc lifeformPos, Horde * hordePtr) {
	//Get horde



}