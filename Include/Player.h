#pragma once
#include "GameObject.h"
enum class playerState{
    IDLE,WALK,DEAD
};
struct player: public gameObject{
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