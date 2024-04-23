#pragma once
#include "GameObject.h"
struct bullet : gameObject
{
public:
	int speed;
	float vx;
	float vy;
	bullet(SDL_Rect& camera, gameObject source, int targetX, int targetY);
};
enum struct weaponState{
    NONE,ATTACK
};
struct weapon : gameObject{
    weapon();
    float damage;
    double ratio = 1;
    double rotation;
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
    void setAnimation(LTexture& targetTexture,SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
    void calRotation(SDL_Rect& camera, int x, int y);
    void getHitbox();
    bool meleeAtack(float px,float py);
};