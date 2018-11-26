#include "constants.h"
#include "iostream"
#include "sdl_files.h"
#include "vector"


float GRAVITY_V = 1.05;
int TILESIZE = 32;
float VELOCITY_MAX = 12;
float MOVE_ACCEL = 2.4;
float DRAG = 0.7;
float FLOOR_SNAP = GRAVITY_V;

using std::vector;

vector<SDL_Rect> TILE_COLLISION_SQUARE;           // Full squares
vector<SDL_Rect> TILE_COLLISION_RAMP_U;           // 45-degree ramp, up to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_D;           // 45-degree ramp, down to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_STEEP_BU;    // 68-degree ramp, bottom, up to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_STEEP_TU;    // 68-degreen ramp, top, up to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_STEEP_BD;    // 68-degree ramp, bottom, down to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_STEEP_TD;    // 68-degree ramp, top, down to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_SHALLOW_BU;  // 23-degree ramp, bottom, up to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_SHALLOW_TU;  // 23-degree ramp, top, up to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_SHALLOW_BD;  // 23-degree ramp, bottom, down to the right
vector<SDL_Rect> TILE_COLLISION_RAMP_SHALLOW_TD;  // 23-degree ramp, top, down to the right
vector<SDL_Rect> TILE_COLLISION_STAIRS_U;         // Staircase, up to the right
vector<SDL_Rect> TILE_COLLISION_STAIRS_D;         // Staircase, down to the right
vector<SDL_Rect> TILE_COLLISION_STEP_U;           // Single step, up to the right
vector<SDL_Rect> TILE_COLLISION_STEP_D;           // Single step, down to the right



void dPrint(DEBUG_T dLvl, std::string msg, bool err) {
	if (DEBUG >= dLvl) {
    if (err) {
		std::cerr << "errlog " << dLvl << ": " << msg << std::endl;
    } else {
      std::cout << "errlog " << dLvl << ": " << msg << std::endl;
    }
	}
}
/*
void dPrint(DEBUG_T dLvl, std::string msg) {
	if (DEBUG >= dLvl) {
		std::cerr << "errlog " << dLvl << ": " << msg << std::endl;
	} 
	
}
*/
void setupColliders() {
// And now to define the hitboxen for each collider... Oh boy.

// Square should just have the one hitbox taking up the whole tile
SDL_Rect* tempRect = new SDL_Rect();
tempRect->w = TILESIZE;
tempRect->h = TILESIZE;
TILE_COLLISION_SQUARE.push_back(*tempRect);

// Ramp up should have TILESIZE boxen, 1 px width, height = x coord

// Ramp down should have TILESIZE boxen, 1 px width, height = TILESIZE - x coord

// Steep ramp up bottom should have TILESIZE/2 boxen, most 1 px, last TILESIZE/2 width, height = min (x coord*2,TILESIZE)

// Steep ramp up top should have TILESIZE/2 boxen, most 1 px, first TILESIZE/2 width, height = (TILESIZE/2 - x coord) * 2

// Steep ramp down bottom  should have TILESIZE/2 boxen, most 1 px, first TILESIZE/2 width, height = TILESIZE - ((TILESIZE/2 - x coord) *2)

// Steep ramp down top should have TILESIZE/2 boxen, most 1 px, last TILESIZE/2 width, height = min(TILESIZE-x coord*2,0)

// Shallow ramp up bottom should have TILESIZE/2 boxen, 2 px width, height = x coord/2

// Shallow ramp up top should have TILESIZE/2 boxen, 2 px width, height = (TILESIZE + x coord)/2

// Shallow ramp down bottom should have TILESIZE/2 boxen, 2 px width, height = (TILESIZE-x coord) /2

// Shallow ramp down top should have TILESIZE/2 boxen, 2 px width, height = TILESIZE - (xcoord /2)

// Up stairs should have TILESIZE/4 boxen, 4 px width, height = ((x coord /4) +1) *4

// Down stairs should have TILESIZE/4 boxen, 4 px width, height = TILESIZE - ((x coord /4) +1) *4

// Up step should have 2 boxen, TILESIZE/2 px width, first height TILESIZE/2, second height TILESIZE.

// Down step should have 2 boxen, TILESIZE/2 px width, first height TILESIZE, second height TILESIZE/2.

}