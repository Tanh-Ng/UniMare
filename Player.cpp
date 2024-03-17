#include "Include/Player.h
void player::initPlayer()
{
	init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, SCREEN_HEIGHT / 7, -1);
	currentFrame = 0;
	currentTotalFrame = 0;
	currentState = playerState::IDLE;
	previousState = playerState::IDLE;
}
void player::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	currentTexture = &targetTexture;
	currentClip = &targetClip;
}
void player::render(SDL_Rect& camera)
{
	currentTexture->render(rx - camera.x, ry - camera.y, size, size, currentClip, rotation, NULL, SDL_FLIP_NONE);
}
