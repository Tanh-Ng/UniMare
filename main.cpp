#include "Include/Global.h"
#include "Include/Graphics.h"
#include "Include/Player.h"
SDL_Window* gWindow=NULL;
SDL_Renderer* gRenderer=NULL;
LTexture gGroundTexture;
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//event handler
SDL_Event event;
bool initedLevel=false;
bool quit=false;
std::stack<StateStruct> g_StateStack;
std::stack<StateStruct> emptyStack; //for clearing stack
//player animation
player myPlayer;
const int PLAYER_IDLE_ANIMATION_FRAMES=17;
std::map<playerState, LTexture> gPlayerTexture;
std::map<playerState, std::vector <SDL_Rect>> gPlayerClips;
bool init();
bool loadMedia();
void Game();
void setCamera(SDL_Rect& camera, gameObject target);
void setPlayerAnimation();
void updatePlayer();
void updateAnimation();
void handleGameEvent();
void handleGameInput();
void Exit();
//load player sprites
void loadSpritesheet(enum playerState state, std::map<playerState, LTexture>& spritesheet,
	std::map<playerState, std::vector <SDL_Rect>>& spritesheetClip, int totalFrame);
int main(int argc, char* argv[])
{
	srand((unsigned)time(0)); //random seed

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			// Our game loop is just a while loop that breaks when our state stack is empty. //
			while (!g_StateStack.empty())
			{
				g_StateStack.top().StatePointer();
			}
		}

		return 0;
	}
	return 0;
}
 bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("UniMare", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //SDL_WINDOW_RESIZABLE
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	//set base scaling
	SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	StateStruct state;
	//add menu 
	state.StatePointer = Game;
	g_StateStack.push(state);
	return success;
}
bool loadMedia(){
	bool success = true;
		//Load static texture
	if (!gGroundTexture.loadFromFile("IMGfile/ground.png"))
	{
		printf("Failed to load ground texture!\n");
		success = false;
	}
	if (!gPlayerTexture[playerState::IDLE].loadFromFile("IMGfile/idle.png"))
	{
		printf("Failed to load player idle texture!\n");
		success = false;
	}
	else
	{
		loadSpritesheet(playerState::IDLE, gPlayerTexture, gPlayerClips, PLAYER_IDLE_ANIMATION_FRAMES);
	}

	return success;
}
void Game()
{
	SDL_ShowCursor(SDL_ENABLE);
	if (!initedLevel)
	{
		//init level
		//initLevel();
		myPlayer.initPlayer();
		initedLevel = true;
	}

	//While application is running
	while (initedLevel && !quit)
	{
		//Clear screen
		myPlayer.previousState = myPlayer.currentState;
		setCamera(camera, myPlayer);

		//Set the player back to idle mode
		myPlayer.currentState = playerState::IDLE;	
		handleGameEvent();
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(gRenderer);

		//Render ground
		for (int y = 0; y < LEVEL_SIZE; y++)
		{
			for (int x = 0; x < LEVEL_SIZE; x++)
			{
				gGroundTexture.render(camera, x * GROUND_TILE_SIZE, y * GROUND_TILE_SIZE, GROUND_TILE_SIZE, GROUND_TILE_SIZE);
			}
		}
		updatePlayer();
		updateAnimation();
		//Update screen
		
		SDL_RenderPresent(gRenderer);
	}
		if (quit)
	{
		while (!g_StateStack.empty())
		{
			g_StateStack.pop();
		}
	}
}
void close(){
		gGroundTexture.free();
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
		gRenderer = NULL;
		IMG_Quit();
		SDL_Quit();
		g_StateStack.swap(emptyStack);
}
void Exit()
{
	close();
}
void handleGameEvent(){
	Uint32 windowID = SDL_GetWindowID(gWindow);
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				quit=true;
				break;
		}
	}
}
void loadSpritesheet(enum playerState state, std::map<playerState, LTexture>& spritesheet,
	std::map<playerState, std::vector <SDL_Rect>>& spritesheetClip, int totalFrame)
{
	int w = spritesheet[state].getWidth() / totalFrame;
	int h = spritesheet[state].getHeight();
	for (int i = 0; i < totalFrame; i++)
	{
		spritesheetClip[state].push_back({ i * w, 0, w , h });
	}
}
void setCamera(SDL_Rect& camera, gameObject target) {
	//Center the camera over the target
	camera.x = int(target.px - SCREEN_WIDTH / 2);
	camera.y = int(target.py - SCREEN_HEIGHT / 2);

	//Keep the camera in bounds.
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}
void updatePlayer(){
	setPlayerAnimation();
	myPlayer.updateRenderPosition();
	myPlayer.render(camera);
}
void setPlayerAnimation(){
	myPlayer.currentState = playerState::IDLE;
	myPlayer.currentTotalFrame = PLAYER_IDLE_ANIMATION_FRAMES;
	myPlayer.setAnimation(gPlayerTexture[myPlayer.currentState],gPlayerClips[myPlayer.currentState][myPlayer.currentFrame]);
	}
void updateAnimation(){
	myPlayer.currentFrame++;
	if (myPlayer.currentFrame > myPlayer.currentTotalFrame - 1)
	{
		myPlayer.currentFrame = 0;
	}
}