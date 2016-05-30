// I have no idea what I'm doing. But I won't let that stop me.

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "iostream"
#include "string"
#include "sstream"

// Define globals here. Probably bad practice, but eh.

//Let's see... window, obviously, and that needs a renderer. Camera stuff, too? And maybe an enum for game state or current map or something.

SDL_Window *gWindow;
SDL_Renderer *gRenderer;
SDL_Surface *gSurface;
SDL_Surface *jackSprite;
SDL_Texture *gTexture;
Mix_Music *gMusic;

// SDL requires int main(int argc char* argv[]). Remember that.
// Also, should make new functions for init, event, loop, render, close.

int init(){
	//Setup phase
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL init failed! " << SDL_GetError() << "\n";
	}
	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
		std::cout << "SDL_img init failed! " << SDL_GetError() << "\n";
	}
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
		std::cout << "SDL_mixer init failed! " << SDL_GetError() << "\n";
	}
	
	//Load phase
	
	gWindow = SDL_CreateWindow("jack DANGER strong in: castle of the draculas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, NULL);
	if (gWindow == NULL){
		std::cout << "gWindow is NULL! 'cause of " << SDL_GetError() << "\n";
		return 1;
	}
	
	jackSprite = IMG_Load("jack.png");
	if (!jackSprite){
		std::cout << "we let go of jack!";
	}
	
	gMusic = Mix_LoadMUS("jack.mid");
	if (gMusic == NULL){
		std::cout << "jack.mid refused to load! " << Mix_GetError() << "\n";
	}
	
}
int main(int argc, char* argv[]){
	
	init();
	
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, jackSprite);
	//SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);
	Mix_PlayMusic(gMusic, -1);
	
	SDL_Delay(30000);
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyTexture(gTexture);
	IMG_Quit();
	SDL_Quit();
	return 0;
}