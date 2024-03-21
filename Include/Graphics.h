#pragma once
#include "Global.h"
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);
	//Deallocates texture
	void free();
	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);
	SDL_Texture* getSDLTexture();
	//Renders texture at given point
	void renderchar(int x, int y, int w, int h, SDL_Rect* clip);
	void render(int x, int y, int w, int h, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void render(SDL_Rect& camera, int x, int y, int w, int h, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};