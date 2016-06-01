// I have no idea what I'm doing. But I won't let that stop me.

#include "draccasl.h"
#include "gameobject.h"
#include <vector> //should come along with map.h, but just in case.
#include <iostream>
#include <fstream>


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
	
	gWindow = SDL_CreateWindow("jack DANGER strong in: castle of the draculas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, 0);
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

void loadMapInfo(Map tempMap, std::ifstream map, std::vector<Map>mapVector){
	char inputChar = '0';
	int inputCharAsInt = 0;
	
	map.get(c);
	//do map loading stuff
	//if not eof, loadTileInfo(). else, throw error
}

void loadTileInfo(Map tempMap, std::ifstream map, std::vector<Map>mapVector){
	//do tile loading stuff
	//if not eof, loadEventInfo(). else, throw error
}

void loadEventInfo(Map tempMap, std::ifstream map, std::vector<Map>mapVector){
	//do event loading stuff
	//if not eof, loadMapInfo(). else, exit gracefully
}

void populateMapVector(std::vector<Map>mapVector){
	//craaaaap, this should be split into three subfuctions:
	  // loadMapInfo() grabs the ID, width/height, tileset, and exit data. When it finds a return character, run loadTileInfo(). Throws noTileData error on EOF.
	  // loadTileInfo() grabs a tab-separated list of tiles by ID, and arranges them according to the height/width obtained earlier. Calls loadEventInfo() upon return. Throws noEventData error on EOF.
	  // loadEventInfo() grabs the event ID, type, and X and Y positions from the list. If it encounters another return, runs loadMapInfo(). If it encounters EOF, exits gracefully.
	// if (int)c ==9, it's a tab. if ==10, it's a newline.
	char c;
	std::string input;
	int increment = 0;
	int mapNum = 0;
	int oldMap;
	int entX;
	int entY;
	Map* tempMap = new Map();
	std::ifstream map("maps.map");
	while (map.get(c)){
		//std::cout << static_cast<int>(c) << "\n";
	
	if (!map.eof()){
		//loadMapInfo(tempMap, map, mapVector);

		
		int inChar = static_cast<int>(c);
		if (inChar != 9 && inChar != 10) { //If the current character isn't a tab or newline...
		input += c;
		} else if (inChar == 9) { //once tab is reached...
		switch (increment){
			case 0: //First tab is ID.
			tempMap->setID(std::strtol(input.c_str(),NULL,10));
			//std::cout << "tempMap ID is: " << tempMap->getID() << "\n";
			increment++;
			input.clear();
			break;
			case 1: //Second tab is width in tiles.
			tempMap->setWidth(std::strtol(input.c_str(),NULL,10));
			increment++;
			input.clear();
			break;
			case 2: //Third tab is height in tiles.
			tempMap->setHeight(std::strtol(input.c_str(),NULL,10));
			increment++;
			input.clear();
			break;
			case 3: //Fourth tab is tileset ID.
			tempMap->setTileset(std::strtol(input.c_str(),NULL,10));
			increment++;
			input.clear();
			break;
			default: //Tabs 5 + 3x are old map entrance data. 6 + 3x is player's X from that entrance, 7 + 3x is Y.
			switch ((increment-4) % 3){
				case 0:
				oldMap = std::strtol(input.c_str(),NULL,10);
				input.clear();
				increment++;
				break;
				case 1:
				entX = std::strtol(input.c_str(),NULL,10);
				input.clear();
				increment++;
				break;
				case 2:
				entY = std::strtol(input.c_str(),NULL,10);
				input.clear();
				increment++;
				//Once we have all three members of the struct, we write them.
				tempMap->addEntrance(oldMap,entX,entY);
				std::vector<Map::entrances_t> tempVector = tempMap->getEntrances();
				std::cout << "tempMap stats: \nID: " << tempMap->getID() << "\nWidth: " << tempMap->getWidth() << "\nHeight: " << tempMap->getHeight() << "\nTileset: " << tempMap->getTileset() <<"\n" << "Entrance from map: " << tempVector.at(0).prevMap <<"\nEntrance X: " << tempVector.at(0).x_pos << "\nEntrance Y: "<< tempVector.at(0).y_pos << "\n";
				break;
			    }
		
		    }
	    } else if (inChar == 10){ //Once we hit a newline, things get difficult.
		//break;
		}
	} else {
		std::cout << "\nEOF reached. Closing.\n";
		map.close();		
	}
	}
			
		 
}	
		
	
	


bool event(SDL_Event e){
	// handle keypresses and whatnot.
	while (SDL_PollEvent(&e) != 0) {
		
		if(e.type == SDL_QUIT){
	        return true;	
		}
		
		if (e.key.keysym.sym == SDLK_RETURN){
			if (gameState == title){
				gameStart();
			
			}
			else {
				gameState = pause;
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

void loop(){
	//physics, enemy AI goes here
	// also handle map transitions?
}

void gameStart(){
	//New Game setup goes here. Load map, set up player graphics and such.
	Mix_PlayMusic(gMusic, -1);
	
	SDL_Delay(3000);
	Mix_HaltMusic();
	gMusic = Mix_LoadMUS("jack.mid");
	gameState = game;
	nextMap = 1;
	populateMapVector(vectorMaps);
}

void changeMap(Map oldMap, Map newMap){
	//blank screen, move Jack to coordinates taken from newMap, load newMap's graphics, fade in.
	//or stop rendering Jack, save current screen image, and keep rendering that until the warp is complete.
	
	std::vector<Map::entrances_t> entrances = newMap.getEntrances();
	for (int i = 0; i < entrances.size(); i++){
		if (entrances.at(i).prevMap == oldMap.getID()) {
			Jack->setXPos(entrances.at(i).x_pos);
			Jack->setYPos(entrances.at(i).y_pos);
			currentMap = newMap.getID();
			break;
		}
	}
	if (currentMap == oldMap.getID()){
		std::cout << "ERROR! Could not find map "  << newMap.getID() <<"\n";
	}
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