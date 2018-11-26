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
