#ifndef __map_h__
#define __map_h__

//Shit, if I'm gonna be loading these off disk, I'll need to come up with a file format.

//ID (integer) \t width (tiles) \t height (tiles) \t tileset to use \t
//entrance struct: old map \t entrance struct: x pos \t entrance struct: y pos \t
//tile data (tab-separated string of ints representing each tile) \t
//event ID \t event X \t event Y \t


#include "vector"
#include "iostream"
#include "iomanip"
#include "cmath"
#include "fstream"
#include "constants.h"
#include "sdl_files.h"

extern DEBUG_T DEBUG;
extern SDL_Renderer *gRenderer;
extern SDL_Texture *tileTexture;
extern SDL_Rect gCamera;
extern const int TILESIZE;

class Tile {

 private:
	int id;             //The tiletype ID.
	SDL_Rect tileRect; //Contains the size and location in pixels of the tile.
  TILE_T type = TILE_FLOOR;
  float normal = 0; // Tile's normal vector in degrees. For ramps.

 public:
	void setRect(int x, int y);
	void setID (int i);
	SDL_Rect getRect();
	int getXPos();
	int getYPos();
	void setXPos(int x);
	void setYPos(int y);
	int getID();
  TILE_T getType();
  void setType(TILE_T type);
  float getNormal();
  void setNormal(float normal);
	Tile();
  Tile(TILE_T type, float normal);
};


class Map {
 public:
	struct entrances_t {
		int prevMap;
		int exitXPos;
		int exitYPos;
	};
	struct events_t {
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
	std::vector<Tile> tiles;
	std::vector<events_t> eventsVector;

 public:

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	int getTileset() {
		return this -> tileset;
	}

	int getID() {
		return id;
	}

	std::vector<entrances_t> getEntrances() {
		return entranceVector;
	}

	std::vector<Tile> getTiles() {
		return tiles;
	}

	void addEntrance(int oldMap, int entrance_x, int entrance_y) {
		entrances_t entrances;
		entrances.prevMap = oldMap;
		entrances.exitXPos = entrance_x;
		entrances.exitYPos = entrance_y;
		entranceVector.push_back(entrances);
	}

	void addEvent(int event_id, int event_x, int event_y) {
		events_t event;
		event.eventID = event_id;
		event.eventXPos = event_x;
		event.eventYPos = event_y;
		eventsVector.push_back(event);
	}

	void clearMap() {
		eventsVector.clear();
		entranceVector.clear();
		tiles.clear();
	}

	void setWidth(int w) {
		width = w;
	}

	void setHeight(int h) {
		height = h;
	}
	void setTileset(int t) {
		tileset = t;
	}
	void setID(int i) {
		id = i;
	}

	void addTile(Tile t) {
		tiles.push_back(t);
	}

	Tile getLatestTile() {
		return tiles.back();
	}

	void render(std::vector<Tile>* tileVector);

};	// prototypes to be used later


void populateMapVector(std::vector<Map>* mapVector);

bool loadMapInfo(Map* tempMap, std::ifstream &mapData);

bool loadTileInfo(Map* tempMap, std::ifstream &mapData);

bool loadEventInfo(Map* tempMap, std::ifstream &mapData);

bool loadExitInfo(Map* tempmap, std::ifstream &mapData);

int parseMapInfo(std::ifstream &mapData);



#endif
