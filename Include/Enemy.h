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
    int orHealth;
    int health;
    bool hurted;
    bool isBoss;
    int attackSpeed;
    int attackTimer;
	SDL_RendererFlip direction;
    enemyState currentState;
    enemyState previousState;
    LTexture* currentTexture;
	SDL_Rect* currentClip;
	int currentFrame;
    enemy();  
    void initEnemy(int difficulty);
    void setAnimation(LTexture& targetTexture, SDL_Rect& targetClip);
    void render(SDL_Rect& camera);
    void move(gameObject player,std::vector<gameObject> rocks);
    void hurt(int damage);
};