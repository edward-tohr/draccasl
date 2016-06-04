// I have no idea what I'm doing. But I won't let that stop me.

#include "draccasl.h"
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

int parseMapInfo(std::ifstream &mapData){
	char data[256];
	int parsedData = 0;
	mapData.getline(data,256,'\t');
	if (mapData.gcount() > 0){
	for (int i = 0; i < mapData.gcount()-1; i++){
		
		// char '0' is value 48, so by subtracting 48 from each char, we can convert to int.
		// the math looks a little hairy, but we're reading a number one at a time, then multiplying it by 10^X where X is the number of characters read - 1
		// so for single digits we multiply by 10^0, i.e. 1. Since gcount() also counts the tab delimiter, we subtract another one from it to deal with that.
		// yes, we get two different off-by-one errors at the same time, in the same direction. yaaaaaaaaaaay.
		parsedData += (data[i]-48) * pow(10,(mapData.gcount()-2) - i);
		
	}
	return parsedData;
	} else {
		return -1; //Uh-oh, something went wrong.
	}
	
}

bool loadMapInfo(Map* tempMap, std::ifstream &mapData){
	bool success = true;
	tempMap->setID(parseMapInfo(mapData));
	std::cout << "tempMap->getID() = " << tempMap->getID() << ".\n";
	tempMap->setWidth(parseMapInfo(mapData));
	std::cout << "tempMap->getWidth() = " << tempMap->getWidth() << ".\n";
	tempMap->setHeight(parseMapInfo(mapData));
	std::cout << "tempMap->getHeight() = " << tempMap->getHeight() << ".\n";
	tempMap->setTileset(parseMapInfo(mapData));
	std::cout << "tempMap->getTileset() = " << tempMap->getTileset() << ".\n";
	if (mapData.peek() != 10) { // If there's not a newline following the map header...
	success = false;
		if (mapData.eof()){
			std::cout << "Early EOF reached! Map data only contains header info! \n";
		}
		std::cout << "Map header is not followed by a newline. Map file may need to be recreated.\n";
	} 
	std::cout << "Post-header, we are at position " << mapData.tellg() << ".\n";
	return success;
}

bool loadTileInfo(Map* tempMap, std::ifstream &mapData){
	bool success = true;
	std::cout << "loating tile info....\n";
	mapData.ignore(5,10); // Skip over the newline character.
	int tilesLoaded = 0;
	while (mapData.peek() != 10 && !mapData.eof()){ //Keep loading exit data until you hit a newline.
	std::cout << "Loading tile " << tilesLoaded << " of " << tempMap->getWidth() * tempMap->getHeight() << "...\n";
	tempMap->addTile(parseMapInfo(mapData));
	std::cout << "Wrote tile " << tempMap->getLatestTile() << ".\n";
	if (++tilesLoaded > tempMap->getWidth() * tempMap->getHeight()) {
		std::cout << "too many tiles defined for map's listed size!\n";
		std::cout << "either remove tiles or increase map's width and/or height.\n";
		success = false;
		break;
	}
	}
	if (tilesLoaded < tempMap->getWidth() * tempMap->getHeight()){
		std::cout << "too few tiles defined for map's listed size!\n";
		std::cout << "either add tiles or decrease map's width and/or height.\n";
		success = false;
	}
	if (mapData.eof()) {
		std::cout << "EOF reached after tile info. No events defined.\n";
		success = false;
	}
	
	return success;
	

}

bool loadEventInfo(Map* tempMap, std::ifstream &mapData){
	bool repeat = false;
	std::cout << "loading event info....\n";
	mapData.ignore(5,10); // Skip over the newline character.
	int eventID = 0;
	int eventXPos = 0;
	int eventYPos = 0;
	bool success = true;
	while (mapData.peek() != 10 && !mapData.eof()){ //Keep loading exit data until you hit a newline.
	//Wait wait wait wait wait. This works perfectly fucking fine???
	// but the shit I copy/pasted this from is broken???
	// fuuuuuuuuuuuuuuuuuuuuck.
	success = false;
	eventID = parseMapInfo(mapData);
	std::cout << "eventID is " << eventID << ".\n";
	eventXPos = parseMapInfo(mapData);
	std::cout << "eventXPos is " << eventXPos << ".\n";
	eventYPos = parseMapInfo(mapData);
	std::cout << "eventYPos is " << eventYPos << ".\n";
	tempMap->addEvent(eventID,eventXPos,eventYPos);
	success = true;
	}
	
	if (success){
		if (!mapData.eof()){
			repeat = true;
			mapData.ignore(5,10);
		}
	}
	std::cout << "Do we repeat? " << repeat << ".\n";
	return repeat;
}

bool loadExitInfo(Map* tempMap, std::ifstream &mapData){
	//goodie, there's a bug somewhere in here.
	bool success = true;
	std::cout << "loading exit info....\n";
	std::cout << "Pre-seek, we are at position " << mapData.tellg() << ".\n";
	mapData.ignore(5,10); //ignore a newline within the next 5 characters.
	std::cout << "(post-seek) Exit ID should be " << mapData.peek() - 48 << ".\n";
	std::cout << "Post-seek, we are at position " << mapData.tellg() << ".\n";
	int exitID = 0;
	int exitXPos = 0;
	int exitYPos = 0;
	// For some reason, it's skipping the first proper data point here.
	// neither peek() nor eof() should consume the next bit....
	// and parseMapInfo() works perfectly for everything else....
	std::cout << "(outside loop) Exit ID should be " << mapData.peek() - 48 << ".\n";
	while (mapData.peek() != 10 && !mapData.eof()){
	success = false;
	std::cout << "(inside loop) Exit ID should be " << mapData.peek() - 48 << ".\n";
	exitID = parseMapInfo(mapData);
	std::cout << "exitID is " << exitID << ".\n";
	exitXPos = parseMapInfo(mapData);
	std::cout << "exitXPos is " << exitXPos << ".\n";
	exitYPos = parseMapInfo(mapData);
	std::cout << "exitYPos is " << exitYPos << ".\n";
	tempMap->addEntrance(exitID,exitXPos,exitYPos);
	success = true;
	}
	
	if (!success){
		std::cout << "failed to load map exit data.\n";
		if (mapData.eof()){
			std::cout << "EOF reached in exit data.\n";
		} else {
			std::cout << "Exit data is malformed.\n";
		}
	}
	
	return success;
}

void populateMapVector(std::vector<Map>mapVector){
	//craaaaap, this should be split into three subfuctions:
	  // loadMapInfo() grabs the ID, width/height, tileset. Calls loadExitInfo() on return character.
	  // loadExitInfo() grabs exit data. When it finds a return character, run loadTileInfo(). Throws noTileData error on EOF.
	  // loadTileInfo() grabs a tab-separated list of tiles by ID, and arranges them according to the height/width obtained earlier. Calls loadEventInfo() upon return. Throws noEventData error on EOF.
	  // loadEventInfo() grabs the event ID, type, and X and Y positions from the list. If it encounters another return, runs loadMapInfo(). If it encounters EOF, exits gracefully.
	// if (int)c ==9, it's a tab. if ==10, it's a newline.

	Map* tempMap = new Map();
	std::ifstream mapData("maps.map");
	bool loop = true;
	do {
	if (!loadMapInfo(tempMap,mapData)) {
		break;
	}
	if (!loadExitInfo(tempMap,mapData)){
		break;
	}
	if (!loadTileInfo(tempMap,mapData)){
		break;
	}
	loop = loadEventInfo(tempMap,mapData);
	mapVector.push_back(*tempMap);
	std::cout << "tempMap ID is " << mapVector.back().getID() << ".\n";
	} while (loop);
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
			Jack->setXPos(entrances.at(i).exitXPos);
			Jack->setYPos(entrances.at(i).exitYPos);
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