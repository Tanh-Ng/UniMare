#include "Include/Weapon.h"
bullet::bullet(SDL_Rect& camera, weapon source, int targetX, int targetY,int accuracy)
{
	size = BULLET_SIZE;
	direction=source.direction;
	damage=source.damage;
	px = source.px-20;
	if(direction==LEFT)
		px+=90;
	py = source.py+source.centerPoint.y-20;
	setRenderPosition(px, py);
	speed = 20;
	rotation = source.rotation+accuracy;
	float screenX = calOnScreenXPosition(camera, px);
	float screenY = calOnScreenYPosition(camera, py);
	float tempX = targetX - screenX;
	float tempY = targetY - screenY;
	float dirX = 0;
	float dirY = 0;
	dirX = cos(rotation * PI / 180.0);
	dirY = sin(rotation * PI / 180.0);
	vx = dirX * speed;
	vy = dirY * speed;
}
weapon::weapon(){
    //init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, SABER_SIZE, -1);
	type=-1;
    direction = RIGHT;
    currentState = weaponState::NONE;
    previousState = weaponState::NONE;
    currentFrame = 0;
    rotation = 0;
	level=0;
}
void weapon::dropWeapon(float x,float y,int difficulty){
	rx=x;
	ry=y;
	level = getLevel(difficulty);
	type = GetRandomInt(0,3,1);
	initWeapon(type);
}
void weapon::initWeapon(int temp){
	type=temp;
	if(type == 0){
		cd=20;
		ratio = 1;
		damage= 15+2*level;
		size = BASE_SIZE*1.5;
		clipsize=1;
	}
	else{
		ratio = 1.783;
		reloadTimer=RELOAD_TIME;
		if(type==1){
			cd=30;
			damage=30+10*level;
			clipsize=10;
			size = BASE_SIZE/2;
		}
		else if (type==2){
			cd=30;
			damage=10+5*level;
			clipsize=7;
			size = BASE_SIZE/1.7;
		}
		else{
			cd=10;
			damage=15+3*level;
			clipsize=30;
			size = BASE_SIZE/1.5;
		}
	}
	Ammo=clipsize;
}
void weapon::render(SDL_Rect& camera)
{	
	if(Texture!=NULL)
		Texture->render(rx - camera.x,ry - camera.y,size*ratio, size,currentClip,rotation,&centerPoint,direction);
}
void weapon::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	Texture =&targetTexture;
	currentClip = &targetClip;
}
void weapon::calRotation(SDL_Rect& camera, int x, int y)
{
	centerPoint.y = size/2;
	int deltaX;
	int deltaY;
	//If player reached the edge of the level
	if (camera.x <= 0 || camera.y <= 0 || camera.x >= LEVEL_WIDTH - camera.w || camera.y >= LEVEL_HEIGHT - camera.h)
	{
		deltaX = px - camera.x - x;
		deltaY = py - camera.y - y;
	}
	else
	{
		deltaX = (SCREEN_WIDTH / 2 - size / 2) - x;
		deltaY = (SCREEN_HEIGHT / 2 - size / 2) - y;
	}
    if( x > SCREEN_WIDTH / 2){
		direction = RIGHT;
		centerPoint.x=0;
		rotation = 180 + (atan2(deltaY, deltaX) * 180.0000) / PI;
    }
    else {
		direction = LEFT;
		centerPoint.x=ratio*size;
         px -= 2*ratio*size/2.5;
         rotation = 0 + (atan2(deltaY, deltaX) * 180.0000) / PI;
	}
}
void weapon::getHitbox(){
	if(direction==RIGHT){
		meleeHitbox.x = rx+centerPoint.x+size/2*cos(rotation*PI/180.000)-size/2;
		meleeHitbox.y = ry+centerPoint.y+size/2*sin(rotation*PI/180.000)-size/2;
	}
	else{
		meleeHitbox.x = rx+centerPoint.x-size/2*cos(rotation*PI/180.000)-size/4;
		meleeHitbox.y = ry+centerPoint.y-size/2*sin(rotation*PI/180.000)-size/4;
	}
	
	meleeHitbox.w=size*ratio/1.5;
	meleeHitbox.h=size/1.5;
}
bool weapon::meleeAtack(float x, float y){
	if((x>=meleeHitbox.x&&x<=meleeHitbox.x+meleeHitbox.w)&&(y>=meleeHitbox.y&&y<=meleeHitbox.y+meleeHitbox.h))
		return true;
	return false;
}
int weapon::getLevel(int difficulty){
	float r=GetRandomFloat(0,1,0.05);
	float base = 0.2;
	float multiplier = 0.2+0.2*difficulty;
	for(int i=0 ; i<5;i++){
		if(r<base)
			return i;
		base+=multiplier;
	}
	return 0;
}