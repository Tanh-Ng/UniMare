#pragma once
#include "Global.h"
#include "GameObject.h"
#include "Player.h"
extern player myPlayer;
enum class enemyState{
    ATTACK,WALK,DEAD
};
struct enemy: public gameObject{
    int speed;
    int health;
    SDL_RendererFlip previousDirection;
	SDL_RendererFlip currentDirection;
    enemyState currentState;
    enemyState previousState;
    LTexture* currentTexture;
	SDL_Rect* currentClip;
	int currentFrame;
    enemy();  
    void initEnemy();
    void setAnimation(LTexture& targetTexture, SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
};