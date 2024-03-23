#pragma once
#include "GameObject.h"
enum class playerState{
    IDLE,WALK,DEAD
};
struct player: public gameObject{
    int vx,vy;
    int speed;
    SDL_RendererFlip previousDirection;
	SDL_RendererFlip currentDirection;
    playerState currentState;
	playerState previousState;
    LTexture* currentTexture;
	SDL_Rect* currentClip;
	int currentFrame;
    player();  
    void initPlayer();
    void setAnimation(LTexture& targetTexture, SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
};