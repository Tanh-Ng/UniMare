#include "Include/GameObject.h"

void gameObject::init(float x,float y,int s,int t){
    size=s;
    setPosition(x, y);
	setRenderPosition(x, y);
}
void gameObject::setPosition(float x, float y) {
	px = x;
	py = y;
}
void gameObject::setRenderPosition(float x, float y) {
	rx = int(x - size / 2);
	ry = int(y - size / 2);
}
void gameObject::updateRenderPosition() {
	rx = int(px - size / 2);
	ry = int(py - size / 2);
}
float gameObject::calDistance(gameObject target) {
	return sqrt(pow(target.px - px, 2) + pow(target.py - py, 2));
}
void gameObject::calRotation(int x , int y){
	int deltaX = px - x;
	int deltaY = py - y;
	rotation = (atan2(deltaY, deltaX) * 180.0) / PI;
}
void renderGameObject(SDL_Rect camera, LTexture& sourceTexture, std::vector<gameObject>& vectorList)
{
	if (vectorList.size() > 0)
	{
		for (int i = 0; i < vectorList.size(); i++)
		{
			sourceTexture.render(camera, vectorList[i].rx, vectorList[i].ry, vectorList[i].size, vectorList[i].size, NULL, vectorList[i].rotation);
		}
	}
}

void renderGameObject(SDL_Rect camera, LTexture& sourceTexture, std::vector<gameObject>& vectorList, std::vector<SDL_Rect> clips)
{
	if (vectorList.size() > 0 && clips.size() > 0)
	{
		for (int i = 0; i < vectorList.size(); i++)
		{
			sourceTexture.render(camera, vectorList[i].rx, vectorList[i].ry, vectorList[i].size, vectorList[i].size, &clips[vectorList[i].type], vectorList[i].rotation);
		}
	}
}