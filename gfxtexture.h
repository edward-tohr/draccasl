#ifndef _GFX_H_
#define _GFX_H_

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


#endif

