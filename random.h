/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller 
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file defines a random number generator wrapper
 class written by Chris
*/ 

#include <stdlib.h>
#include <time.h>

class Random{
public:
	Random(int l=0, int h=1){
		srand(time(0));
		range(l, h);		
	}
	//Set range of random numbers (doubly inclusive)
	void range(int l, int h){
		if (l < h){ low = l; high = h; }
		else { low = h; high = l; }
	}

	int next(){ return rand() % ((high-low)+1) + low; }	
	
private:
	
	int low, high;

};