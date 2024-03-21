#include "Include/Player.h"
player::player()
{
	init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, PLAYER_SIZE, -1);
	currentFrame = 0;
	currentTotalFrame = 0;
	currentState = playerState::IDLE;
	previousState = playerState::IDLE;
}
void player::initPlayer()
{
	init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, SCREEN_HEIGHT / 5, -1);
	currentFrame = 0;
	currentTotalFrame = 0;
	currentState = playerState::IDLE;
	previousState = playerState::IDLE;
}
void player::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	currentTexture =&targetTexture;
	currentClip = &targetClip;
}
void player::render(SDL_Rect& camera)
{	
	if(currentTexture!=NULL)
	currentTexture->renderchar(rx - camera.x,ry - camera.y,size, size,currentClip);
}
