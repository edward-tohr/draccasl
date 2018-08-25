// I have no idea what I'm doing. But I won't let that stop me.

//NEXT STEP: Yay, physics! Time to add other objects. Or ramps/stairs.

#include "constants.h"
#include "draccasl.h"
#include "map.h"
#include "gameobject.h"
#include "version.h"

using std::vector;
using std::cout;
using std::string;

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
vector<Map> vectorMaps;
vector<GameObject> vectorObjects;
vector<Tile> vectorTiles;
vector<Tile> mapTiles;
vector<SDL_Rect> vectorCollision;
Map curMap;
bool sound = true;
bool gravity = true;


DEBUG_T DEBUG = DEBUG_ALL;


// SDL requires int main(int argc char* argv[]). Remember that.
// Also, should make new functions for init, event, loop, render, close.



void init() {
	//Setup phase
	//sound = !DEBUG;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
			dPrint(DEBUG_ERROR,SDL_GetError(),"SDL init failed:");
      exit();
		
	} else {
		dPrint(DEBUG_ERROR,"SDL init succeeded.");
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		dPrint(DEBUG_ERROR, SDL_GetError(),"SDL_img init failed:");
      exit();
	} else {
		dPrint(DEBUG_ERROR,"SDL_img init succeeded.");
	}

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
		dPrint(DEBUG_ERROR,SDL_GetError(), "SDL_mixer init failed:");
      exit();
		
	} else {
		dPrint(DEBUG_ERROR,"SDL_mixer init succeeded.");
	}

	//Load phase

	gWindow = SDL_CreateWindow("jack DANGER strong in: castle of the draculas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W,WINDOW_H, 0);
	if (gWindow == NULL) {
		dPrint(DEBUG_ERROR,SDL_GetError(),"gWindow is NULL! 'cause of");
      exit();
	}

	jackSprite = IMG_Load("jack.png");
	if (!jackSprite) {
		dPrint(DEBUG_ERROR,"we let go of jack!");
		exit();
	}

	gSurface = IMG_Load("title.png");
	if (!gSurface) {
		dPrint(DEBUG_ERROR,"failed to load title screen!");
		exit();
	}

	if (sound) {
		gMusic = Mix_LoadMUS("tocafuge.wav");
		if (gMusic == NULL) {
			dPrint(DEBUG_ERROR,"Failed to load tocafuge.wav!");
		}
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);

	gCamera.x = 0;
	gCamera.y = 0;
	gCamera.w = WINDOW_W;
	gCamera.h = WINDOW_H;
	gameState = title;
	currentMap = 0;
	nextMap = 0;

	Jack->setXPos(70);
	Jack->setYPos(70);
	Jack->setHealth(10);
	Jack->setMaxHealth(10);
	Jack->setAttack(0);
	Jack->setDefense(0);
	Jack->setType(jack);
	Jack->unkill();
	//SDL_SetColorKey(jackSprite,SDL_TRUE,SDL_MapRGB(jackSprite->format,0xFF,0x00,0xFF));
	//cout << "Jack's color-keyed now.\n";
	//Jack->setTexture(SDL_CreateTextureFromSurface(gRenderer,jackSprite));
	Jack->loadSprite("jack");
	dPrint(DEBUG_ALL,"Jack's sprite is set.");

	Jack -> setCollision(70,70,64,64);
	vectorObjects.push_back(*Jack);

}

void exit() {
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyTexture(gTexture);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
	std::exit(0);
}


bool event(SDL_Event e) {
	// handle keypresses and whatnot.
	while (SDL_PollEvent(&e) != 0) {

		if(e.type == SDL_QUIT) {
			return true;

		} else if (e.type == SDL_KEYDOWN) {
			switch(e.key.keysym.sym) {

				case SDLK_RETURN:
					if (gameState == title) {
						gameStart();

					} else if (gameState == game) {
						gameState = pause;
					} else if (gameState == pause) {
						gameState = game;
					}
					break;

				case SDLK_d:
					if ((SDL_GetModState() & KMOD_CTRL) && (SDL_GetModState() & KMOD_ALT)) {
						switch (DEBUG) {
							case DEBUG_NONE:
								DEBUG = DEBUG_ERROR;
								break;
							case DEBUG_ERROR:
								DEBUG = DEBUG_ALL;
								break;
							case DEBUG_ALL:
								DEBUG = DEBUG_NONE;
								break;
						}
						cout << "Debug level set to " << DEBUG << ".\n";
					}
					break;

				case SDLK_g:
					if ((SDL_GetModState() & KMOD_CTRL) && (SDL_GetModState() & KMOD_ALT)) {
						gravity = !gravity;
						cout << "gravity set to " << gravity << "\n";
						break;
					}
					break;


				case SDLK_BACKSLASH:
					if (DEBUG > DEBUG_NONE) {
						currentMap++;
						currentMap %= vectorMaps.size();
						cout << "loading map " << currentMap << "...\n";
						loadMap(vectorMaps.at(currentMap));
					}
					break;

				case SDLK_HOME:
					if (DEBUG > DEBUG_NONE) {
						cout << "X: " << vectorObjects.at(0).getXPos() << " Y: " << vectorObjects.at(0).getYPos() << "\n";
						cout << Jack->getXPos() << " " << Jack->getYPos() << "\n";
					}
					break;

				case SDLK_END:
					if (DEBUG > DEBUG_NONE) {
						vectorObjects.at(0).loadSprite("dracula");
					}

			}
			return false;
		}
	}
	return false;
}

void render() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	// clear the renderer.
	if (gameState != title) { //If we're on the title screen, don't draw all this crap.
		//Render terrain
		vectorMaps.at(currentMap).Map::render(&vectorTiles);
		//Render Objects
		//Jack->render();
		for (unsigned int i = 0; i < vectorObjects.size(); i++) {
			vectorObjects.at(i).render();
		}

	} else { //If we are on the title screen, draw that.
		SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);
	}
	//Render to the screen.

	SDL_RenderPresent(gRenderer);

}

void loadMap(Map mapToLoad) {
	// We have a map taken from vectorMaps, taken from maps.map.
	// We need a surface/texture of the tileset.
	int tiles = mapToLoad.getTileset();
	cout << "tiles = " << mapToLoad.getTileset() << "\n";
	string tileName = "tileset_";
	tileName.append(std::to_string(tiles));
	tileName.append(".png");
	cout << "tilename = " << tileName << "\n";
	SDL_FreeSurface(tileSet);
	cout <<"tileset is freed.\n";
	tileSet = IMG_Load(tileName.c_str());


	if (tileSet == NULL) {
		dPrint(DEBUG_ERROR,"Error loading tileset " + tileName + "!");
	} else {
		dPrint(DEBUG_ALL,"Successfully loaded tileset " + tileName + "!");

		tileTexture = SDL_CreateTextureFromSurface(gRenderer, tileSet);

		// Once we have it loaded, we can slice the tileset into tiles, and store it all in a vector<Tile>.
		int numCols = tileSet->w / TILESIZE;
		int numRows = tileSet->h / TILESIZE;
		Tile currentTile;
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++) {
				currentTile.setRect(j*TILESIZE,i*TILESIZE);
				vectorTiles.push_back(currentTile);
			}
		}
		currentMap = mapToLoad.getID();
		curMap = mapToLoad;
		mapTiles.clear();
		vector<Tile> tempTiles = curMap.getTiles();
		for (unsigned int i = 0; i < tempTiles.size(); i++) {
			if (tempTiles.at(i).getID() != 1) {
				mapTiles.push_back(tempTiles.at(i)); //mapTiles should now only contain non-air tiles.
			}
		}
		
			dPrint(DEBUG_ALL, "Map ID: " + std::to_string(curMap.getID()) + " has " + std::to_string(curMap.getTiles().size()) + " tiles innit.");
		
		// And now we have vector<Tile> vectorTiles that contains each individual tile, sorted by tile ID.
	}
}

void checkCollision(GameObject* actor, std::vector<Tile> collidingTerrain) {
  // takes pointer to gameObject and a vector of tiles which are overlapping in some way with the specified object.
  // object is a pointer so we can modify the original object.
  // let's suss out the proper logic here...
  // first off, take object's position, move it down and see if it's in the floor. Y velocity + gravity? gravity + floor-snap factor? All three? gravity + max(floor-snap,y velocity)?
  // if yes, cancel y velocity, snap object to the level of the floor, and set the midair flag to false.
  // Otherwise, set midair flag to true.

  // Next, we check the midair flag, and handle y velocity if true. add gravity to y vel, check for collisions, etc.
  
  // Next, we handle x velocity. If collider collides, check upwards and downwards by floor-snap for a floor. If yes, get floor height, and maybe calculate x distance via Pythagoras?
  // If collider does not collide, then good.

  // Do floor check again? Or skip the first one and only do it here?


  // Finally, move object to appropriate coordinate of each collider.
  
  //TODO: move collision checks here.

  SDL_Rect* tempRect = new SDL_Rect();
  SDL_Rect* actorCollider = new SDL_Rect();
  tempRect = actorCollider;
  tempRect->x += 1;
  int butts = actor->getID();
  butts++;
  collidingTerrain.clear();

  // take collidingTerrain, grab only tiles below character (where tile X is between actor X and actor X - TILEWIDTH)
  // of those tiles, only keep ones where tile Y is between actor Y + actor height + actor y vel and actor Y + actor height + FLOOR_SNAP + actor y vel
  // If there's a tile left, snap actor y to tile Y + actor height and disable midair flag.
  // else, set midair flag.

}
  


void loop() {

	for (unsigned int i = 0; i < vectorObjects.size(); i++) {
		vectorObjects.at(i).beginUpdate();
		vectorCollision.clear();
		// collision detection goes here.
		// let's see, let's see......
		// move collisionBox in the X direction according to velocity.
		SDL_Rect tempRect = vectorObjects.at(i).moveCollider(vectorObjects.at(i).getXVel(),0);
    signed int rectX = tempRect.x;
    signed int rectW = tempRect.w;
    signed int rectY = tempRect.y;
    signed int rectH = tempRect.h;

		bool eraseTile = false;
		// get a vector of tiles that have x coordinate + width between collider's x and collider's x + width
		//if (DEBUG == DEBUG_ALL){cout << "Temprect is at: " << tempRect.x << "," << tempRect.y << ". Xvel is " << vectorObjects.at(i).getXVel() << "\n";}
		for (unsigned int j = 0; j < mapTiles.size(); j++) {
			if (mapTiles.at(j).getID() != 1) {
				if (rectX >= mapTiles.at(j).getXPos() && rectX <= mapTiles.at(j).getXPos() + TILESIZE) {
					vectorCollision.push_back(mapTiles.at(j).getRect());
				}
				if (rectX + rectW >= mapTiles.at(j).getXPos() && rectX + rectW <= mapTiles.at(j).getXPos() + TILESIZE) {
					vectorCollision.push_back(mapTiles.at(j).getRect());
				}
				for (int k = 0; k <= rectW; k += TILESIZE) {
					if (rectX + k >= mapTiles.at(j).getXPos() && rectX + k <= mapTiles.at(j).getXPos() + TILESIZE) {
						vectorCollision.push_back(mapTiles.at(j).getRect());
					}
				}
			}
		}


		// Ugh, this conditional is still horrible, but here goes....
		// vectorCollision contains all non-air tiles that are in the same column as the object.
		// This should prune tiles that aren't in the same row, leaving only tiles that the object is truly overlapping.

		for (unsigned int j = 0; j < vectorCollision.size(); j++) {
			eraseTile = true;
			for (signed int k = 0; k < rectH; k += TILESIZE) {
				if (rectY >= vectorCollision.at(j).y && rectY <= vectorCollision.at(j).y + TILESIZE) {
					eraseTile = false;
					break;

				} else if (rectY + rectH >= vectorCollision.at(j).y && rectY + rectH <= vectorCollision.at(j).y + TILESIZE) {
					eraseTile = false;
					break;

				} else if (rectY + k >= vectorCollision.at(j).y && rectY + k <= vectorCollision.at(j).y + TILESIZE) {
					eraseTile = false;
					break;
				}
			}
			if (eraseTile) {
				vectorCollision.erase(vectorCollision.begin()+j);
				j--; // No need to back up all the way to the start since we're only erasing one tile at a time.

			}
		}


		// if vector is empty, great.
		if (!vectorCollision.empty()) {
			// if not, check x velocity. If positive, set collider's x equal to smallest x in terrain vector. If negative, set X equal to largest X + width. Set velocity to 0 either way.
      // checkCollision(vectorObjects.at(i),vectorCollision);
      // do I need vectorObjects.at(i)&? maybe.
			if (vectorObjects.at(i).getXVel() > 0) {
				int minx = rectX + rectW;
				for (unsigned int j = 0; j < vectorCollision.size(); j++) {
					minx = std::min(minx,vectorCollision.at(j).x);
				}
				minx--;
				vectorObjects.at(i).setXPos(minx-vectorObjects.at(i).getWidth());
				vectorObjects.at(i).setXVel(0);
			} else if (vectorObjects.at(i).getXVel() < 0) {
				int maxx = rectX;
				for (unsigned int j = 0; j < vectorCollision.size(); j++) {
					maxx = std::max(maxx,vectorCollision.at(j).x + TILESIZE);
				}
				maxx++;
				vectorObjects.at(i).setXPos(maxx);
				vectorObjects.at(i).setXVel(0);
			}
		}

		vectorCollision.clear();
		tempRect = vectorObjects.at(i).moveCollider(0,vectorObjects.at(i).getYVel());
    rectY = tempRect.y;
    rectX = tempRect.x;
    rectW = tempRect.w;
    rectH = tempRect.h;
    
		for (unsigned int j = 0; j < mapTiles.size(); j++) {
			if (mapTiles.at(j).getID() != 1) {
				if (rectY >= mapTiles.at(j).getYPos() && rectY <= mapTiles.at(j).getYPos() + TILESIZE) {
					vectorCollision.push_back(mapTiles.at(j).getRect());
				}
				if (rectY + rectH >= mapTiles.at(j).getYPos()&& rectY + rectH <= mapTiles.at(j).getYPos() + TILESIZE) {
					vectorCollision.push_back(mapTiles.at(j).getRect());
				}
				for (signed int k = 0; k <= rectH; k += TILESIZE) {
					if (rectY + k >= mapTiles.at(j).getYPos() && rectY + k <= mapTiles.at(j).getYPos() + TILESIZE) {
						vectorCollision.push_back(mapTiles.at(j).getRect());
					}
				}
			}
		}


		for (unsigned int j = 0; j < vectorCollision.size(); j++) {
			eraseTile = true;
			for (signed int k = 0; k <= tempRect.w; k += TILESIZE) {
				if (rectX >= vectorCollision.at(j).x && rectX <= vectorCollision.at(j).x + TILESIZE) {
					eraseTile = false;
					break;

				} else if (rectX + rectW >= vectorCollision.at(j).x && rectX + rectW <= vectorCollision.at(j).x + TILESIZE) {
					eraseTile = false;
					break;

				} else if (rectX + k >= vectorCollision.at(j).x && rectX + k <= vectorCollision.at(j).x + TILESIZE) {
					eraseTile = false;
					break;
				}

			}
			if (eraseTile) {
				vectorCollision.erase(vectorCollision.begin()+j);
				j--;
			}
		}

		if(!vectorCollision.empty()) {

			if (vectorObjects.at(i).getYVel() > 0) { // If Y velocity is positive, object is moving downwards and should snap to top of terrain minus object's height.
				int miny = rectY + rectH; // get position of object's feet
				for (unsigned int j = 0; j < vectorCollision.size(); j++) {
					miny = std::min(miny,vectorCollision.at(j).y - vectorObjects.at(i).getHeight()); //Set YPos equal to whichever is higher, the object's feet, or top of terrain.
				}
				miny--;
				vectorObjects.at(i).setYPos(miny);
				vectorObjects.at(i).setYVel(0);
				//gravity = false;
			} else if (vectorObjects.at(i).getYVel() < 0) { //If Y velocity is negative, object is moving upwards and should snap to bottom of terrain.
				int maxy = rectY;
				for (unsigned int j = 0; j < vectorCollision.size(); j++) {
					maxy = std::max(maxy, vectorCollision.at(j).y + TILESIZE);
				}
				maxy++;
				vectorObjects.at(i).setYPos(maxy);
				vectorObjects.at(i).setYVel(0);
			}


		}

		vectorObjects.at(i).collisionUpdate();

	}
	//physics, enemy AI goes here
	// also handle map transitions?



}

void gameStart() {
	//New Game setup goes here. Load map, set up player graphics and such.
	if (sound) {
		Mix_PlayMusic(gMusic, 1);
		if (DEBUG == DEBUG_NONE) {
			SDL_Delay(3500);
		}
		Mix_HaltMusic();
		gMusic = Mix_LoadMUS("jack.mid");
	}
	gameState = pause;
	nextMap = 1;
	populateMapVector(&vectorMaps);
	//populateObjectVector(&vectorObjects) will work the same way once I do that.
	if (DEBUG == DEBUG_ALL) {
		dPrint(DEBUG_ALL, "VectorMaps size = " + std::to_string(vectorMaps.size()));
		dPrint(DEBUG_ALL, "vectorObjects size = " + std::to_string(vectorObjects.size()));
	}
	loadMap(vectorMaps.at(0));
	if (sound) {
		Mix_PlayMusic(gMusic,-1);
	}
}



int main(int argc, char* argv[]) {


	std::ofstream outFile ("version.txt",std::ofstream::trunc);

	string fileOutput;
	fileOutput  = "jack DANGER strong in: castle of the draculas\nBuild Date: ";
	fileOutput += __DATE__;
	fileOutput += " ";
	fileOutput += __TIME__;
	fileOutput += "\n";
	fileOutput += __FILE__;
	fileOutput += " last modified: ";
	fileOutput += __TIMESTAMP__;
	fileOutput += "\nBuild Number: ";
	fileOutput += AutoVersion::BUILD;
	fileOutput += "\n";


	cout << fileOutput;

	outFile << fileOutput;



	//Turns out that maybe SDL uses argc and argv[] for its own stuff, and sometimes breaks when it's messed up like this?
	// Maybe not. Let's see.
	if (argc >= 2) {
		string argument;
		for (int i = 0; i < argc; i++) {
			argument = string(argv[i]);
			if (argument == "--DEBUG-NONE" || argument == "-d") {
				DEBUG = DEBUG_NONE;
			}
			if (argument == "--DEBUG-ERROR" || argument == "-E") {
				DEBUG = DEBUG_ERROR;
			}
			if (argument == "--DEBUG-ALL" || argument == "-D") {
				DEBUG = DEBUG_ALL;
			}
			if (argument == "--NO-SOUND" || argument == "-s") {
				sound = false;
			}
      if (argument == "--help" || argument == "-h") {
        std::cout << "jack DANGER strong in: Castle of the Draculas\n" << \
                     "Debug flags: --DEBUG-NONE, --DEBUG-ERROR, --DEBUG-ALL\n" << \
                     "Sound: --NO-SOUND\n" << \
                     "Help: --help\n";
        exit();
      }

		}

	}

	cout << "Debug level is: ";
	switch (DEBUG) {
		case DEBUG_NONE:
			cout << "none.\n";
			break;
		case DEBUG_ERROR:
			cout << "error messages only.\n";
			break;
		case DEBUG_ALL:
			cout << "full debug info.\n";
			break;
	}
	init();
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		quit = event(e);
		if (gameState == game) {
			loop();
		}
		render();
	}
	exit();
	return 0;
}
