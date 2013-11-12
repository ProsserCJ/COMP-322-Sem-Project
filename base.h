/* 
 Group #: Team 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, and Andy Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines the Base class: a wrapper
 for displaying the Base in the upper left corner
*/ 

#ifndef __BASE_H
#define __BASE_H

#include "object.h"

class Base: public Object{
public:
	Base() {image = BS;}

private:
};

#endif