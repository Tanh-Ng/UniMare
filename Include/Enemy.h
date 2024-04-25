#pragma once
#include "Global.h"
#include "GameObject.h"
#include "Player.h"
extern player myPlayer;
enum struct enemyState{
    WALK,DEAD
};
struct enemy: gameObject{
    int damage;
    float speed;
    int health;
    bool hurted;
    int attackSpeed;
    int attackTimer;
	SDL_RendererFlip direction;
    enemyState currentState;
    enemyState previousState;
    LTexture* currentTexture;
	SDL_Rect* currentClip;
	int currentFrame;
    enemy();  
    void initEnemy();
    void setAnimation(LTexture& targetTexture, SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
    void move(gameObject player,std::vector<gameObject> rocks);
    void hurt(int damage);
};