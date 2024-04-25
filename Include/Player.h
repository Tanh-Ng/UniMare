#pragma once
#include "GameObject.h"
enum struct playerState{
    IDLE,WALK,DEAD,RELOAD
};
struct player: gameObject{
    int vx,vy;
    int health;
    int currentWeapon;
    bool hurted;
    bool attacking;
    float speed;
    SDL_RendererFlip direction;
    playerState currentState;
	playerState previousState;
    LTexture* currentTexture;
	SDL_Rect* currentClip;
	int currentFrame;
    player();  
    void initPlayer();
    void setAnimation(LTexture& targetTexture, SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
    void hurt(int damage);
};