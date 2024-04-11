#include "Include/Global.h"
#include "Include/Graphics.h"
#include "Include/Player.h"
#include "Include/Enemy.h"
#include "Include/Weapon.h"
SDL_Window* gWindow=NULL;
SDL_Renderer* gRenderer=NULL;
LTexture gGroundTexture;
LTexture gRockTexture;
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//event handler
SDL_Event event;
//input handle
const Uint8* keys;
int mouseX;
int mouseY;
Uint32 mouses;
bool initedLevel=false;
bool quit=false;
bool allowSpawning=false;
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
const int ROCKS_CLIP=3;
gameObject rock;
std::vector <SDL_Rect> gRockClips;
std::vector<gameObject> rocks;
//weapons
weapon currentWeapon;
LTexture SaberTexture;
LTexture PistolTexture;
LTexture ShotgunTexture;
LTexture RifleTexture;
std::vector<std::vector <SDL_Rect>> WeaponClip(4);
bool init();
bool loadMedia();
void Game();
void createGameObjectRandom(gameObject source, std::vector<gameObject>& vectorList, int total, int minSize, int maxSize, int maxType);
void setCamera(SDL_Rect& camera, gameObject target);
void setPlayerAnimation();
void updatePlayer();
void updateWeapon();
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
	if (!gGroundTexture.loadFromFile("IMGfile/ground2.png"))
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
	if (!SaberTexture.loadFromFile("IMGfile/lightsaber.png"))
	{
		printf("Failed to load saber texture!\n");
		success = false;
	}
	else
	{
		loadClips(SaberTexture, WeaponClip[0], WEAPON_CLIP);
	}
	if (!ShotgunTexture.loadFromFile("IMGfile/shotgun.png"))
	{
		printf("Failed to load shotgun texture!\n");
		success = false;
	}
	else
	{
		loadClips(ShotgunTexture, WeaponClip[1], WEAPON_CLIP);
	}
	if (!PistolTexture.loadFromFile("IMGfile/pistol.png"))
	{
		printf("Failed to load pistol texture!\n");
		success = false;
	}
	else
	{
		loadClips(PistolTexture, WeaponClip[2], WEAPON_CLIP);
	}
	if (!RifleTexture.loadFromFile("IMGfile/rifle.png"))
	{
		printf("Failed to load rifle texture!\n");
		success = false;
	}
	else
	{
		loadClips(RifleTexture, WeaponClip[3], WEAPON_CLIP);
	}
	return success;
}
void Game()
{
	if (!initedLevel)
	{
		//init level
		//initLevel();
		SDL_WarpMouseInWindow(gWindow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		myPlayer.initPlayer();
		createGameObjectRandom(rock, rocks, MAX_ROCKS_NUM, MIN_ROCK_SIZE, MAX_ROCK_SIZE, ROCKS_CLIP);
		allowSpawning = true;
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
		myPlayer.vx=0;
		myPlayer.vy=0;	
		myPlayer.speed=PLAYER_SPEED;
		handleGameEvent();
		handleGameInput();
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
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
		updatePlayer();
		renderGameObject(camera, gRockTexture,rocks, gRockClips);
		updateEnemy();
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
		if (mouses & SDL_BUTTON(SDL_BUTTON_LEFT)){
			currentWeapon.currentState= weaponState::ATTACK;
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
	myPlayer.direction = currentWeapon.direction;
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
	//myPlayer.drawHitbox(camera,gRenderer);
}
void updateWeapon(){
	currentWeapon.px = myPlayer.px+currentWeapon.size/2.5;
	currentWeapon.py = myPlayer.py+currentWeapon.size/4.5;
	currentWeapon.calRotation(camera, mouseX, mouseY);
	currentWeapon.setAnimation(RifleTexture,WeaponClip[3][currentWeapon.currentFrame]);
	currentWeapon.updateRenderPosition();
	currentWeapon.render(camera);
	currentWeapon.drawHitbox(camera,gRenderer);
}
void setPlayerAnimation(){
	myPlayer.setAnimation(gPlayerTexture[myPlayer.currentState],gPlayerClips[myPlayer.currentState][myPlayer.currentFrame]);
}
void updateEnemy(){
	if(allowSpawning)
		spawnEnemy();
	for(int i = 0 ; i < enemies.size();i++){
		int tmp=enemies[i].type;
		enemies[i].setAnimation(gEnemyTexture[tmp][enemies[i].currentState],gEnemyClips[enemies[i].currentState][enemies[i].currentFrame]);
		enemies[i].move(myPlayer,rocks);
		enemies[i].render(camera);
		//enemies[i].drawHitbox(camera,gRenderer);
		
	}
}
void spawnEnemy(){
	while (enemies.size() <  MAX_CURRENT_EMEMY )
		{
			myEnemy.initEnemy();
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
	if(currentWeapon.currentState== weaponState::ATTACK){
		currentWeapon.currentFrame++;
		if (currentWeapon.currentFrame > WEAPON_CLIP - 1)
		{
			currentWeapon.currentFrame = 0;
			currentWeapon.currentState= weaponState::NONE;
		}
	}
}