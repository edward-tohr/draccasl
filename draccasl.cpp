// I have no idea what I'm doing. But I won't let that stop me.

#include "draccasl.h"


//Let's see... window, obviously, and that needs a renderer. Camera stuff, too? And maybe an enum for game state or current map or something.

enum gameState_t {
	title,
	game,
	pause,
	cutscene
};

gameState_t gameState = title;

// SDL requires int main(int argc char* argv[]). Remember that.
// Also, should make new functions for init, event, loop, render, close.

void init(){
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
	}
	
	jackSprite = IMG_Load("jack.png");
	if (!jackSprite){
		std::cout << "we let go of jack!";
	}
	
	gSurface = IMG_Load("title.png");
	if (!gSurface){
		std::cout << "failed to load title screen!";
	}
	
	gMusic = Mix_LoadMUS("tocafuge.wav");
	if (gMusic == NULL){
		std::cout << "tocafuge.wav refused to load! " << Mix_GetError() << "\n";
	}
	
	gameState = title;
	
}

void exit(){
		SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyTexture(gTexture);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool event(SDL_Event e){
	while (SDL_PollEvent(&e) != 0) {
		
		if(e.type == SDL_QUIT){
	        return true;	
		}
		
		if (e.key.keysym.sym == SDLK_RETURN){
			if (gameState == title){
			Mix_PlayMusic(gMusic, -1);
	
	SDL_Delay(3000);
	Mix_HaltMusic();
	gMusic = Mix_LoadMUS("jack.mid");
	gameState = game;
			}
			else {
				gameState = pause;
			}	
		}
		return false;
	}
}

void render(){
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);
	SDL_RenderPresent(gRenderer);
	
}

void loop(){
	
}

int main(int argc, char* argv[]){
	
	init();
	bool quit = false;
	SDL_Event e;
	while (!quit){
		quit = event(e);
		loop();
		render();	
	}
	exit();
	return 0;
}