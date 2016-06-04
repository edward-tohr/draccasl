#ifndef __map_h__
#define __map_h__

//Shit, if I'm gonna be loading these off disk, I'll need to come up with a file format.

//ID (integer) \t width (tiles) \t height (tiles) \t tileset to use \t
//entrance struct: old map \t entrance struct: x pos \t entrance struct: y pos \t
//tile data (tab-separated string of ints representing each tile) \t
//event ID \t event X \t event Y \t


#include "vector"
#include "iostream"
#include "math.h"
#include "fstream"

class Map{
	public:
		struct entrances_t{
		int prevMap;
		int exitXPos;
		int exitYPos;
	};
	struct events_t{
		int eventID;
		int eventXPos;
		int eventYPos;
	};
	
	private:
	int width;
	int height;
	int tileset;
	int id;
	std::vector<entrances_t> entranceVector;
	std::vector<int> tiles;
	std::vector<events_t> eventsVector;
	
	public:
	
	int getWidth(){
		return width;
	}
	
	int getHeight(){
		return height;
	}
	
	int getTileset(){
		return tileset;
	}
	
	int getID(){
		return id;
	}
	
	std::vector<entrances_t> getEntrances(){
		return entranceVector;
	}
	
	void addEntrance(int oldMap, int entrance_x, int entrance_y){
		entrances_t entrances;
		entrances.prevMap = oldMap;
		entrances.exitXPos = entrance_x;
		entrances.exitYPos = entrance_y;
		entranceVector.push_back(entrances);
	}
	
	void addEvent(int event_id, int event_x, int event_y){
		events_t event;
		event.eventID = event_id;
		event.eventXPos = event_x;
		event.eventYPos = event_y;
		eventsVector.push_back(event);
	}
	
	void setWidth(int w){
		width = w;
	}
	
	void setHeight(int h){
		height = h;
	}
	void setTileset(int t){
		tileset = t;
	}
	void setID(int i){
		id = i;
	}
	
	void addTile(int t) {
		tiles.push_back(t);
	}
	
	int getLatestTile() {
		return tiles.back();
	}
	
};	// prototypes to be used later

void populateMapVector(std::vector<Map> mapVector);
	
bool loadMapInfo(Map* tempMap, std::ifstream &mapData);

bool loadTileInfo(Map* tempMap, std::ifstream &mapData);

bool loadEventInfo(Map* tempMap, std::ifstream &mapData);

bool loadExitInfo(Map* tempmap, std::ifstream &mapData);

int parseMapInfo(std::ifstream &mapData);

#endif