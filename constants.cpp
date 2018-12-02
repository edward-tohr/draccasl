#include "constants.h"
#include "iostream"
#include "sdl_files.h"
#include "vector"


float GRAVITY_V = 1.05;
const int TILESIZE = 32;
float VELOCITY_MAX = 12;
float MOVE_ACCEL = 2.4;
float DRAG = 0.7;
float FLOOR_SNAP = GRAVITY_V;
int STAIR_WIDTH = TILESIZE / 8;
bool LOAD_NEW_MAPS = true;
bool LOG_TO_FILE = false;
bool LOG_ERROR_TO_FILE = false;

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
                                                  // Empty tile would go here, if it needed collision

const COLLISION_T defaultCollision[18] = \
	{COLLISION_SQUARE,COLLISION_NONE,COLLISION_RAMP_U,COLLISION_RAMP_D,COLLISION_RAMP_SHALLOW_TD,COLLISION_RAMP_SHALLOW_BD,\
	COLLISION_RAMP_SHALLOW_BU,COLLISION_RAMP_SHALLOW_TU,COLLISION_RAMP_STEEP_TU,COLLISION_RAMP_STEEP_TD,COLLISION_SQUARE,\
	COLLISION_NONE,COLLISION_NONE,COLLISION_NONE,COLLISION_NONE,COLLISION_NONE,COLLISION_RAMP_STEEP_BU,COLLISION_RAMP_STEEP_BD};



void dPrint(DEBUG_T dLvl, std::string msg, bool err, std::string file = __FILE__, int lineNum = __LINE__) {
	std::string line = std::to_string(lineNum);
	if (DEBUG >= dLvl) {
	std::cout << "Message from " << file << " line " << line << std::endl;
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
SDL_Rect* tempRect = new SDL_Rect(); // So this is the crashy line???
dPrint(DEBUG_ALL,"setting square...",false,__FILE__,__LINE__);
tempRect->w = TILESIZE;
tempRect->h = TILESIZE;
tempRect->x = 0;
tempRect->y = TILESIZE;
TILE_COLLISION_SQUARE.push_back(*tempRect);
dPrint(DEBUG_ALL,"Square set",false,__FILE__,__LINE__);

// Ramp up should have TILESIZE boxen, 1 px width, height = x coord
tempRect->w = 1;

for (int i = 0; i < TILESIZE; i++) {
  tempRect->x = i;	
	tempRect->h = i;
	tempRect->y = i;
	TILE_COLLISION_RAMP_U.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"Upramp set",false,__FILE__,__LINE__);

// Ramp down should have TILESIZE boxen, 1 px width, height = TILESIZE - x coord
for (int i = 0; i < TILESIZE; i++) {
	tempRect->x = i;
	tempRect->y = TILESIZE - i;
	tempRect->h = TILESIZE - i;
	TILE_COLLISION_RAMP_D.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"Downramp set",false,__FILE__,__LINE__);
// Steep ramp up bottom should have TILESIZE/2 boxen, most 1 px, last TILESIZE/2 width, height = min (x coord*2,TILESIZE)
for (int i = 0; i < TILESIZE/2; i++) {
	tempRect->x = i;
	tempRect->y = std::min(i*2,TILESIZE);
	tempRect->h = tempRect->y;
	TILE_COLLISION_RAMP_STEEP_BU.push_back(*tempRect);	
}

tempRect->x = TILESIZE/2;
tempRect->w = TILESIZE/2;
tempRect->h = TILESIZE;
tempRect->y = TILESIZE;
TILE_COLLISION_RAMP_STEEP_BU.push_back(*tempRect);
dPrint(DEBUG_ALL,"Steep rampup bottom set",false,__FILE__,__LINE__);
// Steep ramp up top should have TILESIZE/2 boxen, most 1 px, first TILESIZE/2 width, height = (TILESIZE/2 - x coord) * 2
tempRect->w = 1;
for (int i = 0; i < TILESIZE/2; i++) {
tempRect->x = TILESIZE/2 + i;
tempRect->y = i*2;
tempRect->h = i*2;
TILE_COLLISION_RAMP_STEEP_TU.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"Steep rampup top set",false,__FILE__,__LINE__);

// Steep ramp down bottom  should have TILESIZE/2 boxen, most 1 px, first TILESIZE/2 width, height = TILESIZE - ((TILESIZE/2 - x coord) *2)
for (int i = 0; i < TILESIZE/2; i++) {
	tempRect->x = TILESIZE/2 +i;
	tempRect->y = TILESIZE - (2*i);
	tempRect->h = TILESIZE - (2*i);
	TILE_COLLISION_RAMP_STEEP_BD.push_back(*tempRect);
}
tempRect->x = 0;
tempRect->y = TILESIZE;
tempRect->h = TILESIZE;
tempRect->w = TILESIZE/2;
TILE_COLLISION_RAMP_STEEP_BD.push_back(*tempRect);
dPrint(DEBUG_ALL,"Steep rampup bottom set",false,__FILE__,__LINE__);
// Steep ramp down top should have TILESIZE/2 boxen, most 1 px, last TILESIZE/2 width, height = min(TILESIZE-x coord*2,0)
tempRect->w = 1;
for (int i = 0; i < TILESIZE/2; i++) {
	tempRect->x = i;
	tempRect->y = TILESIZE - (2*i);
	tempRect->h = TILESIZE - (2*i);
	TILE_COLLISION_RAMP_STEEP_TD.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"Steep rampdown top set",false,__FILE__,__LINE__);

// Shallow ramp up bottom should have TILESIZE/2 boxen, 2 px width, height = x coord/2
tempRect->w = 2;
for (int i = 0; i < TILESIZE; i += 2) {
	tempRect->x = i;
	tempRect->y = i/2;
	tempRect->h = i/2;
	TILE_COLLISION_RAMP_SHALLOW_BU.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"shallow rampup bottom set",false,__FILE__,__LINE__);
// Shallow ramp up top should have TILESIZE/2 boxen, 2 px width, height = (TILESIZE + x coord)/2

for (int i = 0; i < TILESIZE; i+=2) {
	tempRect->x = i;
	tempRect->y = (TILESIZE + i)/2;
	tempRect->h = (TILESIZE + i)/2;
	TILE_COLLISION_RAMP_SHALLOW_TU.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"Shallow rampup top set",false,__FILE__,__LINE__);
// Shallow ramp down bottom should have TILESIZE/2 boxen, 2 px width, height = (TILESIZE-x coord) /2

for (int i = 0; i < TILESIZE; i += 2){
	tempRect->x = i;
	tempRect->y = TILESIZE - (i/2);
	tempRect->h = TILESIZE - (i/2);
	TILE_COLLISION_RAMP_SHALLOW_BD.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"shallow rampdown bottom set",false,__FILE__,__LINE__);
// Shallow ramp down top should have TILESIZE/2 boxen, 2 px width, height = TILESIZE - (xcoord /2)

for (int i = 0; i < TILESIZE; i += 2) {
	tempRect->x = i;
	tempRect->y = (TILESIZE - i) /2;
	tempRect->h = (TILESIZE - i) /2;
	TILE_COLLISION_RAMP_SHALLOW_TD.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"shallow rampdown top set",false,__FILE__,__LINE__);
// Up stairs should have TILESIZE/4 boxen, 4 px width, height = ((x coord /4) +1) *4
tempRect->w = STAIR_WIDTH;

for (int i = 0; i < TILESIZE; i+= STAIR_WIDTH){
	tempRect->x = i;
	tempRect->y = ((i/4)+1) *4;
	tempRect->h = tempRect->y;
	TILE_COLLISION_STAIRS_U.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"Stairs up set",false,__FILE__,__LINE__);
// Down stairs should have TILESIZE/4 boxen, 4 px width, height = TILESIZE - ((x coord /4) +1) *4
//TODO: double-check this one, as I don't think the math's right.
for (int i = 0; i < TILESIZE; i += STAIR_WIDTH){
	tempRect->x = i;
	tempRect->y = TILESIZE - (((i/4)+1) *4);
	tempRect->h = tempRect->y;
	TILE_COLLISION_STAIRS_D.push_back(*tempRect);
}
dPrint(DEBUG_ALL,"Stairs down set",false,__FILE__,__LINE__);

// Up step should have 2 boxen, TILESIZE/2 px width, first height TILESIZE/2, second height TILESIZE.
tempRect->w = TILESIZE/2;
tempRect->x = 0;
tempRect->y = TILESIZE/2;
tempRect->h = TILESIZE/2;
TILE_COLLISION_STEP_U.push_back(*tempRect);
tempRect->x = TILESIZE/2;
tempRect->y = TILESIZE;
tempRect->h = TILESIZE;
TILE_COLLISION_STEP_U.push_back(*tempRect);
dPrint(DEBUG_ALL,"Step up set",false,__FILE__,__LINE__);
// Down step should have 2 boxen, TILESIZE/2 px width, first height TILESIZE, second height TILESIZE/2.
tempRect->x = 0;
tempRect->y = TILESIZE;
tempRect->h = TILESIZE;
TILE_COLLISION_STEP_D.push_back(*tempRect);
tempRect->x = TILESIZE/2;
tempRect->y = TILESIZE/2;
tempRect->h = TILESIZE/2;
TILE_COLLISION_STEP_D.push_back(*tempRect);
dPrint(DEBUG_ALL,"Step down set",false,__FILE__,__LINE__);
}

void createFiles() {
	// create the default map and tileset files for collision, etc.

}