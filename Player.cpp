#include "Include/Player.h"
player::player()
{
	init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, PLAYER_SIZE, -1);
	vx=0;vy=0;
	speed=PLAYER_SPEED;
	currentFrame = 0;
	currentState = playerState::IDLE;
	previousState = playerState::IDLE;
	previousDirection = SDL_FLIP_NONE;
	currentDirection = SDL_FLIP_NONE;
}
void player::initPlayer()
{
	init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, SCREEN_HEIGHT/5 , -1);
	vx=0;vy=0;
	speed=PLAYER_SPEED;
	currentFrame = 0;
	currentState = playerState::IDLE;
	previousState = playerState::IDLE;
	previousDirection = SDL_FLIP_NONE;
	currentDirection = SDL_FLIP_NONE;
}
void player::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	currentTexture =&targetTexture;
	currentClip = &targetClip;
}
void player::render(SDL_Rect& camera)
{	
	if(currentTexture!=NULL)
	currentTexture->render(rx - camera.x,ry - camera.y,size, size,currentClip,0,NULL,currentDirection);
}
