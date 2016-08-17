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

static const float GRAVITY_V = 0.45; //Gravity value.
const int TILESIZE = 32;
const float VELOCITY_MAX = 8;
const float MOVE_ACCEL = 1.5;
const float DRAG = 0.5;

#endif
