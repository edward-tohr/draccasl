#ifndef __map_h__
#define __map_h__

//Shit, if I'm gonna be loading these off disk, I'll need to come up with a file format.
//All values separated by tabs.
//ID (integer)
//width (tiles)
//height (tiles)
//tileset to use
//entrance struct: old map
//entrance struct: x pos
//entrance struct: y pos
//repeat until newline
//tile data (string of ints representing each tile)
//another newline
//gameobject data (int type, int X, int Y). ID and such will be set when loading.


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
	
	// prototypes to be used later
	
	void populateMapVector(std::vector<Map> mapVector);
};
#endif