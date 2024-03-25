#include "Include/Enemy.h"
enemy::enemy()
{
	speed=0;
    size = PLAYER_SIZE+100;
	
}
void enemy::initEnemy()
{
	int randomX = GetRandomInt(size, LEVEL_WIDTH - size, 1);
	int randomY = GetRandomInt(size, LEVEL_HEIGHT - size, 1);
	setPosition(randomX, randomY);
	setRenderPosition(px, py);
	type = GetRandomInt(0,2,1);
	currentFrame = 0;
	currentState = enemyState::WALK;
	previousState = enemyState::WALK;
	previousDirection = RIGHT;
	currentDirection = RIGHT;
}
void enemy::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	currentTexture =&targetTexture;
	currentClip = &targetClip;
}
void enemy::render(SDL_Rect& camera)
{	
	if(currentTexture!=NULL)
	currentTexture->render(rx - camera.x,ry - camera.y,size, size,currentClip,0,NULL,currentDirection);
}
