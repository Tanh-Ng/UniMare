#pragma once
#include "Global.h"
#include "Graphics.h"

struct gameObject{
    int size;
    double rotation;
    //object's center position
    float px=0,py=0;
    //game object render offset (top left corner)
	int rx=0,ry=0;
    int type;
    void init(float x,float y,int s,int t=-1);
    void setPosition(float x, float y);
	void setRenderPosition(float x, float y);
    void updateRenderPosition();
    
};
void renderGameObject(SDL_Rect camera, LTexture& sourceTexture, std::vector<gameObject>& vectorList);
void renderGameObject(SDL_Rect camera, LTexture& sourceTexture, std::vector<gameObject>& vectorList, std::vector<SDL_Rect> clips);