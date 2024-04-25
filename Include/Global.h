#pragma once

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#pragma comment(lib, "SDL2_ttf.lib")
//include SDL stuff
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

//include ability to exit program
#include <stdio.h>
#include <stdlib.h>

//include other stuffs
#include <stack>
#include <string>
#include <ctime>
#include <cmath>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
//render window
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
//screen size	
static int SCREEN_WIDTH = 1280;
static int SCREEN_HEIGHT = 720;
const int V_BORDER = SCREEN_WIDTH / 50;
const int H_BORDER = SCREEN_HEIGHT / 50;
//dimensions of the level
const int GROUND_TILE_SIZE = 512;
const int LEVEL_SIZE = 15;
const int LEVEL_WIDTH = GROUND_TILE_SIZE * LEVEL_SIZE;
const int LEVEL_HEIGHT = GROUND_TILE_SIZE * LEVEL_SIZE;
const int BASE_SIZE = SCREEN_HEIGHT/9;
const int PLAYER_SIZE = BASE_SIZE;
const float PLAYER_SPEED = 9;
const int BASE_HEALTH = 100;
const int ENEMY_TYPES = 4;
const int MAX_CURRENT_EMEMY = 30;
const int MAX_ENEMY = 30;
const int MAX_ROCKS_NUM=10*LEVEL_SIZE;
const int ROCKS_CLIP=3;
static int MIN_ROCK_SIZE = BASE_SIZE;
static int MAX_ROCK_SIZE = BASE_SIZE * 3;
const int  WEAPON_CLIP = 9;
const int TOTAL_SLOT= 2;
const int BULLET_SPEED=10;
const int BULLET_SIZE=20;
static int SABER_SIZE = BASE_SIZE/2;
const int CROSSHAIR_SIZE = SCREEN_HEIGHT / 35;
const int RELOAD_TIME=100;
const double PI = 3.14159265358979323846;
const SDL_RendererFlip RIGHT = SDL_FLIP_NONE;
const SDL_RendererFlip LEFT = SDL_FLIP_HORIZONTAL;
int GetRandomInt(int min_value, int max_value, int step);
float GetRandomFloat(float min_value, float max_value, float step);
int map(int x, int in_min, int in_max, int out_min, int out_max);
float calOnScreenXPosition(SDL_Rect& camera, int targetX);
float calOnScreenYPosition(SDL_Rect& camera, int targetY);
float calDistance(float x1, float y1, float x2, float y2);
struct StateStruct
{
	void (*StatePointer)();
};