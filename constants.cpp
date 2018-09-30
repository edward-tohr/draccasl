#include "constants.h"
#include "iostream"


void dPrint(DEBUG_T dLvl, std::string msg, bool err = true) {
	if (DEBUG >= dLvl) {
    if (err) {
		std::cerr << "errlog " << dLvl << ": " << msg << std::endl;
    } else {
      std::cout << "errlog " << dLvl << ": " << msg << std::endl;
    }
	}
}

void dPrint(DEBUG_T dLvl, std::string msg) {
	if (DEBUG >= dLvl) {
		std::cerr << "errlog " << dLvl << ": " << msg << std::endl;
	}
}
