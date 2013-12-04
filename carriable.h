/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines the Carriable class a derivative of Mover.
 Carriables are any object that can be picked up by a Nimkip. Current
 carriables are Coin and Food.  This file also defines an uncarriable
 Obstacle object.
*/ 

#ifndef __CARRIABLE_H
#define __CARRIABLE_H

#include "mover.h"

class Carriable: public Mover{
public:	
	Carriable(bool multiple)				 {multipleCarriers = multiple; carrierStrength = 0;}
	Carriable()								 {multipleCarriers = false;}
	
	enum Type {CHICK,CAND,COI};

	//Getters
	bool getMultipleCarriers()				 {return multipleCarriers;}	
	int getCarrierStrength()				 {return carrierStrength;}
	int getWeight()							 {return weight;}
	int getPoints()							 {return points;}
	virtual Type getType() = 0;

	//setters
	void setPoints(int p) {points = p;}
	void setWeight(int w) {weight = w;}
	void setMultipleCarriers(bool t) {multipleCarriers = t;}

	//Other
	void changeCarrierStrength(int strength) {carrierStrength+=strength;}

	

protected:
	int points;
	int weight;
	bool multipleCarriers;
	int carrierStrength;
	Type type;
};

class Coin: public Carriable{
public:
	Coin() {this->setImage(C);setPoints(50);type=COI;}
	Type getType() {return type;};
private:
};

class Food: public Carriable{
public:
	Food() : Carriable(true) {}
	virtual void addMove(){return;};
private:
};

class Chicken: public Food{
public:
	Chicken() {this->setImage(CH);setWeight(10);setMultipleCarriers(true);setPoints(200);type=CHICK;}
	virtual void addMove(){return;};
	Type getType() {return type;};
private:
};

class CandyCane: public Food{
	public:
	CandyCane() {this->setImage(CC);setWeight(5);setMultipleCarriers(true);setPoints(800);type=CAND;} //not sure what to set the weight/score to
	virtual void addMove(){return;};
	Type getType() {return type;};
private:
};

class Obstacle: public Object{
public:
	Obstacle() {this->setImage(O);}
	virtual void addMove(){return;};
};

#endif