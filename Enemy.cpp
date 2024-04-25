#include "Include/Enemy.h"
enemy::enemy()
{
    size = PLAYER_SIZE;
	hurted=false;	
}
void enemy::initEnemy()
{
	hurted=false;
	int randomX = GetRandomInt(size, LEVEL_WIDTH - size, 1);
	int randomY = GetRandomInt(size, LEVEL_HEIGHT - size, 1);
	setPosition(randomX, randomY);
	setRenderPosition(px, py);
	type = GetRandomInt(0,3,1);
	attackTimer=0;
	if(type==0){
		damage = 5;
		speed = GetRandomFloat(1,3,0.05);
		health= 200;
		attackSpeed=40;
	}
	else if(type==1){
		damage = 10;
		speed = GetRandomFloat(3,4,0.05);
		health = 200;
		attackSpeed=40;
	}
	else if(type==2){
		damage = 5;
		speed = GetRandomFloat(3,4,0.05);
		health = 200;
		attackSpeed=30;
	}
	else {
		damage = 5;
		speed = GetRandomFloat(4,6,0.05);
		health = 200;
	}
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
void enemy::move(gameObject player,std::vector<gameObject> rocks){
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
	bool collided =false;
	for(gameObject i:rocks){
		if(checkCollision(i,0)&&type!=2){
			collided=true;
			break;
			}
		}
		if(collided){
			px -= dirX*1.1*speed;
			py -= dirY*speed;
		}
	setRenderPosition(px, py);
}
void enemy::hurt(int damage){
	health-=damage;
	hurted=true;
}
