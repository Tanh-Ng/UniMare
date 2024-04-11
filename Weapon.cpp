#include "Include/Weapon.h"
weapon::weapon(){
    //init(LEVEL_WIDTH / 2, LEVEL_HEIGHT / 2, SABER_SIZE, -1);
    size= BASE_SIZE*1.5;
    direction = RIGHT;
    currentState = weaponState::NONE;
    previousState = weaponState::NONE;
    currentFrame = 0;
    rotation = 0;
}
void weapon::render(SDL_Rect& camera)
{	
	if(Texture!=NULL)
		Texture->render(rx - camera.x,ry - camera.y,size, size,currentClip,rotation,NULL,direction);
}
void weapon::setAnimation(LTexture& targetTexture, SDL_Rect& targetClip)
{
	Texture =&targetTexture;
	currentClip = &targetClip;
}
void weapon::calRotation(SDL_Rect& camera, int x, int y)
{
	centerPoint.x=px - size;
	centerPoint.y = py;
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
    if( direction == LEFT){
         px -= 2*size/2.5;
         rotation = 0 + (atan2(deltaY, deltaX) * 180.0000) / PI;
    }
    else 
	rotation = 180 + (atan2(deltaY, deltaX) * 180.0000) / PI;
    
}