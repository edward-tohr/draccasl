#ifndef __map_h__
#define __map_h__

//Shit, if I'm gonna be loading these off disk, I'll need to come up with a file format.
//read everything in as ints, tabs separate?
//and newline separates entrances struct from map data?
//and another newline separates map data from gameobject data?
//but then whatever loads the maps needs to be awate of both map.h and gameobject.h. It'll have to be done in draccasl.

#include <vector>

class Map{
	public:
		struct entrances_t{
		int prevMap;
		int x_pos;
		int y_pos;
	};
	
	private:
	int width;
	int height;
	int tileset;
	int id;
	std::vector<entrances_t> entranceVector;
	
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
		entrances.x_pos = entrance_x;
		entrances.y_pos = entrance_y;
		entranceVector.push_back(entrances);
	}
};
#endif