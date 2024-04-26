#include "Include/Player.h"
player::player()
{
	init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, PLAYER_SIZE, -1);
	vx=0;vy=0;
	currentWeapon=0;
	speed=PLAYER_SPEED;
	currentFrame = 0;
	currentState = playerState::IDLE;
	previousState = playerState::IDLE;
	direction = RIGHT;
	attacking =false;
	reload=false;
}
void player::initPlayer()
{
	init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, PLAYER_SIZE,-1);
	vx=0;vy=0;
	currentWeapon=0;
	speed=PLAYER_SPEED;
	health=BASE_HEALTH;
	currentFrame = 0;
	currentState = playerState::IDLE;
	previousState = playerState::IDLE;
	direction = RIGHT;
	attacking = false;
	reload = false;
}
void player::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	currentTexture =&targetTexture;
	currentClip = &targetClip;
}
void player::render(SDL_Rect& camera)
{	
	if(currentTexture!=NULL){
		if(hurted){
			currentTexture->setColor(255,0,0,255);
		}
		else{
			currentTexture->setColor(255,255,255);
		}
	currentTexture->render(rx - camera.x,ry - camera.y,size, size,currentClip,0,NULL,direction);
	}
}
void player::hurt(int damage){
	hurted=true;
	health=health-damage;
}