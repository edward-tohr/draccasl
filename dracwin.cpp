#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "iostream"
#include "string"
#include "sstream"
#include "stdio.h"
#include "constants.h"



//SDL_Texture* jackSprite = NULL;
//SDL_Texture* layout = NULL;

class GfxTexture
{
	public:
		//Initializes variables
		GfxTexture();

		//Deallocates memory
		~GfxTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);

		//Deallocates texture
		void free();

		//Set color modulation
		
		void setAlpha(Uint8 alpha);

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		//Gets image dimensions
		int getWidth();
		int getHeight();
		
		void GfxInit();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
		
};

GfxTexture titleScreen;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

winInit(){
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "Error with SDL_Init! %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Error with SDL_CreateWindow! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Error with SDL_CreateRenderer! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "Error with IMG_Init! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				if( TTF_Init() == -1 )
				{
					printf( "Error with TTF_Init! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

	GfxTexture::GfxTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

GfxTexture::~GfxTexture()
{
	//Deallocate
	free();
}



bool GfxTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void GfxTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void GfxTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int GfxTexture::getWidth()
{
	return mWidth;
}

int GfxTexture::getHeight()
{
	return mHeight;
}


bool loadMedia() {
	
	//Loading success flag
	bool success = true;

	//Load texture
	if( !titleScreen.loadFromFile( "title.png" ) )
	{
		printf( "Error loading title.png!\n" );
		success = false;
	} else {

	}
	
	return success;
	
}

void close()
{
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
	

int main( int argc, char* args[] )
{
	//Start up SDL and create window
     winInit();

		//Load media
		if( !loadMedia() )
		{ 
	       printf( "Failed to load media!\n" );
		} else {	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			double degrees = 0;
			SDL_RendererFlip flipType = SDL_FLIP_NONE;

			//Modulation components
			

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					} 

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

			titleScreen.render((Constants::SCREEN_WIDTH - titleScreen.getWidth()) /2, (Constants::SCREEN_HEIGHT - titleScreen.getHeight()) /2, NULL, degrees, NULL, flipType);
			
				//Update screen
				SDL_RenderPresent( gRenderer );
				
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}