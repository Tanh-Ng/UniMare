#pragma once
#include "GameObject.h"
enum struct weaponState{
    NONE,INHAND,ATTACK
};
struct weapon : gameObject{
    weapon();
    float damage;
    double rotation;
    SDL_Point centerPoint;
    SDL_RendererFlip direction;
    weaponState currentState;
	weaponState previousState;
	int currentFrame;
    LTexture* Texture;
    SDL_Rect* currentClip;
    void setAnimation(LTexture& targetTexture,SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
    void calRotation(SDL_Rect& camera, int x, int y);
};