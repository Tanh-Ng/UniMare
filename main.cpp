#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
const int SCREEN_WIDTH=1200;
const int SCREEN_HEIGHT=600;
bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);
SDL_Window* gWindow=NULL;
SDL_Renderer* gRenderer=NULL;
SDL_Texture* gBackground=NULL;
//event handler
SDL_Event e;
int main(int argc,char* argv[]){
    if(!init()){
        printf("Can't init");
    }
    else {
        if(!loadMedia()){
            printf("Can't load media");
        }
        else{
			bool quit = false;
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
				}
				//Clear screen
				SDL_RenderClear( gRenderer );
				//Render texture to screen
				SDL_RenderCopy( gRenderer, gBackground, NULL, NULL );
				//Update screen
				SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
}
 bool init()
{
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize!" );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		gWindow = SDL_CreateWindow( "UniMare", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created!" );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! ");
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize!" );
					success = false;
				}
			}
		}
	}

	return success;
}
bool loadMedia(){
	bool success = true;
	//Load PNG texture
	gBackground = loadTexture( "IMGfile/ground.png" );
	if( gBackground == NULL ){
		printf( "Failed to load background image!\n" );
		success = false;
	}
	return success;
}
SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ){
		printf( "Unable to load image %s!\n", path.c_str() );
	}
	else{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! \n", path.c_str() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	return newTexture;
}
void close()
{
	//Free loaded image
	SDL_DestroyTexture( gBackground );
	gBackground = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
