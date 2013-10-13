/*
	Chris Prosser
	COMP 322 - Semester Project
	Object.h defines several abstract base classes
*/

struct Position{
	int x, y;
	Position(int X=0, int Y=0){ x = X; y = Y; }
};

class Object{
public:

	int getX() { return pos.x; }
	int getY() { return pos.y; }
	Position getPos() { return pos; }
	void setX(int x) { pos.x = x; }
	void setY(int y) { pos.y = y; }
	void setPos(int x, int y){ pos.x = x; pos.y = y; }
	void setPos(Position& p) { pos.x = p.x; pos.y = p.y; }

private:
	Position pos;
};

class Lifeform: public Object{
public:

	int getHealth() { return health; }
	void setHealth(int h) { health = h; }
	void subHealth(int diff) { health -= diff; }

	virtual void move(Position& p) = 0;	
	virtual void move(int x, int y) = 0;	
	virtual void attack(Position& p) = 0;
	virtual void attack(int x, int y) = 0;
	virtual void takeTurn() = 0;

private:
	int health, atk;
};

class Carriable: public Object{

	int getWeight() { return weight; }

private:
	int weight;
};
