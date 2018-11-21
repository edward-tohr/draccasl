#include "constants.h"
#include "iostream"

float GRAVITY_V = 1.05;
int TILESIZE = 32;
float VELOCITY_MAX = 12;
float MOVE_ACCEL = 2.4;
float DRAG = 0.7;
float FLOOR_SNAP = GRAVITY_V;


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
