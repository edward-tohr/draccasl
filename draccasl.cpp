// I have no idea what I'm doing. But I won't let that stop me.

//NEXT STEP: Maps are loaded from file properly, so now to display them on-screen.

#include "constants.h"
#include "draccasl.h"
#include "map.h"
#include "gameobject.h"

//See definition in constants.h. \
  Valid values for DEBUG are NONE, ERROR, and ALL.




//Let's see... window, obviously, and that needs a renderer. Camera stuff, too? And maybe an enum for game state or current map or something.

enum gameState_t {
	title,
	game,
	pause,
	cutscene
};

enum gameObject_t {
	jack,
	zombie,
	door
};

gameState_t gameState = title;
GameObject* Jack = new GameObject();
std::vector<Map> vectorMaps;

DEBUG_T DEBUG = NONE;

// SDL requires int main(int argc char* argv[]). Remember that.
// Also, should make new functions for init, event, loop, render, close.



void init(){
	//Setup phase
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		if (DEBUG >= ERROR)
			std::cout << "SDL init failed! " << SDL_GetError() << "\n";
	}
	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
		if (DEBUG >= ERROR)
			std::cout << "SDL_img init failed! " << SDL_GetError() << "\n";
	}
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
		if (DEBUG >= ERROR)
			std::cout << "SDL_mixer init failed! " << SDL_GetError() << "\n";
	}
	
	//Load phase
	
	gWindow = SDL_CreateWindow("jack DANGER strong in: castle of the draculas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, 0);
	if (gWindow == NULL){
		if (DEBUG >= ERROR)
			std::cout << "gWindow is NULL! 'cause of " << SDL_GetError() << "\n";
	}
	
	jackSprite = IMG_Load("jack.png");
	if (!jackSprite){
		if (DEBUG >= ERROR)
			std::cout << "we let go of jack!";
	}
	
	gSurface = IMG_Load("title.png");
	if (!gSurface){
		if (DEBUG >= ERROR)
			std::cout << "failed to load title screen!";
	}
	
	gMusic = Mix_LoadMUS("tocafuge.wav");
	if (gMusic == NULL){
		if (DEBUG >= ERROR)
			std::cout << "tocafuge.wav refused to load! " << Mix_GetError() << "\n";
	}
	
	gameState = title;
	currentMap = 0;
	nextMap = 0;
	Jack->setXPos(0);
	Jack->setYPos(0);
	Jack->setHealth(10);
	Jack->setMaxHealth(10);
	Jack->setAttack(0);
	Jack->setDefense(0);
	Jack->setType(jack);
	Jack->unkill();
	
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
	// handle keypresses and whatnot.
	while (SDL_PollEvent(&e) != 0) {
		
		if(e.type == SDL_QUIT){
	        return true;	
		} else if (e.type == SDL_KEYDOWN){
		switch(e.key.keysym.sym) {
		
		case SDLK_RETURN:
			if (gameState == title){
				gameStart();
			
			}
			else {
				gameState = pause;
			}	
		break;
		
		case SDLK_d:
		if ((SDL_GetModState() & KMOD_CTRL) && (SDL_GetModState() & KMOD_ALT)){
		switch (DEBUG){
			case NONE:
			DEBUG = ERROR;
			break;
			case ERROR:
			DEBUG = ALL;
			break;
			case ALL:
			DEBUG = NONE;
			break;
		}
		std::cout << "Debug level set to " << DEBUG << ".\n";
		break;
	    } else {
			break;
		}
	}
	}
	
	return false;
	}
}

void render(){
	
	//gotta put some code here to render Jack's sprite and such.
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);
	SDL_RenderPresent(gRenderer);
	
}

void drawMap(Map currentMap){
	// Code what draws out the map goes here.
	// Let's see... first, grab the tileset.
	// Then split it up into tiles.
	// Then draw the tiles one at a time until you reach currentMap.getWidth().
	// Move down a row and repeat. 
	// populateMapVector() makes sure that the maps have the right number of tiles for their dimensions, so we don't need to check that here.
	// That should be all we need to draw the maps.
	
	
	
}

void loop(){
	//physics, enemy AI goes here
	// also handle map transitions?
}

void gameStart(){
	//New Game setup goes here. Load map, set up player graphics and such.
	Mix_PlayMusic(gMusic, -1);
	if (DEBUG == NONE)
		SDL_Delay(3000);
	Mix_HaltMusic();
	gMusic = Mix_LoadMUS("jack.mid");
	gameState = game;
	nextMap = 1;
	populateMapVector(vectorMaps);
	Mix_PlayMusic(gMusic,-1);
}

void changeMap(Map oldMap, Map newMap){
	//blank screen, move Jack to coordinates taken from newMap, load newMap's graphics, fade in.
	//or stop rendering Jack, save current screen image, and keep rendering that until the warp is complete.
	
	std::vector<Map::entrances_t> entrances = newMap.getEntrances();
	for (int i = 0; i < entrances.size(); i++){
		if (entrances.at(i).prevMap == oldMap.getID()) {
			Jack->setXPos(entrances.at(i).exitXPos);
			Jack->setYPos(entrances.at(i).exitYPos);
			currentMap = newMap.getID();
			break;
		}
	}
	if (currentMap == oldMap.getID()){
		if (DEBUG >= ERROR)
			std::cout << "ERROR! Could not find map "  << newMap.getID() <<"\n";
	}
}

int main(int argc, char* argv[]){
	
	if (argc == 2){
		if (std::string (argv[1]) == "--DEBUG-ALL"){
			DEBUG = ALL;
		} else if (std::string (argv[1]) == "--DEBUG-ERROR"){
			DEBUG = ERROR;
		} else {
			DEBUG = NONE;
		}
	} else {
		DEBUG = NONE;
	}
	
	std::cout << "Debug level is: ";
	switch (DEBUG){
		case NONE:
		std::cout << "none.\n";
		break;
		case ERROR:
		std::cout << "error messages only.\n";
		break;
		case ALL:
		std::cout << "full debug info.\n";
		break;
	}
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