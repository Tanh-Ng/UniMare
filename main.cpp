#include "Include/Global.h"
#include "Include/Graphics.h"
#include "Include/Player.h"
#include "Include/Enemy.h"
#include "Include/Weapon.h"
#include "Include/Button.h"
SDL_Window* gWindow=NULL;
SDL_Renderer* gRenderer=NULL;
LTexture gGroundTexture;
LTexture gRockTexture;
LTexture gBulletTexture;
LTexture gCrosshairTexture;
LTexture gMenuTexture;
SDL_Color UIColor = {0,0,0};
//backdrop texture used for pause screen
SDL_Texture* backdrop;
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//event handler
SDL_Event event;
//input handle
const Uint8* keys;
int mouseX;
int mouseY;
Uint32 mouses;
button myButton;
std::vector<button> buttons;
bool initedLevel=false;
bool quit=false;
bool allowSpawning=false;
bool pickup=false;
int level = 0;
int enemiesCount = 0;
int target = 20;
std::stack<StateStruct> g_StateStack;
std::stack<StateStruct> emptyStack; //for clearing stack
//player animation
player myPlayer;
const int ANIMATION_FRAMES=18;
std::map<playerState, LTexture> gPlayerTexture;
std::map<playerState, std::vector <SDL_Rect>> gPlayerClips;
//enemy animation
enemy myEnemy;
std::vector<std::map<enemyState, LTexture>> gEnemyTexture(4);
std::map<enemyState, std::vector <SDL_Rect>> gEnemyClips;
std::vector<enemy> enemies;
//rocks 
gameObject rock;
std::vector <SDL_Rect> gRockClips;
std::vector<gameObject> rocks;
//weapons
int currentSlot;
std::vector<weapon> Weapon(TOTAL_SLOT);
weapon Dummy;
std::vector<LTexture> WeaponTexture(4);
std::vector<std::vector <SDL_Rect>> WeaponClip(4);
std::vector<weapon> droppedWeapon;
int cooldown;
std::vector<bullet> bullets;
//fonts;
const int fontSize = SCREEN_HEIGHT / 30;
const int fontSizeSmall = SCREEN_HEIGHT / 40;
const int fontSizeLarge = SCREEN_HEIGHT / 20;
const int fontSizeTitle = SCREEN_HEIGHT / 7.5;
TTF_Font* boldFont;
TTF_Font* regularFont;
TTF_Font* boldFontSmall;
TTF_Font* boldFontLarge;
TTF_Font* boldFontTitle;
TTF_Font* regularFontSmall;
bool init();
bool loadMedia();
void initLevel();
void clearScreen();
void handleMenuEvent(int& choice);
void Menu();
//confirm screen
void Confirm();
enum class confirmState { FALSE, RETRY, QUIT, QUIT_TO_MENU };
confirmState confirmMode;
void showConfirmScreen(confirmState m);
void hideConfirmScreen();
bool confirmScreen = false; //flag
void handleConfirmEvent(int& choice);
void Game();
void Pause();
bool paused = false; //flag
void handlePauseEvent();
//end game screen
void EndGame();
enum class endState { FALSE, WIN, LOSE};
endState endGameMode = endState::FALSE;
void showEndGamecreen(endState m);
void hideEndGameScreen();
bool endGameScreen = false; //flag
void handleEndGameEvent(int& choice);
void createGameObjectRandom(gameObject source, std::vector<gameObject>& vectorList, int total, int minSize, int maxSize, int maxType);
void setCamera(SDL_Rect& camera, gameObject target);
void setPlayerAnimation();
void updatePlayer();
void updateWeapon();
void updateBullet();
void renderCrosshair();
void spawnEnemy();
void updateEnemy();
void updateAnimation();
void handleGameEvent();
void handleGameInput();
void Exit();
//load player sprites
void loadSpritesheet(enum playerState state, std::map<playerState, LTexture>& spritesheet,
	std::map<playerState, std::vector <SDL_Rect>>& spritesheetClip, int totalFrame);
void loadSpritesheet(enum enemyState state, std::map<enemyState, LTexture>& spritesheet,
	std::map<enemyState, std::vector <SDL_Rect>>& spritesheetClip, int totalFrame);
void loadClips(LTexture& spritesheet, std::vector<SDL_Rect>& spritesheetClip, int totalClip);
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
		if (TTF_Init() == -1)
		{
			printf("Unable to init TTF: %s\n", SDL_GetError());
			success = false;
		}
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
	state.StatePointer = Menu;
	g_StateStack.push(state);
	return success;
}
bool loadMedia(){
	bool success = true;
		//Load static texture
		
	if (!gMenuTexture.loadFromFile("IMGfile/background.png"))
	{
		printf("Failed to load background texture!\n");
		success = false;
	}	
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
		loadSpritesheet(playerState::IDLE, gPlayerTexture, gPlayerClips, ANIMATION_FRAMES);
	}
	if (!gPlayerTexture[playerState::WALK].loadFromFile("IMGfile/walk.png"))
	{
		printf("Failed to load player walk texture!\n");
		success = false;
	}
	else
	{
		loadSpritesheet(playerState::WALK, gPlayerTexture, gPlayerClips, ANIMATION_FRAMES);
	}
	if (!gPlayerTexture[playerState::DEAD].loadFromFile("IMGfile/dead.png"))
	{
		printf("Failed to load player dead texture!\n");
		success = false;
	}
	else
	{
		loadSpritesheet(playerState::DEAD, gPlayerTexture, gPlayerClips, ANIMATION_FRAMES);
	}
	if (!gEnemyTexture[0][enemyState::WALK].loadFromFile("IMGfile/enemy1walk.png"))
	{
		printf("Failed to load enemy 1 walk texture!\n");
		success = false;
	}
	else
	{
		loadSpritesheet(enemyState::WALK, gEnemyTexture[0], gEnemyClips, ANIMATION_FRAMES);
	}
	if (!gEnemyTexture[1][enemyState::WALK].loadFromFile("IMGfile/enemy2walk.png"))
	{
		printf("Failed to load enemy 2 walk texture!\n");
		success = false;
	}
	else
	{
		loadSpritesheet(enemyState::WALK, gEnemyTexture[1], gEnemyClips, ANIMATION_FRAMES);
	}
	if (!gEnemyTexture[2][enemyState::WALK].loadFromFile("IMGfile/enemy3walk.png"))
	{
		printf("Failed to load enemy 3 walk texture!\n");
		success = false;
	}
	else
	{
		loadSpritesheet(enemyState::WALK, gEnemyTexture[2], gEnemyClips, ANIMATION_FRAMES);
	}
	if (!gEnemyTexture[3][enemyState::WALK].loadFromFile("IMGfile/enemy4walk.png"))
	{
		printf("Failed to load enemy 4 walk texture!\n");
		success = false;
	}
	else
	{
		loadSpritesheet(enemyState::WALK, gEnemyTexture[3], gEnemyClips, ANIMATION_FRAMES);
	}
	if (!gRockTexture.loadFromFile("IMGfile/rocks.png"))
	{
		printf("Failed to load rock texture!\n");
		success = false;
	}
	else
	{
		loadClips(gRockTexture, gRockClips, ROCKS_CLIP);
	}
	if (!WeaponTexture[0].loadFromFile("IMGfile/lightsaber.png"))
	{
		printf("Failed to load saber texture!\n");
		success = false;
	}
	else
	{
		loadClips(WeaponTexture[0], WeaponClip[0], WEAPON_CLIP);
	}
	if (!WeaponTexture[2].loadFromFile("IMGfile/shotgun.png"))
	{
		printf("Failed to load shotgun texture!\n");
		success = false;
	}
	else
	{
		loadClips(WeaponTexture[2], WeaponClip[2], WEAPON_CLIP);
	}
	if (!WeaponTexture[1].loadFromFile("IMGfile/pistol.png"))
	{
		printf("Failed to load pistol texture!\n");
		success = false;
	}
	else
	{
		loadClips(WeaponTexture[1], WeaponClip[1], WEAPON_CLIP);
	}
	if (!WeaponTexture[3].loadFromFile("IMGfile/rifle.png"))
	{
		printf("Failed to load rifle texture!\n");
		success = false;
	}
	else
	{
		loadClips(WeaponTexture[3], WeaponClip[3], WEAPON_CLIP);
	}
	if (!gBulletTexture.loadFromFile("IMGfile/bullet.png"))
	{
		printf("Failed to load bullet texture!\n");
		success = false;
	}
	if (!gCrosshairTexture.loadFromFile("IMGfile/crosshair.png"))
	{
		printf("Failed to load crosshair texture!\n");
		success = false;
	}
	//fonts
	boldFont = TTF_OpenFont("Font/OpenSans-Bold.ttf", fontSize);
	if (boldFont == NULL) {
		printf("Failed to load bold font! TTF Error: %s\n", TTF_GetError());
		success = false;
	}

	boldFontSmall = TTF_OpenFont("Font/OpenSans-Bold.ttf", fontSizeSmall);
	if (boldFontSmall == NULL) {
		printf("Failed to load bold font small! TTF Error: %s\n", TTF_GetError());
		success = false;
	}

	boldFontLarge = TTF_OpenFont("Font/OpenSans-Bold.ttf", fontSizeLarge);
	if (boldFontLarge == NULL) {
		printf("Failed to load bold font large! TTF Error: %s\n", TTF_GetError());
		success = false;
	}

	boldFontTitle = TTF_OpenFont("Font/OpenSans-Bold.ttf", fontSizeTitle);
	if (boldFontTitle == NULL) {
		printf("Failed to load bold font title! TTF Error: %s\n", TTF_GetError());
		success = false;
	}

	regularFont = TTF_OpenFont("Font/OpenSans-Regular.ttf", fontSize);
	if (regularFont == NULL) {
		printf("Failed to load regular font! TTF Error: %s\n", TTF_GetError());
		success = false;
	}

	regularFontSmall = TTF_OpenFont("Font/OpenSans-Regular.ttf", fontSizeSmall);
	if (regularFontSmall == NULL) {
		printf("Failed to load regular font small! TTF Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}
void initLevel(){
	//clear level objects
	rocks.clear();
	enemies.clear();
	bullets.clear();
	//objective
	int level = 0;
	int enemiesCount = 0;
	int target = 20;
	SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	myPlayer.initPlayer();
	Weapon[currentSlot].initWeapon(myPlayer.currentWeapon);
	createGameObjectRandom(rock, rocks, MAX_ROCKS_NUM, MIN_ROCK_SIZE, MAX_ROCK_SIZE, ROCKS_CLIP);
	allowSpawning = true;
	confirmMode = confirmState::FALSE;
	paused = false;
	confirmScreen = false;
	initedLevel = true;
	currentSlot=0;
	cooldown=0;
}
void clearScreen()
{
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 0);
	SDL_RenderClear(gRenderer);
}
void handleMenuEvent(int& choice)
{
	//Poll events
	while (SDL_PollEvent(&event))
	{
		//check events
		switch (event.type)
		{
		case SDL_QUIT: //User hit the X
			choice = 1;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				//resize window
				SDL_SetWindowSize(gWindow, event.window.data1, event.window.data2);
				//SCREEN_WIDTH = event.window.data1;
				//SCREEN_HEIGHT = event.window.data2;
			}
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			//start button
			if (buttons[0].checkInside(mouseX, mouseY))
			{
				choice = 0;
			}
			//quit
			if (buttons[1].checkInside(mouseX, mouseY))
			{
				choice = 1;
			}
			break;
		}
	}

	
}
void Menu(){
	//show back the cursor
	SDL_ShowCursor(SDL_ENABLE);
	//SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); //set the cursor to center of the window

	//set text positions
	int textOffset = SCREEN_HEIGHT / 4;
	int textX = SCREEN_WIDTH / 2;
	int textY = SCREEN_HEIGHT / 2 - textOffset;

	//add buttons
	//start button
	int buttonpy = textY + SCREEN_HEIGHT / 7.5 + 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Start", regularFont);
	buttons.push_back(myButton);
	//quit button
	buttonpy += 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Quit", regularFont);
	buttons.push_back(myButton);

	int choice = -1; //0 for yes, 1 for no

	while (choice == -1)
	{
		mouses = SDL_GetMouseState(&mouseX, &mouseY);
		handleMenuEvent(choice);
		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(gRenderer);
		//Render black overlay 
		gMenuTexture.render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		//Render title
		drawText(textX, textY, boldFontTitle, UIColor, "UNI MARE!", 1);

		//set toggle music text
		//Render buttons
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i].checkButton(mouses, mouseX, mouseY);
			buttons[i].render(gRenderer);
		}
		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	//remove all buttons
	buttons.clear();
	clearScreen();
	StateStruct temp;
	switch (choice)
	{
	case 0: //start
		initedLevel = false;
		temp.StatePointer = Game;
		g_StateStack.push(temp);
		break;
	case 1: //quit
		showConfirmScreen(confirmState::QUIT);
		temp.StatePointer = Confirm;
		g_StateStack.push(temp);
	}

	//get backdrop
	backdrop = gMenuTexture.getSDLTexture();
}
void Confirm()
{
	//show back the cursor
	SDL_ShowCursor(SDL_ENABLE);
	//SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); //set the cursor to center of the window
	//set text positions
	int textOffset = SCREEN_HEIGHT / 10;
	int textX = SCREEN_WIDTH / 2;
	int textY = SCREEN_HEIGHT / 2 - textOffset;

	//add buttons
	//yes button
	int buttonpy = textY + 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Yes", regularFont);
	buttons.push_back(myButton);
	//no button
	buttonpy += 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "No", regularFont);
	buttons.push_back(myButton);

	int choice = -1; //0 for yes, 1 for no

	while (choice == -1)
	{
		mouses = SDL_GetMouseState(&mouseX, &mouseY);
		handleConfirmEvent(choice);
		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(gRenderer);
		//Render backdrop
		SDL_RenderCopy(gRenderer, backdrop, NULL, NULL);
		std::string confirmText = "";
		//Render text
		switch (confirmMode)
		{
		case confirmState::RETRY:
			confirmText = "Are you sure you want to retry the level?";
			break;
		case confirmState::QUIT:
			confirmText = "Are you sure you want to quit?";
			break;
		case confirmState::QUIT_TO_MENU:
			confirmText = "Are you sure you want to quit to menu?";
			break;
		}
		drawText(textX, textY, boldFontLarge, UIColor, confirmText, 1);

		//Render buttons
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i].checkButton(mouses, mouseX, mouseY);
			buttons[i].render(gRenderer);
		}

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	//remove all buttons
	buttons.clear();

	clearScreen();

	StateStruct temp;
	switch (choice)
	{
	case 0:
		if (confirmMode == confirmState::RETRY) //retry
		{
			hideConfirmScreen();
			initedLevel = false;
			paused = false;
			g_StateStack.pop();
		}
		if (confirmMode == confirmState::QUIT) //quit
		{
			temp.StatePointer = Exit;
			g_StateStack.push(temp);
		}
		if (confirmMode == confirmState::QUIT_TO_MENU) //quit to menu
		{
			g_StateStack.swap(emptyStack);
			temp.StatePointer = Menu;
			g_StateStack.push(temp);
		}
		break;
	case 1:
		hideConfirmScreen();
		break;
	case 2:
		hideConfirmScreen();
		showConfirmScreen(confirmState::QUIT);
		temp.StatePointer = Confirm;
		g_StateStack.push(temp);
	}
}
void showConfirmScreen(confirmState m)
{
	confirmScreen = true;
	confirmMode = m;
}
void hideConfirmScreen()
{
	confirmScreen = false;
	confirmMode = confirmState::FALSE;
	g_StateStack.pop();
}

void handleConfirmEvent(int& choice)
{
	//Poll events
	while (SDL_PollEvent(&event))
	{
		//check events
		switch (event.type)
		{
		case SDL_QUIT: //User hit the X
			choice = 2;
			//confirmMode = confirmState::QUIT;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				//resize window
				SDL_SetWindowSize(gWindow, event.window.data1, event.window.data2);
				//SCREEN_WIDTH = event.window.data1;
				//SCREEN_HEIGHT = event.window.data2;
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) //esc
			{
				choice = 1;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			//yes button
			if (buttons[0].checkInside(mouseX, mouseY))
			{
				choice = 0;
			}
			//no button
			if (buttons[1].checkInside(mouseX, mouseY))
			{
				choice = 1;
			}
			break;
		}
	}
}
void Game()
{
	if (!initedLevel)
	{
		//init level
		initLevel();
		
	}

	//While application is running
	while (initedLevel && !quit &&!paused&& !confirmScreen&&!endGameScreen)
	{
		//Clear screen
		if(enemiesCount>=target){
			level++;
			target=20+level*20;
			enemiesCount=0;

		}
		SDL_ShowCursor(SDL_DISABLE);
		pickup=false;
		myPlayer.previousState = myPlayer.currentState;
		myPlayer.currentWeapon= Weapon[currentSlot].type;
		setCamera(camera, myPlayer);

		//Set the player back to idle mode
		myPlayer.currentState = playerState::IDLE;
		myPlayer.vx=0;
		myPlayer.vy=0;	
		myPlayer.speed=PLAYER_SPEED;
		handleGameEvent();
		handleGameInput();
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

		//Render ground
		gGroundTexture.setColor(107,107,107,192);
		for (int y = 0; y < LEVEL_SIZE; y++)
		{
			for (int x = 0; x < LEVEL_SIZE; x++)
			{
				gGroundTexture.render(camera, x * GROUND_TILE_SIZE, y * GROUND_TILE_SIZE, GROUND_TILE_SIZE, GROUND_TILE_SIZE);
			}
		}
		
		updateWeapon();
		updateBullet();
		updatePlayer();
		renderGameObject(camera, gRockTexture,rocks, gRockClips);
		updateEnemy();
		renderCrosshair();
		updateAnimation();
		//Update screen
		
		SDL_RenderPresent(gRenderer);
	}
	//get backdrop
	SDL_Surface* screencap = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, screencap->pixels, screencap->pitch);
	backdrop = SDL_CreateTextureFromSurface(gRenderer, screencap);
	SDL_FreeSurface(screencap);
	StateStruct temp;
	//handle menus
	if (paused)
	{
		temp.StatePointer = Pause;
		g_StateStack.push(temp);
	}
	if (confirmScreen)
	{
		temp.StatePointer = Confirm;
		g_StateStack.push(temp);
	}
	if (endGameScreen)
	{
		temp.StatePointer = EndGame;
		g_StateStack.push(temp);
	}
	if (quit)
	{
		while (!g_StateStack.empty())
		{
			g_StateStack.pop();
		}
	}
}
void handlePauseEvent()
{
	//Poll events
	while (SDL_PollEvent(&event))
	{
		//check events
		switch (event.type)
		{
		case SDL_QUIT: //User hit the X	
			showConfirmScreen(confirmState::QUIT);
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				//resize window
				SDL_SetWindowSize(gWindow, event.window.data1, event.window.data2);
				//SCREEN_WIDTH = event.window.data1;
				//SCREEN_HEIGHT = event.window.data2;
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) //esc
			{
				paused = false;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			//resume button
			if (buttons[0].checkInside(mouseX, mouseY))
			{
				paused = false;
			}
			//retry button
			if (buttons[1].checkInside(mouseX, mouseY))
			{
				showConfirmScreen(confirmState::RETRY);
			}
			//quit to menu button
			if (buttons[2].checkInside(mouseX, mouseY))
			{
				showConfirmScreen(confirmState::QUIT_TO_MENU);
			}
			break;
		}
	}
}
void Pause()
{
	//show back the cursor
	SDL_ShowCursor(SDL_ENABLE);
	//SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); //set the cursor to center of the window
	//set text positions
	int pausedOffset = SCREEN_HEIGHT / 3.5;
	int pausedX = SCREEN_WIDTH / 2;
	int pausedY = SCREEN_HEIGHT / 2 - pausedOffset;
	//add buttons
	//resume button
	int buttonpy = pausedY + SCREEN_HEIGHT / 5 + 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Resume", regularFont);
	buttons.push_back(myButton);
	//retry button
	buttonpy += 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Retry", regularFont);
	buttons.push_back(myButton);
	//quit to menu button
	buttonpy += 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Quit to menu", regularFont);
	buttons.push_back(myButton);
	while (paused && !confirmScreen)
	{
		mouses = SDL_GetMouseState(&mouseX, &mouseY);
		handlePauseEvent();
		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(gRenderer);
		//Render backdrop
		SDL_RenderCopy(gRenderer, backdrop, NULL, NULL);
		//Render paused text
		drawText(pausedX, pausedY, boldFontTitle, UIColor, "Paused", 1);
		//Render buttons
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i].checkButton(mouses, mouseX, mouseY);
			buttons[i].render(gRenderer);
		}
		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	//remove all buttons
	buttons.clear();
	clearScreen();
	StateStruct temp;
	if (confirmScreen)
	{
		temp.StatePointer = Confirm;
		g_StateStack.push(temp);
	}
	else
	{
		g_StateStack.pop();
	}
}
void showEndGamecreen(endState m)
{
	endGameScreen = true;
	endGameMode = m;
}

void hideEndGameScreen()
{
	endGameScreen = false;
	endGameMode = endState::FALSE;
	paused = false;
	confirmScreen = false;
	g_StateStack.pop();
}

void handleEndGameEvent(int& choice)
{
	//Poll events
	while (SDL_PollEvent(&event))
	{
		//check events
		switch (event.type)
		{
		case SDL_QUIT: //User hit the X
			choice = 2;
			//confirmMode = confirmState::QUIT;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				//resize window
				SDL_SetWindowSize(gWindow, event.window.data1, event.window.data2);
				//SCREEN_WIDTH = event.window.data1;
				//SCREEN_HEIGHT = event.window.data2;
			}
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			//retry button
			if (buttons[0].checkInside(mouseX, mouseY))
			{
				choice = 0;
			}
			//quit to menu button
			if (buttons[1].checkInside(mouseX, mouseY))
			{
				choice = 1;
			}
			break;
		}
	}
}
void EndGame()
{
	//show back the cursor
	SDL_ShowCursor(SDL_ENABLE);
	//SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); //set the cursor to center of the window

	//set text positions
	int textOffset = SCREEN_HEIGHT / 5;
	int textX = SCREEN_WIDTH / 2;
	int textY = SCREEN_HEIGHT / 2 - textOffset;
	//add buttons
	//yes button
	int buttonpy = textY + SCREEN_HEIGHT / 5 + 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Retry", regularFont);
	buttons.push_back(myButton);
	//no button
	buttonpy += 75;
	myButton.init(SCREEN_WIDTH / 2, buttonpy, 50, "Quit to menu", regularFont);
	buttons.push_back(myButton);
	int choice = -1; //0 for retry, 1 for quit to menu
	while (choice == -1)
	{
		mouses = SDL_GetMouseState(&mouseX, &mouseY);
		handleConfirmEvent(choice);
		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderClear(gRenderer);
		//Render backdrop
		SDL_RenderCopy(gRenderer, backdrop, NULL, NULL);

		std::string endGameText = "";
		SDL_Color endGameColor = { 255, 255, 255 };
		//Render text
		switch (endGameMode)
		{
		case endState::WIN:
			endGameText = "You win!";
			endGameColor = { 0, 200, 0 };
			break;
		case endState::LOSE:
			endGameText = "You died!";
			endGameColor = { 255, 0, 0 };
			break;
		}
		//Render overlay 
		//Render text
		drawText(textX, textY, boldFontTitle, endGameColor, endGameText, 1);
		//Render buttons
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i].checkButton(mouses, mouseX, mouseY);
			buttons[i].render(gRenderer);
		}

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	//remove all buttons
	buttons.clear();
	clearScreen();
	StateStruct temp;
	switch (choice)
	{
	case 0: //retry		
		hideEndGameScreen();
		initedLevel = false;
		break;
	case 1: //quit to menu
		hideEndGameScreen();
		showConfirmScreen(confirmState::QUIT_TO_MENU);
		temp.StatePointer = Confirm;
		g_StateStack.push(temp);
		break;
	case 2: //pressed exit
		hideEndGameScreen();
		showConfirmScreen(confirmState::QUIT);
		temp.StatePointer = Confirm;
		g_StateStack.push(temp);
	}
}
void createGameObjectRandom(gameObject source, std::vector<gameObject>& vectorList, int total, int minSize, int maxSize, int maxType)
{
	for (int i = 0; i < total; i++)
	{
		bool ok = false;
		int randomSize = 0;
		int randomX = 0;
		int randomY = 0;

		while (!ok)
		{
			randomSize = GetRandomInt(minSize, maxSize, 1);
			randomX = GetRandomInt(randomSize,LEVEL_WIDTH-randomSize, 1);
			randomY = GetRandomInt(randomSize,LEVEL_HEIGHT-randomSize, 1);
			if (calDistance(randomX, randomY, myPlayer.px, myPlayer.py) > randomSize + myPlayer.size)
			{
				ok = true;
			}
		}
		source.init(randomX, randomY, randomSize, 0);
		if (maxType != -1)
		{
			source.type = GetRandomInt(0, maxType-1, 1);
		}
		vectorList.push_back(source);
	}
}
void close(){
	gGroundTexture.free();
	gRockTexture.free();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
		//Free fonts
	TTF_CloseFont(boldFont);
	boldFont = NULL;
	TTF_CloseFont(boldFontSmall);
	boldFontSmall = NULL;
	TTF_CloseFont(boldFontLarge);
	boldFontLarge = NULL;
	TTF_CloseFont(boldFontTitle);
	boldFontTitle = NULL;
	TTF_CloseFont(regularFont);
	regularFont = NULL;
	TTF_CloseFont(regularFontSmall);
	regularFontSmall = NULL;
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
				showConfirmScreen(confirmState::QUIT);
			break;
				break;
			case SDL_MOUSEWHEEL:
				currentSlot=(currentSlot+1)%2;
				
			case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_f)
				pickup=true;
			if(event.key.keysym.sym == SDLK_r){
				myPlayer.reload=true;
				Weapon[currentSlot].reloadTimer=RELOAD_TIME;
			}
			if (event.key.keysym.sym == SDLK_ESCAPE) //esc
			{
				paused = true;
			}
		}
		
	}
}
void handleGameInput(){
	keys = SDL_GetKeyboardState(NULL);
	mouses = SDL_GetMouseState(&mouseX, &mouseY);
	if (keys[SDL_SCANCODE_W])
		if (myPlayer.py - myPlayer.size < 0)
		{
			myPlayer.vy = 0;
		}
		else
		{
			myPlayer.vy = -1;
			myPlayer.currentState = playerState::WALK;
		}
		if (keys[SDL_SCANCODE_S])
		if (myPlayer.py + myPlayer.size > LEVEL_HEIGHT)
		{
			myPlayer.vy = 0;
		}
		else
		{
			myPlayer.vy = 1;
			myPlayer.currentState = playerState::WALK;
		}
		if (keys[SDL_SCANCODE_A])
		if (myPlayer.px - myPlayer.size < 0)
		{
			myPlayer.vx = 0;
		}
		else
		{
			myPlayer.vx = -1;
			myPlayer.currentState = playerState::WALK;
		}
		if (keys[SDL_SCANCODE_D])
		if (myPlayer.px + myPlayer.size > LEVEL_WIDTH)
		{
			myPlayer.vx = 0;
		}
		else
		{
			myPlayer.vx = 1;
			myPlayer.currentState = playerState::WALK;
		}
		if ((keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_D]) || (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A])||
		(keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D]) || (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_A]))
			myPlayer.speed *= 0.7;
		if ((keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_S]) || (keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_D])){
			myPlayer.speed = 0;
			myPlayer.currentState = playerState::IDLE;
		}
		if (keys[SDL_SCANCODE_F]&&!pickup){
			pickup=true;
		}
		if (mouses & SDL_BUTTON(SDL_BUTTON_LEFT)&&myPlayer.attacking==false&&cooldown==0&&Weapon[currentSlot].Ammo>0){
			Weapon[currentSlot].currentState=weaponState::ATTACK;
			myPlayer.attacking=true;
			myPlayer.reload=false;
			if(Weapon[currentSlot].type!=2&&Weapon[currentSlot].type>0){
				Weapon[currentSlot].Ammo--;
				int acc=GetRandomFloat(-17,15,3);
				bullet myBullet(camera, Weapon[currentSlot], mouseX, mouseY,acc);
				bullets.push_back(myBullet);
			}
			else if(Weapon[currentSlot].type==2){
				Weapon[currentSlot].Ammo--;
				for(int i=-20;i<30;i+=10){
					bullet myBullet(camera, Weapon[currentSlot], mouseX, mouseY,i);
					bullets.push_back(myBullet);
				}
			}
			cooldown=Weapon[currentSlot].cd;
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
void loadSpritesheet(enum enemyState state, std::map<enemyState, LTexture>& spritesheet,
	std::map<enemyState, std::vector <SDL_Rect>>& spritesheetClip, int totalFrame)
{
	int w = spritesheet[state].getWidth() / totalFrame;
	int h = spritesheet[state].getHeight();
	for (int i = 0; i < totalFrame; i++)
	{
		spritesheetClip[state].push_back({ i * w, 0, w , h });
	}
}
void loadClips(LTexture& spritesheet, std::vector<SDL_Rect>& spritesheetClip, int totalClip)
{
	int w = spritesheet.getWidth() / totalClip;
	int h = spritesheet.getHeight();
	for (int i = 0; i < totalClip; i++)
	{
		spritesheetClip.push_back({ i * w, 0, w , h });
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
	if(myPlayer.health>0){
		myPlayer.direction = Weapon[currentSlot].direction;
		float dirX = myPlayer.vx * myPlayer.speed;
		float dirY = myPlayer.vy * myPlayer.speed;
		myPlayer.px += dirX;
		myPlayer.py += dirY;
		bool collided =false;
		for(gameObject i:rocks){
			if(myPlayer.checkCollision(i,0)){
			collided=true;
			break;
			}
		}
		if(collided){
			myPlayer.px -= dirX;
			myPlayer.py -= dirY;
		}
		setPlayerAnimation();
		myPlayer.updateRenderPosition();
		myPlayer.render(camera);
		myPlayer.hurted=false;
	}
	//myPlayer.drawHitbox(camera,gRenderer);
	if (myPlayer.health <= 0)
		showEndGamecreen(endState::LOSE);
}
void updateWeapon(){
	cooldown-=1;
	if(cooldown<=0)	cooldown=0;
	if(myPlayer.reload){
			if(Weapon[currentSlot].reloadTimer--==0){
				Weapon[currentSlot].Ammo=Weapon[currentSlot].clipsize;
				myPlayer.reload=false;
			}
	}
	Weapon[currentSlot].px = myPlayer.px+Weapon[currentSlot].size/2.5;
	Weapon[currentSlot].py = myPlayer.py+Weapon[currentSlot].size/4.5;
	Weapon[currentSlot].calRotation(camera, mouseX, mouseY);
	//std::cout<<Weapon[currentSlot].rotation<<" ";
	Weapon[currentSlot].setAnimation(WeaponTexture[myPlayer.currentWeapon],WeaponClip[myPlayer.currentWeapon][Weapon[currentSlot].currentFrame]);
	Weapon[currentSlot].updateRenderPosition();
	for(int i=0;i<(int)droppedWeapon.size();i++){
		if(myPlayer.px>=droppedWeapon[i].rx&&myPlayer.px<=droppedWeapon[i].rx+droppedWeapon[i].size*droppedWeapon[i].ratio&&myPlayer.py>=droppedWeapon[i].ry&&myPlayer.py<=droppedWeapon[i].ry+droppedWeapon[i].size&&pickup){
			if(Weapon[TOTAL_SLOT-1].type==-1){
				Weapon[TOTAL_SLOT-1]=droppedWeapon[i];
			}
			else{
				Weapon[currentSlot]=droppedWeapon[i];
			}
			droppedWeapon.erase(droppedWeapon.begin()+i);
			
		}
		droppedWeapon[i].render(camera);
	}
	Weapon[currentSlot].render(camera);
	if(myPlayer.attacking&&myPlayer.currentWeapon==0)
		Weapon[currentSlot].getHitbox();
}
void updateBullet()
{
	int i = 0;
	while (i < bullets.size())
	{
		if(bullets[i].direction==RIGHT){
		bullets[i].px += bullets[i].vx;
		bullets[i].py += bullets[i].vy;
		}
		else{
		bullets[i].px -= bullets[i].vx;
		bullets[i].py -= bullets[i].vy;
		}

		//delete bullet if its out of view
		if (bullets[i].calDistance(myPlayer) > SCREEN_WIDTH)
		{
			bullets.erase(bullets.begin() + i);
		}
		else
		{
			bool collised = false;
			//check bullet collision
			//rock
			for(gameObject j:rocks)
			{
				if (bullets[i].checkCollision(j,0))
				{
					collised = true;
					break;
				}
			}
			//enemies
			for (int j = 0; j < enemies.size(); j++)
			{
				if (bullets[i].checkCollision(enemies[j],0))
				{
					collised = true;
					enemies[j].hurt(bullets[i].damage);
					//remove zombie if it's health is below 0
					if (enemies[j].health < 0)
					{
						int odd=GetRandomInt(0,100,1);
						if(odd%(10-level)==0){
							Dummy.dropWeapon(enemies[j].rx,enemies[j].ry,level);
							Dummy.Texture=&WeaponTexture[Dummy.type];
							Dummy.currentClip=&WeaponClip[Dummy.type][Dummy.currentFrame];
							droppedWeapon.push_back(Dummy);
						}
						enemies[j].currentState=enemyState::DEAD;
					}
					break;
				}
				
			}
			//delete current bullet if it's collided with something
			if (collised)
			{
				bullets.erase(bullets.begin() + i);
			}
			else
			{
				//update bullet position
				bullets[i].updateRenderPosition();
				//render bullet
				gBulletTexture.render(camera, bullets[i].rx, bullets[i].ry, bullets[i].size, bullets[i].size, NULL, bullets[i].rotation);
				i++;
			}
		}
	}
}

void setPlayerAnimation(){
	myPlayer.setAnimation(gPlayerTexture[myPlayer.currentState],gPlayerClips[myPlayer.currentState][myPlayer.currentFrame]);
}
void updateEnemy(){
	if(allowSpawning)
		spawnEnemy();
	for(int i = 0 ; i < enemies.size();i++){
		if(enemies[i].attackTimer--<=0)
			enemies[i].attackTimer=0;
		if(enemies[i].currentState!=enemyState::DEAD&&enemies[i].checkCollision(myPlayer,1.5)&&enemies[i].attackTimer==0){
					myPlayer.hurt(enemies[i].damage);
					enemies[i].attackTimer=enemies[i].attackSpeed;
					if(enemies[i].type==3)
						enemies[i].currentState=enemyState::DEAD;
				}
		if(myPlayer.attacking){
			if(Weapon[currentSlot].type==0){
				if(Weapon[currentSlot].meleeAtack(enemies[i].px,enemies[i].py)){
					enemies[i].hurt(Weapon[currentSlot].damage);
				}
				if(enemies[i].health<=0){
						enemies[i].currentState=enemyState::DEAD;
					}
			}
		}
		enemies[i].setAnimation(gEnemyTexture[enemies[i].type][enemies[i].currentState],gEnemyClips[enemies[i].currentState][enemies[i].currentFrame]);
		enemies[i].move(myPlayer,rocks);
		enemies[i].render(camera);
		enemies[i].hurted=false;
		if(enemies[i].currentState==enemyState::DEAD){
			int odd=GetRandomInt(0,100,1);
				if(odd%(10-level)==0){
					Dummy.dropWeapon(enemies[i].rx,enemies[i].ry,level);
					Dummy.Texture=&WeaponTexture[Dummy.type];
					Dummy.currentClip=&WeaponClip[Dummy.type][Dummy.currentFrame];
					droppedWeapon.push_back(Dummy);
				}
			enemies.erase(enemies.begin() + i);
			enemiesCount++;
			allowSpawning=true;
		}
	}
	myPlayer.attacking=false;
}
void spawnEnemy(){
	while (enemies.size() < MAX_CURRENT_EMEMY + level*15)
		{
			myEnemy.initEnemy(level);
			enemies.push_back(myEnemy);
		}
		allowSpawning = false;
}
void updateAnimation(){
	myPlayer.currentFrame++;
	if (myPlayer.currentFrame > ANIMATION_FRAMES - 1)
	{
		myPlayer.currentFrame = 0;
	}
	for(int i = 0 ; i < enemies.size() ; i++){
		enemies[i].currentFrame++;
		if (enemies[i].currentFrame > ANIMATION_FRAMES - 1)
	{
		enemies[i].currentFrame = 0;
	}
	}
	if(Weapon[currentSlot].currentState== weaponState::ATTACK){
		Weapon[currentSlot].currentFrame++;
		if (Weapon[currentSlot].currentFrame > WEAPON_CLIP - 1)
		{
			Weapon[currentSlot].currentFrame = 0;
			Weapon[currentSlot].currentState= weaponState::NONE;
		}
	}
}
void renderCrosshair()
{
	if (myPlayer.reload||Weapon[currentSlot].Ammo == 0 )
	{
		gCrosshairTexture.setColor(150, 150, 150);
	}
	else
	{
		gCrosshairTexture.setColor(255, 255, 255);
	}
	gCrosshairTexture.render(mouseX + CROSSHAIR_SIZE / 2, mouseY + CROSSHAIR_SIZE / 2, CROSSHAIR_SIZE, CROSSHAIR_SIZE);
}