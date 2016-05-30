#ifndef __draccasl_h__
#define __draccasl_h__

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "iostream"
#include "string"
#include "sstream"

//globals go in header file? Is this also awful?
SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Surface *gSurface;
SDL_Surface *jackSprite;
SDL_Texture *gTexture;
Mix_Music *gMusic;
//gameState_t gameState; //Proooooobably can't define that in a header.


//Prototype functions!

void init();

void exit();

bool event(SDL_Event e);

void render();

void loop();

#endif
//There. Now draccasl files just need to #include draccasl.h. Right?