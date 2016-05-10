#ifndef _DRACWIN_H_
#define _DRACWIN_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "iostream"
#include "string"
#include "sstream"
#include "stdio.h"
#include "constants.h"

class DracWin {
	public:
	int winInit();
	
	private:
	SDL_Window* gWindow = NULL;

};
#endif