#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
// Three different debug values.
//  NONE will not output any debug info.
//  ERROR will only output error messages.
//  ALL will output all debug messages.


enum DEBUG_T {
	DEBUG_NONE,
	DEBUG_ERROR,
	DEBUG_ALL
};

enum TILE_T { // Tile types.
  TILE_NONE,       // No collision..
  TILE_FLOOR,      // Regular floor.
  TILE_DAMAGE,     // Damages on touch (spikes, etc.)
  TILE_DEATH,      // Insta-kill (lava, bottomless pit, etc.)
  TILE_ICE         // Slippery Floor
};

extern DEBUG_T DEBUG;

float GRAVITY_V = 1.05; //Gravity value.
int TILESIZE = 32;
float VELOCITY_MAX = 12;
float MOVE_ACCEL = 2.4;
float DRAG = 0.7;
float FLOOR_SNAP = GRAVITY_V; // snap to floors that are within this value of projected y position.


#endif
