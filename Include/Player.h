#pragma once
#include "GameObject.h"
enum struct playerState{
    IDLE,WALK,DEAD
};
struct player: gameObject{
    int vx,vy;
    int health;
    float speed;
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