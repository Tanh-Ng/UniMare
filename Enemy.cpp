#include "Include/Enemy.h"
enemy::enemy()
{
	speed=6;
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
	direction = RIGHT;
}
void enemy::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	currentTexture =&targetTexture;
	currentClip = &targetClip;
}
void enemy::render(SDL_Rect& camera)
{	
	if(currentTexture!=NULL)
		currentTexture->render(rx - camera.x,ry - camera.y,size, size,currentClip,0,NULL,direction);
}
void enemy::move(gameObject player){
	if(player.px < px)
		direction = LEFT;
	else direction = RIGHT;
	calRotation(player.px, player.py);
	float dirX = 0;
	float dirY = 0;
	dirX = -cos(rotation * PI / 180.0);
	dirY = -sin(rotation * PI / 180.0);
	px +=  dirX * speed;
	py += dirY * speed;
	setRenderPosition(px, py);
}