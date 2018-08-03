#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
// Three different debug values.
//  NONE will not output any debug info.
//  ERROR will only output error messages.
//  ALL will output all debug messages.


enum DEBUG_T {
	NONE,
	ERROR,
	ALL
};

extern DEBUG_T DEBUG;

static const float GRAVITY_V = 1.05; //Gravity value.
const int TILESIZE = 32;
const float VELOCITY_MAX = 12;
const float MOVE_ACCEL = 2.4;
const float DRAG = 0.7;
const float FLOOR_SNAP = GRAVITY_V; // snap to floors that are within this value of projected y position.

#endif
