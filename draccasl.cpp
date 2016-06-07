// I have no idea what I'm doing. But I won't let that stop me.

//NEXT STEP: Maps display on-screen properly. Now to get Jack activated with physics and such.

#include "constants.h"
#include "draccasl.h"
#include "map.h"
#include "gameobject.h"

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
std::vector<GameObject> vectorObjects;
std::vector<SDL_Rect> vectorTiles;


DEBUG_T DEBUG = ALL;
const int TILESIZE = 32;

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
	
	gWindow = SDL_CreateWindow("jack DANGER strong in: castle of the draculas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W,WINDOW_H, 0);
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
	
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	
	
	std::cout << "pre-gCamera setting\n";
	gCamera.x = 0;
	std::cout << "gCamera x is set\n";
	gCamera.y = 0;
	std::cout << "gCamera y is set\n";
	gCamera.w = WINDOW_W;
	std::cout << "gCamera w is set\n";
	gCamera.h = WINDOW_H;
	std::cout << "gCamera h is set\n";
	std::cout << "post-gCamera setting\n";
	
	
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
	SDL_SetColorKey(jackSprite,SDL_TRUE,SDL_MapRGB(jackSprite->format,0xFF,0x00,0xFF));
	Jack->setTexture(SDL_CreateTextureFromSurface(gRenderer,jackSprite));
	SDL_Rect *jackCollision;
	jackCollision->x = 0;
	jackCollision->y = 0;
	jackCollision->w = jackSprite->w;
	jackCollision->h = jackSprite->h;
	Jack->setCollision(jackCollision);
	
	if (DEBUG == ALL) {
		std::cout << "Jack: x == " << Jack -> getCollision()-> x << " y == " << Jack -> getCollision()-> y << " w == " << Jack ->getCollision() -> w << " h == " << Jack ->getCollision() -> h << "\n";
	}
	
	vectorObjects.push_back(*Jack);
	
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
		
		case SDLK_UP:
		if (DEBUG > NONE){
			currentMap++;
			currentMap %= vectorMaps.size();
			std::cout << "loading map " << currentMap << "...\n";
			loadMap(vectorMaps.at(currentMap));
			break;
		}
			
	}
	}
	
	return false;
	}
}

void render(){
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	// clear the renderer.
	if (gameState != title){ //If we're on the title screen, don't draw all this crap.
	//Render terrain
	vectorMaps.at(currentMap).render(&vectorTiles, tileSet->w /TILESIZE);
	//Render Objects
	for (int i = 0; i < vectorObjects.size();i++){
		vectorObjects.at(i).render();
	}
	
	} else { //If we are on the title screen, draw that.
		SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);
	}
	//Render to the screen.
	
	SDL_RenderPresent(gRenderer);
	
}

void loadMap(Map currentMap){
	// We have a map taken from vectorMaps, taken from maps.map.
	// We need a surface/texture of the tileset.
	int tiles = currentMap.getTileset();
	std::string tileName = "tileset_";
	tileName.append(std::to_string(tiles));
	tileName.append(".png");
	tileSet = IMG_Load(tileName.c_str());
	
	if (tileSet == NULL){
		if (DEBUG >= ERROR){
			std::cout << "Error loading tileset " << tileName <<"!\n";
		}
	} else {
	
	if (DEBUG == ALL)
		std::cout << "Successfully loaded tileset " << tileName <<"!\n";
	
	tileTexture = SDL_CreateTextureFromSurface(gRenderer, tileSet);
	
	// Once we have it loaded, we can slice the tileset into tiles, and store it all in a vector<SDL_Rect>.
	int numCols = tileSet->w / TILESIZE;
	int numRows = tileSet->h / TILESIZE;
	SDL_Rect currentTile;
	currentTile.w = TILESIZE;
	currentTile.h = TILESIZE;
	
	for (int i = 0; i < numRows; i++){
		for (int j = 0; j < numCols; j++){
			currentTile.x = j * TILESIZE;
			currentTile.y = i * TILESIZE;
			vectorTiles.push_back(currentTile);
		}
	}
	// And now we have std::vector<SDL_Rect> vectorTiles that contains each individual tile, sorted by tile ID.
}
}

void loop(){
	//physics, enemy AI goes here
	// also handle map transitions?
}

void gameStart(){
	//New Game setup goes here. Load map, set up player graphics and such.
	Mix_PlayMusic(gMusic, 1);
	if (DEBUG == NONE)
		SDL_Delay(3500);
	Mix_HaltMusic();
	gMusic = Mix_LoadMUS("jack.mid");
	gameState = game;
	nextMap = 1;
	populateMapVector(&vectorMaps);
	//populateObjectVector(&vectorObjects) will work the same way once I do that.
	if (DEBUG == ALL)
		std::cout << "VectorMaps size = " << vectorMaps.size() << "\nvectorObjects size = " << vectorObjects.size() << ".\n";
	loadMap(vectorMaps.at(0));
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
	
	
	//Turns out that maybe SDL uses argc and argv[] for its own stuff, and sometimes breaks when it's messed up like this?
	/*if (argc == 2){
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
	}*/
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