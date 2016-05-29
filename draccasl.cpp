// I have no idea what I'm doing. But I won't let that stop me.

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "iostream"
#include "string"
#include "sstream"

// Define globals here. Probably bad practice, but eh.

//Let's see... window, obviously, and that needs a renderer. Camera stuff, too? And maybe an enum for game state or current map or something.

SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Surface *gSurface;
SDL_Texture *gTexture;

// SDL requires int main(int argc char* argv[]). Remember that.
int main(int argc, char* argv[]){
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	
	gWindow = SDL_CreateWindow("jack DANGER strong in: castle of the draculas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, NULL);
	if (gWindow == NULL){
		std::cout << "gWindow is NULL! 'cause of " << SDL_GetError() << "\n";
		return 1;
	}
	
	gSurface = IMG_Load("jack.png");
	if (!gSurface){
		std::cout << "we let go of jack! " << SDL_GetError() << "\n";
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	//SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);
	
	SDL_Delay(3000);
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyTexture(gTexture);
	IMG_Quit();
	SDL_Quit();
	return 0;
}