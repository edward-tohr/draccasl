#include "map.h"
int Tile::getID(){
return id;
}

int Tile::getXPos(){
return x_pos;
}

int Tile::getYPos(){
return y_pos;
}

SDL_Rect Tile::getRect(){
    return tileRect;
}

void Tile::setID(int i){
id = i;
}

void Tile::setXPos(int x){
x_pos = x;
}

void Tile::setYPos(int y){
y_pos = y;
}

void Tile::setRect(int x_in, int y_in){
tileRect.x = x_in;
tileRect.y = y_in;
tileRect.w = TILESIZE;
tileRect.h = TILESIZE;
}

int parseMapInfo(std::ifstream &mapData){
	char data[256];
	int parsedData = 0;
	mapData.getline(data,256,'\t');
	if (mapData.gcount() > 0){
	for (int i = 0; i < mapData.gcount()-1; i++){
		parsedData += (data[i]-48) * pow(10,(mapData.gcount()-2) - i);
		// char '0' is value 48, so by subtracting 48 from each char, we can convert to int.
		// the math looks a little hairy, but we're reading a number one at a time, then multiplying it by 10^number of characters read - 1
		// so for single digits we multiply by 10^0, i.e. 1. Since gcount() also counts the tab, we subtract another one from it to deal with that.
		// yes, we get two different off-by-one errors at the same time, in the same direction. yaaaaaaaaaaay.


	}
	return parsedData;
	} else {
		return -1; //Uh-oh, something went wrong.
	}

}

bool loadMapInfo(Map *tempMap, std::ifstream &mapData){
	bool success = true;
	if (DEBUG == ALL){std::cout << "Parsing map ID... ";}
	tempMap->setID(parseMapInfo(mapData));
	if (DEBUG == ALL){
		std::cout << tempMap -> getID() << "\n";
		std::cout << "Parsing map width... ";
		}
	tempMap->setWidth(parseMapInfo(mapData));
	if (DEBUG == ALL){
		std::cout << tempMap -> getWidth() << "\n";
		std::cout << "Parsing map height... ";
	}
	tempMap->setHeight(parseMapInfo(mapData));
	if (DEBUG == ALL){
		std::cout << tempMap -> getHeight() << "\n";
		std::cout << "Parsing map tileset... ";
	}
	tempMap->setTileset(parseMapInfo(mapData));
	if (DEBUG == ALL){std::cout << tempMap -> getTileset() << "\n";}
	if (mapData.peek() != 10) { // If there's not a newline following the map header...
	success = false;
		if (mapData.eof()){
			if (DEBUG >= ERROR){std::cout << "Early EOF reached! Map data only contains header info! \n";}
		}
		if (DEBUG >= ERROR){std::cout << "Map header is not followed by a newline. Map file may need to be recreated.\n";}
	}
	if (DEBUG >= ALL){std::cout << "Post-header, we are at position " << mapData.tellg() << ".\n";}
	return success;
}

bool loadTileInfo(Map *tempMap, std::ifstream &mapData){
	bool success = true;
	Tile tempTile;
	if (DEBUG >= ALL){		std::cout << "loading tile info....\n";}
	if (DEBUG >= ALL){		std::cout << "We are at position " << mapData.tellg() << ".\n";}
	mapData.ignore(5,10); // Skip over the newline character.
	int tilesLoaded = 0;
	int tempX = 0;
	int tempY = 0;
	while (mapData.peek() != 10 && !mapData.eof()){ //Keep loading exit data until you hit a newline.
	tempTile.setID(parseMapInfo(mapData));
	if (tempX > tempMap->getWidth()){
	    tempX = 0;
        tempY++;
	}
	tempTile.setXPos(tempX);
	tempTile.setYPos(tempY);
	tempX++;
	tempMap->addTile(tempTile);

	if (++tilesLoaded > tempMap->getWidth() * tempMap->getHeight()) {
		if (DEBUG >= ERROR){			std::cout << "too many tiles defined for map's listed size!\neither remove tiles or increase map's width and/or height.\n";}
		success = false;
		break;
	}
	}
	if (tilesLoaded < tempMap->getWidth() * tempMap->getHeight()){
		if (DEBUG >= ERROR){std::cout << "too few tiles defined for map's listed size!\neither add tiles or decrease map's width and/or height.\n";}
		success = false;
	}
	if (mapData.eof()) {
		if (DEBUG >= ERROR){			std::cout << "EOF reached after tile info. No events defined.\n";}
		success = false;
	}
	if (DEBUG == ALL) {
		std::cout << "Finished parsing map data for map " << tempMap -> getID() << ":\n";
		for (int i = 0; i < tempMap->getHeight(); i++){
			for (int j = 0; j < tempMap->getWidth(); j++) {
				std::cout << tempMap->getTiles().at((i*tempMap->getWidth()) + j).getID() << " ";
			}
			std::cout << "\n";
		}
	}

	return success;


}

bool loadEventInfo(Map *tempMap, std::ifstream &mapData){
	bool repeat = false;
	if (DEBUG == ALL){std::cout << "loading event info....\n";}
	mapData.ignore(5,10); // Skip over the newline character.
	int eventID = 0;
	int eventXPos = 0;
	int eventYPos = 0;
	bool success = true;
	while (mapData.peek() != 10 && !mapData.eof()){ //Keep loading exit data until you hit a newline or eof.
	success = false;
	if (DEBUG == ALL){std::cout << "Parsing event ID...\n";}
	eventID = parseMapInfo(mapData);
	if (DEBUG == ALL){std::cout << "Parsing event X pos...\n";}
	eventXPos = parseMapInfo(mapData);
	if (DEBUG == ALL){std::cout << "Parsing event Y pos...\n";}
	eventYPos = parseMapInfo(mapData);
	tempMap->addEvent(eventID,eventXPos,eventYPos);
	success = true;
	}

	if (success){
		if (!mapData.eof()){
			repeat = true;
			mapData.ignore(5,10);
		}
	}
	if (DEBUG >= ALL){		std::cout << "Do we repeat? " << repeat << ".\n";}
	return repeat;
}

bool loadExitInfo(Map *tempMap, std::ifstream &mapData){
	bool success = true;
	mapData.ignore(5,10); //ignore a newline within the next 5 characters.
	int exitID = 0;
	int exitXPos = 0;
	int exitYPos = 0;
	while (mapData.peek() != 10 && !mapData.eof()){
	success = false;
	if (DEBUG == ALL){std::cout << "Parsing exit ID...\n";}
	exitID = parseMapInfo(mapData);
	if (DEBUG == ALL){std::cout << "Parsing exit X pos...\n";}
	exitXPos = parseMapInfo(mapData);
	if (DEBUG == ALL){std::cout << "Parsing exit Y pos...\n";}
	exitYPos = parseMapInfo(mapData);
	tempMap->addEntrance(exitID,exitXPos,exitYPos);
	success = true;
	}

	if (!success && DEBUG >= ERROR){
		std::cout << "failed to load map exit data.\n";
		if (mapData.eof()){
			std::cout << "EOF reached in exit data.\n";
		} else {
			std::cout << "Exit data is malformed.\n";
		}
	}

	return success;
}

void populateMapVector(std::vector<Map>* mapVector){
	//craaaaap, this should be split into three subfuctions:
	  // loadMapInfo() grabs the ID, width/height, tileset. Calls loadExitInfo() on return character.
	  // loadExitInfo() grabs exit data. When it finds a return character, run loadTileInfo(). Throws noTileData error on EOF.
	  // loadTileInfo() grabs a tab-separated list of tiles by ID, and arranges them according to the height/width obtained earlier. Calls loadEventInfo() upon return. Throws noEventData error on EOF.
	  // loadEventInfo() grabs the event ID, type, and X and Y positions from the list. If it encounters another return, runs loadMapInfo(). If it encounters EOF, exits gracefully.
	// if (int)c ==9, it's a tab. if ==10, it's a newline.

	Map tempMap;
	std::ifstream mapData("maps.map");
	bool loop = true;
	do {
	if (!loadMapInfo(&tempMap,mapData)) {
		break;
	}
	if (!loadExitInfo(&tempMap,mapData)){
		break;
	}
	if (!loadTileInfo(&tempMap,mapData)){
		break;
	}
	loop = loadEventInfo(&tempMap,mapData);
	(*mapVector).push_back(tempMap);
	tempMap.clearMap();
	if (DEBUG >= ALL){std::cout << "tempMap ID is " << mapVector->back().getID() << ".\n";}
	} while (loop);
	if (DEBUG >= ALL){std::cout << "Number of maps: " << mapVector->size() << ".\n";}
}

void Map::render(std::vector<Tile>* tileVector, int tileWidth) {
//so we've got two vectors, tiles, which contains a bunch of ints, and tileVector, which contains a bunch of SDL_Rects.
// We want to take tileVector.at(tiles.at(i)), slice that rect out of tileSet, and draw it to the screen at the proper coordinates.
// ... and also make sure that we're only rendering stuff what's on-camera.
SDL_Rect curSpot; //The spot on the screen we're rendering to.
SDL_Rect curTile; //The specific tile we're rendering.
curSpot.w = TILESIZE;
curSpot.h = TILESIZE;
curTile.w = TILESIZE;
curTile.h = TILESIZE;
int curTileID = 0; //The ID of the current tile.
// if inside camera bounds


for (int i = 0; i < gCamera.h / TILESIZE; i++){
	for (int j = 0; j < gCamera.w / TILESIZE; j++){
	curSpot.x = j * TILESIZE;
	curSpot.y = i * TILESIZE;
	curTileID = this->getTiles().at((i * this->getWidth()) + j).getID();
	curTile.x = tileVector->at(curTileID).getRect().x;
	curTile.y = tileVector->at(curTileID).getRect().y;

SDL_RenderCopy(gRenderer, tileTexture,&curTile,&curSpot);
}
}

}
