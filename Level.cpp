#include "Level.h"

vector<vector<Space>> Level::levelMap;

vector<Lifeform*> Level::lifeForms;

vector<Object*> Level::objects;

//constructor for defauls
Level::Level()
{
	//seeds the random number generator for later use throughout the program
	srand(time(0));
	this->numEnemies = 5;
	this->startKips = 10;
	this->mapSizeX = 10;
	this->mapSizeY = 10;
	this->maxKips = 20;
	createEmptyLevel();
	fillLevel();
}

//constructor for specific values
Level::Level(int numEnemies, int numKips, int maxKips, int sizeX, int sizeY)
{
	srand(time(0));
	this->numEnemies = numEnemies;
	this->startKips = numKips;
	this->maxKips = maxKips;
	this->mapSizeX = sizeX;
	this->mapSizeY = sizeY;
	createEmptyLevel();
	fillLevel();
}

//deletes all objects it has, which should in turn call the destructors of those, freeing all memory
Level::~Level()
{
	for(int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	for(int i = 0; i < lifeForms.size(); i++)
	{
		delete lifeForms[i];
	}
}

//fills the levelMap with entirely empty spaces
void Level::createEmptyLevel()
{
	levelMap = vector<vector<Space>>(this->mapSizeX);
	for(int i = 0; i < levelMap.size(); i++)
	{
		levelMap[i] = vector<Space>(this->mapSizeY);
		for(int j = 0; j < levelMap[i].size(); j++)
		{
			levelMap[i][j] = EMPTY;
		}
	}
}

//makes all lifeforms take their turn and uses their new positions to update its map
void Level::runTimeStep()
{
	for(int i = 0; i < lifeForms.size(); i++)
	{
		//changes the levelMap based off of the new location of the lifeForm
		Position oldPosition = lifeForms[i]->getPos();
		Position newPosition = lifeForms[i]->takeTurn();
		levelMap[oldPosition.x][oldPosition.y] = EMPTY;
		if(lifeForms[i]->getImage() == 'N')
			levelMap[newPosition.x][newPosition.y] = NIMKIP;
		else
			levelMap[newPosition.x][newPosition.y] = BROBLUB;
	}
}

//transfers item to a Lifeform
//the Lifeform calls the function sending itself and the location where the item is.
bool Level::transferObject(Lifeform* receiver, Position item)
{
	//only works if the receiver is not already holding something
	if(!receiver->getHolding())
	{
		receiver->setHeldObject(identifyObject(item));
		receiver->setHolding(true);
	}
	else
	{
		return false;
	}
	return true;
}

//right now just drawing in the console window
void Level::drawMap()
{
	for(int i = 0; i < mapSizeX; i++)
	{
		for(int j = 0; j < mapSizeY; j++)
		{
			switch(levelMap[i][j])
			{
			case EMPTY:
				cout << "   |";
				break;
			case NIMKIP:
				cout << " N |";
				break;
			case BROBLUB:
				cout << " B |";
				break;
			case COIN:
				cout << " C |";
				break;
			case FOOD:
				cout << " F |";
				break;
			case OBSTACLE:
				cout << " O |";
				break;
			case GAP:
				cout << " G |";
				break;
			}
		}
		cout << "\n----------------------------------------\n";
	}
}

//takes the attacker and the location of the target, and runs the attack
void Level::runAttack(Lifeform* attacker, Position target)
{
	Lifeform* targetLifeForm = identifyLifeForm(target);
	targetLifeForm->subHealth(attacker->getAttackStrength());
}

//fills level with all necessary starting objects
void Level::fillLevel()
{
	//locations for nimkips not set at default because the use will choose the location
	for(int i = 0; i < startKips; i++)
	{
		Nimkip* p = new Nimkip();
		lifeForms.push_back(reinterpret_cast<Lifeform*>(p));
	}
	//create enemies
	for(int i = 0; i < numEnemies; i++)
	{
		Broblub* b = new Broblub();
		int xLoc, yLoc;
		do
		{
			xLoc = rand() % 10;
			yLoc = rand() % 10;
		}while(levelMap[xLoc][yLoc] != EMPTY);
		b->setX(xLoc);
		b->setY(yLoc);
		levelMap[xLoc][yLoc] = BROBLUB;

		lifeForms.push_back(reinterpret_cast<Lifeform*>(b));
	}

	//chooses a number of coins to display
	int numberOfCoins = 5 + rand()%10;
	//chooses a number of food items
	int foodAmount = 5 + rand()%10;
	int obstacleAmount = 5 + rand()%10;

	//create coins
	for(int i = 0; i < numberOfCoins; i++)
	{
		Coin* c = new Coin();
		int xLoc, yLoc;
		do
		{
			xLoc = rand() % 10;
			yLoc = rand() % 10;
		}while(levelMap[xLoc][yLoc] != EMPTY);
		c->setX(xLoc);
		c->setY(yLoc);
		levelMap[xLoc][yLoc] = COIN;

		objects.push_back(reinterpret_cast<Object*>(c));
	}

	//create food
	for(int i = 0; i < foodAmount; i++)
	{
		Food* f = new Food();
		int xLoc, yLoc;
		do
		{
			xLoc = rand() % 10;
			yLoc = rand() % 10;
		}while(levelMap[xLoc][yLoc] != EMPTY);
		f->setX(xLoc);
		f->setY(yLoc);
		levelMap[xLoc][yLoc] = FOOD;
		objects.push_back(reinterpret_cast<Object*>(f));
	}

	//create obstacles
	for(int i = 0; i < obstacleAmount; i++)
	{
		Obstacle* o = new Obstacle();
		int xLoc, yLoc;
		do
		{
			xLoc = rand() % 10;
			yLoc = rand() % 10;
		}while(levelMap[xLoc][yLoc] != EMPTY);
		o->setX(xLoc);
		o->setY(yLoc);
		levelMap[xLoc][yLoc] = OBSTACLE;

		objects.push_back(reinterpret_cast<Object*>(o));
	}
}

//returns a struct that contains all of the surrounding squares
Surroundings Level::getSurroundings(Position currentLocation)
{
	Surroundings box;
	box.N = levelMap[currentLocation.x][currentLocation.y-1];
	box.S = levelMap[currentLocation.x][currentLocation.y+1];
	box.E = levelMap[currentLocation.x+1][currentLocation.y];
	box.W = levelMap[currentLocation.x-1][currentLocation.y-1];
	box.NE = levelMap[currentLocation.x+1][currentLocation.y-1];
	box.NW = levelMap[currentLocation.x-1][currentLocation.y-1];
	box.SE = levelMap[currentLocation.x+1][currentLocation.y+1];
	box.SW = levelMap[currentLocation.x-1][currentLocation.y+1];

	return box;
}

//this finds a lifeForm pointer based off of its location
//meant only to be used by the Level object
Lifeform* Level::identifyLifeForm(Position pos)
{
	for(int i = 0; i < lifeForms.size(); i++)
	{
		if(lifeForms[i]->getPos().x == pos.x && lifeForms[i]->getPos().y == pos.y)
		{
			return lifeForms[i];
		}
	}
	return 0;
}

Object* Level::identifyObject(Position pos)
{
	for(int i = 0; i < lifeForms.size(); i++)
	{
		if(objects[i]->getPos().x == pos.x && objects[i]->getPos().y == pos.y)
		{
			return objects[i];
		}
	}
	return 0;
}
