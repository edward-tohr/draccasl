// I have no idea what I'm doing. But I won't let that stop me.

//NEXT STEP: Fixed the nonzero X pos causes crash bug. Next up, physics.

#include "constants.h"
#include "draccasl.h"
#include "map.h"
#include "gameobject.h"
#include "buildnumber.h"

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
std::vector<Tile> vectorTiles;
std::vector<Tile> mapTiles;
std::vector<SDL_Rect> vectorCollision;
Map curMap;


DEBUG_T DEBUG = ALL;
const int TILESIZE = 32;
const int VELOCITY_MAX = 4;

// SDL requires int main(int argc char* argv[]). Remember that.
// Also, should make new functions for init, event, loop, render, close.



void init(){
	//Setup phase
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		if (DEBUG >= ERROR){std::cout << "SDL init failed! " << SDL_GetError() << "\n";}
	}
	if (!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
		if (DEBUG >= ERROR){std::cout << "SDL_img init failed! " << SDL_GetError() << "\n";}
	}
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
		if (DEBUG >= ERROR){std::cout << "SDL_mixer init failed! " << SDL_GetError() << "\n";}
	}

	//Load phase

	gWindow = SDL_CreateWindow("jack DANGER strong in: castle of the draculas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W,WINDOW_H, 0);
	if (gWindow == NULL){
		if (DEBUG >= ERROR){std::cout << "gWindow is NULL! 'cause of " << SDL_GetError() << "\n";}
	}

	jackSprite = IMG_Load("jack.png");
	if (!jackSprite){
		if (DEBUG >= ERROR){std::cout << "we let go of jack!";}
	}

	gSurface = IMG_Load("title.png");
	if (!gSurface){
		if (DEBUG >= ERROR){std::cout << "failed to load title screen!";}
	}

	gMusic = Mix_LoadMUS("tocafuge.wav");
	if (gMusic == NULL){
		if (DEBUG >= ERROR){std::cout << "tocafuge.wav refused to load! " << Mix_GetError() << "\n";}
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
	Jack->setXPos(64);
	Jack->setYPos(64);
	Jack->setHealth(10);
	Jack->setMaxHealth(10);
	Jack->setAttack(0);
	Jack->setDefense(0);
	Jack->setType(jack);
	Jack->unkill();
	SDL_SetColorKey(jackSprite,SDL_TRUE,SDL_MapRGB(jackSprite->format,0xFF,0x00,0xFF));
	std::cout << "Jack's color-keyed now.\n";
	Jack->setTexture(SDL_CreateTextureFromSurface(gRenderer,jackSprite));
	std::cout << "Jack's sprite is set.\n";

	Jack -> setCollision(0,0,64,64);

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
	    }
		break;


		case SDLK_BACKSLASH:
		if (DEBUG > NONE){
			currentMap++;
			currentMap %= vectorMaps.size();
			std::cout << "loading map " << currentMap << "...\n";
			loadMap(vectorMaps.at(currentMap));
		}
		break;

		case SDLK_HOME:
		if (DEBUG > NONE) {
			std::cout << "X: " << vectorObjects.at(0).getXPos() << " Y: " << vectorObjects.at(0).getYPos() << "\n";
			std::cout << Jack->getXPos() << " " << Jack->getYPos() << "\n";
		}
		break;

	}
	return false;
	}
	}
	return false;
}

void render(){
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	// clear the renderer.
	if (gameState != title){ //If we're on the title screen, don't draw all this crap.
	//Render terrain
	vectorMaps.at(currentMap).Map::render(&vectorTiles, tileSet->w /TILESIZE);
	//Render Objects
	//Jack->render();
	for (unsigned int i = 0; i < vectorObjects.size();i++){
		vectorObjects.at(i).render();
	}

	} else { //If we are on the title screen, draw that.
		SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);
	}
	//Render to the screen.

	SDL_RenderPresent(gRenderer);

}

void loadMap(Map mapToLoad){
	// We have a map taken from vectorMaps, taken from maps.map.
	// We need a surface/texture of the tileset.
	int tiles = mapToLoad.getTileset();
	std::cout << "tiles = " << mapToLoad.getTileset() << "\n";
	std::string tileName = "tileset_";
	tileName.append(std::to_string(tiles));
	tileName.append(".png");
	std::cout << "tilename = " << tileName << "\n";
	SDL_FreeSurface(tileSet);
	std::cout <<"tileset is freed.\n";
	tileSet = IMG_Load(tileName.c_str());


	if (tileSet == NULL){
		if (DEBUG >= ERROR){std::cout << "Error loading tileset " << tileName <<"!\n";}
	} else {

	if (DEBUG == ALL){std::cout << "Successfully loaded tileset " << tileName <<"!\n";}

	tileTexture = SDL_CreateTextureFromSurface(gRenderer, tileSet);

	// Once we have it loaded, we can slice the tileset into tiles, and store it all in a vector<Tile>.
	int numCols = tileSet->w / TILESIZE;
	int numRows = tileSet->h / TILESIZE;
	Tile currentTile;
		for (int i = 0; i < numRows; i++){
		for (int j = 0; j < numCols; j++){
			currentTile.setRect(j*TILESIZE,i*TILESIZE);
			vectorTiles.push_back(currentTile);
		}
	}
	currentMap = mapToLoad.getID();
	curMap = mapToLoad;
	mapTiles = curMap.getTiles();
	if (DEBUG == ALL){std::cout << "Map ID: " << curMap.getID() << " has " << curMap.getTiles().size() << " tiles innit.\n";}
	// And now we have std::vector<Tile> vectorTiles that contains each individual tile, sorted by tile ID.
}
}

void loop(){

	for (unsigned int i = 0; i < vectorObjects.size(); i++){
		vectorObjects.at(i).beginUpdate();
		vectorCollision.clear();
		// collision detection goes here.
		// let's see, let's see......
		// move collisionBox in the X direction according to velocity.
		SDL_Rect tempRect = vectorObjects.at(i).moveCollider(vectorObjects.at(i).getXVel(),0);
		// get a vector of tiles that have x coordinate + width between collider's x and collider's x + width
		//if (DEBUG == ALL){std::cout << "Temprect is at: " << tempRect.x << "," << tempRect.y << ". Xvel is " << vectorObjects.at(i).getXVel() << "\n";}
        for (unsigned int j = 0; j < mapTiles.size(); j++){
                if (mapTiles.at(j).getID() != 1){
                        if (tempRect.x > mapTiles.at(j).getXPos() && tempRect.x < mapTiles.at(j).getXPos()+TILESIZE){
                            vectorCollision.push_back(mapTiles.at(j).getRect());
                        }
                        if (tempRect.x + tempRect.w > mapTiles.at(j).getXPos() && tempRect.x + tempRect.w < mapTiles.at(j).getXPos() + TILESIZE){
                            vectorCollision.push_back(mapTiles.at(j).getRect());
                        }
                }
        }

		// if vector is empty, great.
		if (!vectorCollision.empty()){
		// if not, check x velocity. If positive, set collider's x equal to smallest x in terrain vector. If negative, set X equal to largest X + width. Set velocity to 0 either way.
		if (vectorObjects.at(i).getXVel() > 0){
            int minx = vectorObjects.at(i).getXPos();
            for (unsigned int j = 0; j < vectorCollision.size(); j++){
                minx = std::min(minx,vectorCollision.at(j).x);
            }
            vectorObjects.at(i).setXPos(minx-vectorObjects.at(i).getWidth());
            vectorObjects.at(i).setXVel(0);
		} else if (vectorObjects.at(i).getXVel() < 0){
		            int maxx = vectorObjects.at(i).getXPos();
            for (unsigned int j = 0; j < vectorCollision.size(); j++){
                maxx = std::max(maxx,vectorCollision.at(j).x);
            }
            vectorObjects.at(i).setXPos(maxx);
            vectorObjects.at(i).setXVel(0);
		}
		}

		vectorCollision.clear();
	    tempRect = vectorObjects.at(i).moveCollider(0,vectorObjects.at(i).getYVel());
		 for (unsigned int j = 0; j < mapTiles.size(); j++){
                if (mapTiles.at(j).getID() != 1){
                        if (tempRect.y > mapTiles.at(j).getYPos() && tempRect.y < mapTiles.at(j).getYPos() + TILESIZE){
                    vectorCollision.push_back(mapTiles.at(j).getRect());
                        }
                        if (tempRect.y + tempRect.h > mapTiles.at(j).getYPos() && tempRect.y + tempRect.h < mapTiles.at(j).getYPos() + TILESIZE){
                            vectorCollision.push_back(mapTiles.at(j).getRect());
                        }
                }
        }

        if(!vectorCollision.empty()){

		if (vectorObjects.at(i).getYVel() > 0){
            int miny = vectorObjects.at(i).getYPos();
            for (unsigned int j = 0; j < vectorCollision.size(); j++){
                miny = std::min(miny,vectorCollision.at(j).y);
            }
            vectorObjects.at(i).setYPos(miny);
            vectorObjects.at(i).setYVel(0);
		} else if (vectorObjects.at(i).getYVel() < 0){
		            int maxy = vectorObjects.at(i).getYPos();
            for (unsigned int j = 0; j < vectorCollision.size(); j++){
                maxy = std::max(maxy,vectorCollision.at(j).y);
            }
            vectorObjects.at(i).setYPos(maxy);
            vectorObjects.at(i).setYVel(0);
		}


		}

		vectorObjects.at(i).collisionUpdate();

	}
	//physics, enemy AI goes here
	// also handle map transitions?



}

void gameStart(){
	//New Game setup goes here. Load map, set up player graphics and such.
	Mix_PlayMusic(gMusic, 1);
	if (DEBUG == NONE){SDL_Delay(3500);}
	Mix_HaltMusic();
	gMusic = Mix_LoadMUS("jack.mid");
	gameState = game;
	nextMap = 1;
	populateMapVector(&vectorMaps);
	//populateObjectVector(&vectorObjects) will work the same way once I do that.
	if (DEBUG == ALL){std::cout << "VectorMaps size = " << vectorMaps.size() << "\nvectorObjects size = " << vectorObjects.size() << ".\n";}
	loadMap(vectorMaps.at(0));
	Mix_PlayMusic(gMusic,-1);
}


int main(int argc, char* argv[]){

    std::cout<< "jack DANGER strong in: castle of the draculas\nBuild Date: " <<__DATE__<<" "<< __TIME__<<"\n";
    std::cout<< __FILE__ <<" last modified: " __TIMESTAMP__ << "\n";
    std::cout<< "Build number: " << BUILDNUMBER_STR << "\n";



	//Turns out that maybe SDL uses argc and argv[] for its own stuff, and sometimes breaks when it's messed up like this?
	// Maybe not. Let's see.
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
		if (gameState == game){ loop();}
		render();
	}
	exit();
	return 0;
}
