#include "map.h"


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
	tempMap->setID(parseMapInfo(mapData));
	tempMap->setWidth(parseMapInfo(mapData));
	tempMap->setHeight(parseMapInfo(mapData));
	tempMap->setTileset(parseMapInfo(mapData));
	if (mapData.peek() != 10) { // If there's not a newline following the map header...
	success = false;
		if (mapData.eof()){
			if (DEBUG >= ERROR)
				std::cout << "Early EOF reached! Map data only contains header info! \n";
		}
		if (DEBUG >= ERROR)
			std::cout << "Map header is not followed by a newline. Map file may need to be recreated.\n";
	} 
	if (DEBUG >= ALL)
		std::cout << "Post-header, we are at position " << mapData.tellg() << ".\n";
	return success;
}

bool loadTileInfo(Map *tempMap, std::ifstream &mapData){
	bool success = true;
	if (DEBUG >= ALL)
		std::cout << "loating tile info....\n";
	mapData.ignore(5,10); // Skip over the newline character.
	int tilesLoaded = 0;
	while (mapData.peek() != 10 && !mapData.eof()){ //Keep loading exit data until you hit a newline.
	tempMap->addTile(parseMapInfo(mapData));
	if (++tilesLoaded > tempMap->getWidth() * tempMap->getHeight()) {
		if (DEBUG >= ERROR)
			std::cout << "too many tiles defined for map's listed size!\neither remove tiles or increase map's width and/or height.\n";
		success = false;
		break;
	}
	}
	if (tilesLoaded < tempMap->getWidth() * tempMap->getHeight()){
		if (DEBUG >= ERROR)
			std::cout << "too few tiles defined for map's listed size!\neither add tiles or decrease map's width and/or height.\n";
		success = false;
	}
	if (mapData.eof()) {
		if (DEBUG >= ERROR)
			std::cout << "EOF reached after tile info. No events defined.\n";
		success = false;
	}
	
	return success;
	

}

bool loadEventInfo(Map *tempMap, std::ifstream &mapData){
	bool repeat = false;
	if (DEBUG >= ALL)
		std::cout << "loading event info....\n";
	mapData.ignore(5,10); // Skip over the newline character.
	int eventID = 0;
	int eventXPos = 0;
	int eventYPos = 0;
	bool success = true;
	while (mapData.peek() != 10 && !mapData.eof()){ //Keep loading exit data until you hit a newline or eof.
	success = false;
	eventID = parseMapInfo(mapData);
	eventXPos = parseMapInfo(mapData);
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
	if (DEBUG >= ALL)
		std::cout << "Do we repeat? " << repeat << ".\n";
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
	exitID = parseMapInfo(mapData);
	exitXPos = parseMapInfo(mapData);
	exitYPos = parseMapInfo(mapData);
	tempMap->addEntrance(exitID,exitXPos,exitYPos);
	success = true;
	}
	
	if (!success){
		if (DEBUG >= ERROR)
			std::cout << "failed to load map exit data.\n";
		if (mapData.eof()){
			if (DEBUG >= ERROR)
				std::cout << "EOF reached in exit data.\n";
		} else {
			if (DEBUG >= ERROR)
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
	if (DEBUG >= ALL)
		std::cout << "tempMap ID is " << mapVector->back().getID() << ".\n";
	} while (loop);
	if (DEBUG >= ALL)
		std::cout << "Number of maps: " << mapVector->size() << ".\n";
}