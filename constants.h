/* 
 Group #: 3 
 Members: Chris Prosser, Jacob Gearhart, Kory Kappel, Andrew Miller
 Course: COMP 322, Advanced Programming 
 Date: 28 October 2013 
 Description: This file is inherited from the skeleton
 provided by Charles Kelly.
*/ 

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 constants.h v1.0

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

//-----------------------------------------------
//                  Constants
//-----------------------------------------------

// window
const char CLASS_NAME[] = "Nimkips";
const char GAME_TITLE[] = "Nimkips";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  900;   // width of game in pixels
const UINT GAME_HEIGHT = 800;               // height of game in pixels

// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float GRID_SIZE = 80.0f;
const float HUD_WIDTH = 100.0f;
const int NUM_ENEMIES = 10;

// graphic images

const char BACKGROUND_IMAGE[] = "source\\pictures\\background.png";
const char TEXTURES_IMAGE[] = "source\\pictures\\textures.png";  // game textures. Bulborp image curtesy of Michael Peterson
const char GRID_IMAGE[] = "source\\pictures\\grid.png"; //game grid
const char HUD_IMAGE[] = "source\\pictures\\hud.png";


// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;       // escape key
const UCHAR ALT_KEY      = VK_MENU;         // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;       // Enter key

const enum {
	BNIM=0, BNIM_HURT=1, BNIM_ATK=2,
	YNIM=3, YNIM_HURT=4, YNIM_ATK=5,
	RNIM=6, RNIM_HURT=7, RNIM_ATK=8,
	BRO=9, BRO_HURT=10, BRO_ATK=11,
	MONEY=12, CHICKEN=13, HOLE=14, ROCK=15, NIMBASE=16
};

#endif