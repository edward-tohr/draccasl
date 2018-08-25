#include "constants.h"
#include "iostream"

void dPrint(DEBUG_T dLvl, std::string msg) {
	if (DEBUG >= dLvl) {
		std::cout << "errlog lv: " << dLvl << ": " << msg << std::endl;
	}
}
