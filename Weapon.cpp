#include "Include/Weapon.h"
weapon::weapon(){
    //init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, SABER_SIZE, -1);
    direction = RIGHT;
    currentState = weaponState::NONE;
    previousState = weaponState::NONE;
    currentFrame = 0;
    rotation = 0;
}
void weapon::dropWeapon(float x,float y){
	rx=x;
	ry=y;
	type = GetRandomInt(1,3,1);
	initWeapon(type);
}
void weapon::initWeapon(int temp){
	type=temp;
	if(type == 0){
		ratio = 1;
		damage= 10;
		size = BASE_SIZE*1.5;
	}
	else{
		ratio = 1.783;
		
		if(type==1){
			damage=10;
			clipsize=5;
			size = BASE_SIZE/1.7;
		}
		else if (type==2){
			damage=10;
			clipsize=5;
			size = BASE_SIZE/2;
		}
		else{
			damage=10;
			clipsize=5;
			size = BASE_SIZE/1.5;
		}

	}

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
    