#pragma once
#include "Global.h"
#include "Graphics.h"
enum class pState{
    IDLE,WALK,DEAD
};
struct player{
    int size;   
    //player position
    float px,py;
    //render offset
    int rx,ry;
    void initPlayer();
    void setPosition(float x,float y);
    void render(LTexture target);
    void render(SDL_Rect& camera);
}