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
	
	//Getters
	bool getMultipleCarriers()				 {return multipleCarriers;}	
	int getCarrierStrength()				 {return carrierStrength;}
	int getWeight()							 {return weight;}

	//setters
	void setWeight(int w) {weight = w;}
	void setMultipleCarriers(bool t) {multipleCarriers = t;}

	//Other
	void changeCarrierStrength(int strength) {carrierStrength+=strength;}

protected:
	int weight;
	bool multipleCarriers;
	int carrierStrength;
};

class Coin: public Carriable{
public:
	Coin() {this->setImage(C);}
private:
	int points;
};

class Food: public Carriable{
public:
	Food() {this->setImage(F);setWeight(10);setMultipleCarriers(true);}
	virtual void addMove(){return;};
private:
	int points;
};

class Obstacle: public Object{
public:
	Obstacle() {this->setImage(O);}
	virtual void addMove(){return;};
};

#endif