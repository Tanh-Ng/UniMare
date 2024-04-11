#include "Include/Weapon.h"
weapon::weapon(){
    //init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, SABER_SIZE, -1);
    size = BASE_SIZE;
    direction = RIGHT;
    currentState = weaponState::NONE;
    previousState = weaponState::NONE;
    currentFrame = 0;
    rotation = 0;
}
void weapon::initWeapon(){
	type = GetRandomInt(0,3,1);
	if(type == 0)
		ratio = 1;
	else
		ratio = 1.783;
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