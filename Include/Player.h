#pragma once
#include "GameObject.h"
enum class pState{
    IDLE,WALK,DEAD
};
struct player: gameObject{
    pState currentState;
	pState previousState;
    LTexture* currentTexture;
	SDL_Rect* currentClip;
	int currentFrame;
	int currentTotalFrame;  

    void initPlayer();
    void setAnimation(LTexture& targetTexture, SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
}