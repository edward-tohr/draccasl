#include "map.h"

using std::ifstream;
using std::ofstream;
using std::cout;
using std::vector;

void Tile::setCollisionType(COLLISION_T collType) {
	switch (collType) {
		
		case COLLISION_SQUARE:
		setColliders(TILE_COLLISION_SQUARE);
		break;

		case COLLISION_RAMP_D:
		setColliders(TILE_COLLISION_RAMP_D);
		break;

		case COLLISION_RAMP_SHALLOW_BD:
		setColliders(TILE_COLLISION_RAMP_SHALLOW_BD);
		break;

		case COLLISION_RAMP_SHALLOW_BU:
		setColliders(TILE_COLLISION_RAMP_SHALLOW_BU);
		break;

		case COLLISION_RAMP_SHALLOW_TD:
		setColliders(TILE_COLLISION_RAMP_SHALLOW_TD);
		break;

		case COLLISION_RAMP_SHALLOW_TU:
		setColliders(TILE_COLLISION_RAMP_SHALLOW_TU);
		break;

		case COLLISION_RAMP_STEEP_BD:
		setColliders(TILE_COLLISION_RAMP_STEEP_BD);
		break;

		case COLLISION_RAMP_STEEP_TD:
		setColliders(TILE_COLLISION_RAMP_STEEP_TD);
		break;

		case COLLISION_RAMP_STEEP_TU:
		setColliders(TILE_COLLISION_RAMP_STEEP_TU);
		break;

		case COLLISION_RAMP_U:
		setColliders(TILE_COLLISION_RAMP_U);
		break;

		case COLLISION_STAIRS_D:
		setColliders(TILE_COLLISION_STAIRS_D);
		break;

		case COLLISION_STAIRS_U:
		setColliders(TILE_COLLISION_STAIRS_U);
		break;

		case COLLISION_STEP_D:
		setColliders(TILE_COLLISION_STEP_D);
		break;

		case COLLISION_STEP_U:
		setColliders(TILE_COLLISION_STEP_U);
		break;

		default:
		break;
	}
}

int Tile::getID() {
	return id;
}

int Tile::getXPos() {
	return tileRect.x;
}

int Tile::getYPos() {
	return tileRect.y;
}

SDL_Rect Tile::getRect() {
	return tileRect;
}

void Tile::setID(int i) {
	id = i;
}

void Tile::setXPos(int x) {
	tileRect.x = x;
}

void Tile::setYPos(int y) {
	tileRect.y = y;
}

void Tile::setRect(int x_in, int y_in) {
	tileRect.x = x_in;
	tileRect.y = y_in;
	tileRect.w = TILESIZE;
	tileRect.h = TILESIZE;
}

TILE_T Tile::getType(){
  return type;
}
/*
float Tile::getNormal(){
  return normal;
}*/

void Tile::setType(TILE_T t_type) {
  type = t_type;
}

COLLISION_T Tile::getCollision() {
	return collision;
}

void Tile::setCollision(COLLISION_T t_collision) {
	collision = t_collision;
}

std::vector<SDL_Rect> Tile::getColliders() {
	return vectorColliders;
}

void Tile::setColliders(std::vector<SDL_Rect> colliders) {
	vectorColliders = colliders;
}
/*
void Tile::setNormal(float norm) {
  normal = norm;
}
*/
Tile::Tile() {
	setRect(0,0);
	setID(0);
}

int parseMapInfo(ifstream &mapData) {
	char data[256];
	int parsedData = 0;
	mapData.getline(data,256,'\t');
	if (mapData.gcount() > 0) {
		for (int i = 0; i < mapData.gcount()-1; i++) {
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



bool loadMapInfo(Map *tempMap, ifstream &mapData) {
	bool success = true;
	dPrint(DEBUG_ALL,"Parsing map ID... ",false,__FILE__,__LINE__);

	tempMap->setID(parseMapInfo(mapData));
	
	dPrint(DEBUG_ALL,std::to_string(tempMap -> getID()), false,__FILE__,__LINE__);
	dPrint(DEBUG_ALL,"Parsing map width... ",false,__FILE__,__LINE__);
	
	tempMap->setWidth(parseMapInfo(mapData));
	
	
	dPrint(DEBUG_ALL,std::to_string(tempMap -> getWidth()),false,__FILE__,__LINE__);
	dPrint(DEBUG_ALL,"Parsing map height... ",false,__FILE__,__LINE__);
	
	tempMap->setHeight(parseMapInfo(mapData));
	
	dPrint(DEBUG_ALL,std::to_string(tempMap -> getHeight()),false,__FILE__,__LINE__);
	dPrint(DEBUG_ALL,"Parsing map tileset... ",false,__FILE__,__LINE__);
	
	tempMap->setTileset(parseMapInfo(mapData));
	
	dPrint(DEBUG_ALL,std::to_string(tempMap -> getTileset()),false,__FILE__,__LINE__);
	
	if (mapData.peek() != 10) { // If there's not a newline following the map header...
		success = false;
		if (mapData.eof()) {
				dPrint(DEBUG_ERROR,"Early EOF reached! Map data only contains header info!",true,__FILE__,__LINE__);
		}

		dPrint(DEBUG_ERROR,"Map header is not followed by a newline. Map file may need to be recreated.",true,__FILE__,__LINE__);
		
	}
	
		dPrint(DEBUG_ALL, "Post-header, we are at position " + std::to_string(mapData.tellg()),false,__FILE__,__LINE__);
	
	return success;
}



bool loadTileInfo(Map *tempMap, ifstream &mapData) {
	bool success = true;
	Tile tempTile;
	dPrint(DEBUG_ALL,"loading tile info....",false,__FILE__,__LINE__);
	dPrint(DEBUG_ALL,"We are at position " + std::to_string(mapData.tellg()) ,false,__FILE__,__LINE__);
	
	mapData.ignore(5,10); // Skip over the newline character.
	int tilesLoaded = 0;
	int tempX = 0;
	int tempY = 0;
	while (mapData.peek() != 10 && !mapData.eof()) { //Keep loading exit data until you hit a newline.
		tempTile.setID(parseMapInfo(mapData));
		if (tempX >= tempMap->getWidth()) {
			tempX = 0;
			tempY++;
		}
		tempTile.setXPos(tempX * TILESIZE);
		tempTile.setYPos(tempY * TILESIZE);
		tempX++;
		tempMap->addTile(tempTile);

		if (++tilesLoaded > tempMap->getWidth() * tempMap->getHeight()) {
			dPrint(DEBUG_ERROR,"too many tiles defined for map's listed size!\n"\
					"either remove tiles or increase map's width and/or height.",true,__FILE__,__LINE__);
			success = false;
			break;
		}
	}
	
	if (tilesLoaded < tempMap->getWidth() * tempMap->getHeight()) {
		dPrint(DEBUG_ERROR,"too few tiles defined for map's listed size!\neither add tiles or decrease "\
				"map's width and/or height.",true,__FILE__,__LINE__);
		success = false;
	}

	if (mapData.eof()) {
		dPrint(DEBUG_ERROR,"EOF reached after tile info. No events defined.\n",true,__FILE__,__LINE__);
		success = false;
	}

	if (DEBUG == DEBUG_ALL) { // Yeah, this dPrint can be a special case.
		cout << "Finished parsing map data for map " << tempMap -> getID() << ":\n";
		for (int i = 0; i < tempMap->getHeight(); i++) {
			for (int j = 0; j < tempMap->getWidth(); j++) {
				cout << tempMap->getTiles().at((i*tempMap->getWidth()) + j).getID() << " ";
			}
			cout << "\n";
		}
	}

	return success;
}



bool loadEventInfo(Map *tempMap, ifstream &mapData) {
	bool repeat = false;
	
	dPrint(DEBUG_ALL,"loading event info....",false,__FILE__,__LINE__);
	
	mapData.ignore(5,10); // Skip over the newline character.
	int eventID = 0;
	int eventXPos = 0;
	int eventYPos = 0;
	bool success = true;
	while (mapData.peek() != 10 && !mapData.eof()) { //Keep loading exit data until you hit a newline or eof.
		success = false;
		dPrint(DEBUG_ALL,"Parsing event ID...",false,__FILE__,__LINE__);
		
		eventID = parseMapInfo(mapData);
		
		dPrint(DEBUG_ALL,"Parsing event X pos...",false,__FILE__,__LINE__);
		
		eventXPos = parseMapInfo(mapData);
		
		dPrint(DEBUG_ALL,"Parsing event Y pos...",false,__FILE__,__LINE__);
		
		eventYPos = parseMapInfo(mapData);
		tempMap->addEvent(eventID,eventXPos,eventYPos);
		success = true;
	}

	if (success) {
		if (!mapData.eof()) {
			repeat = true;
			mapData.ignore(5,10);
		}
	}
	
	dPrint(DEBUG_ALL,"Do we repeat? " + std::to_string(repeat) +".",false,__FILE__,__LINE__);
	
	return repeat;
}



bool loadExitInfo(Map *tempMap, ifstream &mapData) {
	bool success = true;
	mapData.ignore(5,10); //ignore a newline within the next 5 characters.
	int exitID = 0;
	int exitXPos = 0;
	int exitYPos = 0;
	while (mapData.peek() != 10 && !mapData.eof()) {
		success = false;

		dPrint(DEBUG_ALL,"Parsing exit ID...",false,__FILE__,__LINE__);
		
		exitID = parseMapInfo(mapData);

		dPrint(DEBUG_ALL,"Parsing exit X pos...",false,__FILE__,__LINE__);
		
		exitXPos = parseMapInfo(mapData);

		dPrint(DEBUG_ALL,"Parsing exit Y pos...",false,__FILE__,__LINE__);
		
		exitYPos = parseMapInfo(mapData);
		tempMap->addEntrance(exitID,exitXPos,exitYPos);
		success = true;
	}

	if (!success) {
		dPrint(DEBUG_ERROR,"failed to load map exit data.",true,__FILE__,__LINE__);
		if (mapData.eof()) {
			dPrint(DEBUG_ERROR,"EOF reached in exit data.",true,__FILE__,__LINE__);
		} else {
			dPrint(DEBUG_ERROR,"Exit data is malformed.",true,__FILE__,__LINE__);
		}
	}

	return success;
}



void populateMapVector(vector<Map>* mapVector) {
	//craaaaap, this should be split into three subfuctions:
	// loadMapInfo() grabs the ID, width/height, tileset. Calls loadExitInfo() on return character.
	// loadExitInfo() grabs exit data. When it finds a return character, run loadTileInfo(). Throws noTileData error on EOF.
	// loadTileInfo() grabs a tab-separated list of tiles by ID, and arranges them according to the height/width obtained earlier. Calls loadEventInfo() upon return. Throws noEventData error on EOF.
	// loadEventInfo() grabs the event ID, type, and X and Y positions from the list. If it encounters another return, runs loadMapInfo(). If it encounters EOF, exits gracefully.
	// if (int)c ==9, it's a tab. if ==10, it's a newline.

	Map tempMap;
	ifstream mapData;
	if (!LOAD_NEW_MAPS) {
	mapData.open("maps.map");
	} else {
	mapData.open("maps.nmp");
	}
	bool loop = true;
	int id = 0;
	if (!LOAD_NEW_MAPS) {
	do {
		if (!loadMapInfo(&tempMap,mapData)) {
			break;
		}
		if (!loadExitInfo(&tempMap,mapData)) {
			break;
		}
		if (!loadTileInfo(&tempMap,mapData)) {
			break;
		}
		loop = loadEventInfo(&tempMap,mapData);
		mapVector->push_back(tempMap);
		tempMap.clearMap();
		
		dPrint(DEBUG_ALL,"tempMap ID is " + std::to_string(mapVector->back().getID()),false,__FILE__,__LINE__);
		if (tempMap.getID() != id) {
			dPrint(DEBUG_ERROR,"Map ID mismatch! Should be " + std::to_string(id) + " but says it's " +\
			  std::to_string(tempMap.getID()) + "!\n",true,__FILE__,__LINE__);
		}

		id++;
	} while (loop);
	} else {
		do {
			loop = newLoadMapInfo(&tempMap,mapData);
			mapVector->push_back(tempMap);
			dPrint(DEBUG_ALL,"map pushed to vector.",false,__FILE__,__LINE__);	
			tempMap.clearMap();
			dPrint(DEBUG_ALL,"tempMap ID is " + std::to_string(mapVector->back().getID()),false,__FILE__,__LINE__);
			if (tempMap.getID() != id) {
				dPrint(DEBUG_ERROR,"Map ID mismatch! Should be " + std::to_string(id) + " but says it's " +\
			  			std::to_string(tempMap.getID()) + "!\n",true,__FILE__,__LINE__);
			  	mapVector->erase(mapVector->end());
			} 
		
			id++;
		} while (loop);
	}
	dPrint(DEBUG_ALL,"Number of maps: " + std::to_string(mapVector->size()),false,__FILE__,__LINE__);
	
}

void Map::render(vector<Tile>* tileVector) {
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


	for (int i = 0; i < gCamera.h / TILESIZE; i++) {
		for (int j = 0; j < gCamera.w / TILESIZE; j++) {
			curSpot.x = j * TILESIZE;
			curSpot.y = i * TILESIZE;
			curTileID = this->getTiles().at((i * this->getWidth()) + j).getID();
			curTile.x = tileVector->at(curTileID).getRect().x;
			curTile.y = tileVector->at(curTileID).getRect().y;

			SDL_RenderCopy(gRenderer, tileTexture,&curTile,&curSpot);
		}
	}

}

int newParseMapInfo(ifstream &mapData) {
	return parseMapInfo(mapData);
}

bool newLoadMapInfo(Map *tempMap, ifstream &mapData) {
	//return loadMapInfo(tempMap, mapData);
	int mapID = mapData.get();
	if (mapData.eof()) {
		return false;
	}
	dPrint(DEBUG_ALL,"Map ID: " + std::to_string(mapID) + ".\n",false,__FILE__,__LINE__);
	int mapWidth = mapData.get();
	if (mapData.eof()) {
		return false;
	}
	dPrint(DEBUG_ALL,"Map Width: " + std::to_string(mapWidth) + ".\n",false,__FILE__,__LINE__);
	int mapHeight = mapData.get();
	if (mapData.eof()) {
		return false;
	}
	dPrint(DEBUG_ALL,"Map Height: " + std::to_string(mapHeight) + ".\n",false,__FILE__,__LINE__);
	int mapTileset = mapData.get();
	if (mapData.eof()) {
		return false;
	}
	dPrint(DEBUG_ALL,"Map Tileset: " + std::to_string(mapTileset) + ".\n",false,__FILE__,__LINE__);

	//mapData seems to be off by four???

	tempMap->setID(mapID);
	tempMap->setWidth(mapWidth);
	tempMap->setHeight(mapHeight);
	tempMap->setTileset(mapTileset);
	dPrint(DEBUG_ALL,"Loading " + std::to_string(mapWidth * mapHeight) + " tiles....",false,__FILE__,__LINE__);
	newLoadTileInfo(tempMap,mapData);
	newLoadEventInfo(tempMap,mapData);
	newLoadExitInfo(tempMap,mapData);
	if (mapData.peek() == EOF) {
		dPrint(DEBUG_ALL,"All maps loaded!",false,__FILE__,__LINE__);
	}
	return !(mapData.peek() == EOF); // TODO: error checking
}

bool newLoadTileInfo(Map *tempMap, ifstream &mapData) {
	//return loadTileInfo(tempMap, mapData);
	int mapHeight = tempMap->getHeight();
	int mapWidth = tempMap->getWidth();
	Tile tempTile;
	int maxRead = mapHeight * mapWidth;
	int curRead = 0;

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			tempTile.setXPos(TILESIZE * j);
			tempTile.setYPos(TILESIZE * i);
			tempTile.setID(mapData.get());
			if (mapData.eof()) {
				std::string errMap = std::to_string(tempMap->getID());
				std::string errTileX = std::to_string(tempTile.getXPos());
				std::string errTileY = std::to_string(tempTile.getYPos());
				dPrint(DEBUG_ERROR,"\n\nEOF reached!!!\nMap ID: " + errMap + "\nX: " + errTileX +\
					" Y: " + errTileY + "\n\n",true,__FILE__,__LINE__);
				SDL_Quit();
				exit(1);
			}
			tempTile.setCollision(defaultCollision[tempTile.getID()]);
			if (tempTile.getID() < 0 || tempTile.getID() > 20) {
				dPrint(DEBUG_ERROR,"Tile load error!",true,__FILE__,__LINE__);
				return true;
			}
			if (tempTile.getCollision() == COLLISION_UNDEFINED) {
				using std::to_string;
				dPrint(DEBUG_ERROR,"Tile in map " + to_string(tempMap->getID()) + " at coords " +\
				  to_string(i) + "," + to_string(j) + " is undefined!",true,__FILE__,__LINE__);
				  SDL_Quit();
				  return false;
			}

			tempTile.setCollisionType(tempTile.getCollision());
			if (tempTile.getCollision() == COLLISION_NONE){
				tempTile.setType(TILE_NONE);
			} else {
				tempTile.setType(TILE_FLOOR);
			}
			if (++curRead > maxRead) {
				dPrint(DEBUG_ERROR,"Map ID " + std::to_string(tempMap->getID()) + \
					" tried to load too many tiles!",true,__FILE__,__LINE__);
				break;
			}
			tempMap->addTile(tempTile);
		}
		
	}
	if (mapData.peek() == EOF) {
		return true;
	}
	return !mapData.eof(); //TODO: error checking
}

bool newLoadEventInfo(Map *tempMap, ifstream &mapData) {
	//return loadEventInfo(tempMap, mapData);
	return !mapData.eof(); //TODO: actually read/parse event info
}

bool newLoadExitInfo(Map *tempMap, ifstream &mapData) {
	//return loadExitInfo(tempMap, mapData); 
	return !mapData.eof(); //TODO: actually read/parse exit info
}