#pragma once
#include "GameObject.h"

enum struct weaponState{
    NONE,ATTACK
};
struct weapon : gameObject{
    weapon();
    int cd;
    float damage;
    double ratio = 1;
    int clipsize;
    SDL_Point centerPoint;
    SDL_Rect meleeHitbox;
    SDL_RendererFlip direction;
    weaponState currentState;
	weaponState previousState;
	int currentFrame;
    LTexture* Texture;
    SDL_Rect* currentClip;
    void initWeapon(int temp);
    void dropWeapon(float x,float y);
    void setAnimation(LTexture& targetTexture,SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
    void calRotation(SDL_Rect& camera, int x, int y);
    void getHitbox();
    bool meleeAtack(float px,float py);
};
struct bullet : gameObject
{
    float damage;
	int speed;
	float vx;
	float vy;
    SDL_RendererFlip direction;
	bullet(SDL_Rect& camera, weapon source, int targetX, int targetY,int accuracy);
};