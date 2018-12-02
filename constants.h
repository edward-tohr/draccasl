#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
// Three different debug values.
//  NONE will not output any debug info.
//  ERROR will only output error messages.
//  ALL will output all debug messages.
#include "string"


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
  TILE_ICE,        // Slippery Floor
  TILE_UNDEFINED   // Tile type not set! Error!
};

enum COLLISION_T {
  COLLISION_SQUARE,
  COLLISION_RAMP_U,
  COLLISION_RAMP_D,
  COLLISION_RAMP_SHALLOW_BD,
  COLLISION_RAMP_SHALLOW_TD,
  COLLISION_RAMP_SHALLOW_BU,
  COLLISION_RAMP_SHALLOW_TU,
  COLLISION_RAMP_STEEP_BD,
  COLLISION_RAMP_STEEP_BU,
  COLLISION_RAMP_STEEP_TD,
  COLLISION_RAMP_STEEP_TU,
  COLLISION_STAIRS_D,
  COLLISION_STAIRS_U,
  COLLISION_STEP_D,
  COLLISION_STEP_U,
  COLLISION_NONE,
  COLLISION_UNDEFINED          // Collision not set! Error!
};

extern DEBUG_T DEBUG;

extern float GRAVITY_V;//Gravity value.
extern const int TILESIZE;
extern float VELOCITY_MAX;
extern float MOVE_ACCEL;
extern float DRAG;
extern float FLOOR_SNAP;// snap to floors that are within this value of projected y position.
extern int STAIR_WIDTH;
extern const COLLISION_T defaultCollision[18];
extern bool LOAD_NEW_MAPS;
void dPrint(DEBUG_T dLvl, std::string msg, bool err, std::string file, int lineNum);
void setupColliders();


#endif
