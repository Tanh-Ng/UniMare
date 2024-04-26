#pragma once
#include "Global.h"
#include "GameObject.h"

extern SDL_Rect camera;

class audioManager {
public:
	audioManager();
	void loadAudio();
	void freeSound(Mix_Chunk* target);
	void setSoundEffect(int channel, Mix_Chunk* soundEffect);
	void playPlayerHurt();
	void playReload();
	void stopReload();
	void playCollectObject();
	void playGameLose();
	void playGameWin();
	void playHitEnemy();
	void playSaberswing();
	void playGunshot();
	void playSwapWeapon();
	void playGunEmpty();
	void freeAudio();
private:
	//Fixed channel for some sound effect
	const int RELOAD_CHANNEL = 0;
	const int SWAP_WEAPON_CHANNEL = 1;
	const int PLAYER_HURT_CHANNEL = 2;
	//The sound effects that will be used
	Mix_Chunk* reload;
	Mix_Chunk* swapWeapon;
	Mix_Chunk* gunEmpty;
	Mix_Chunk* collectObject;
	Mix_Chunk* gameLose;
	Mix_Chunk* gameWin;
	Mix_Chunk* playerHurt;
	Mix_Chunk* gunshot;
	Mix_Chunk* saberswing;
	Mix_Chunk* hitEnemy;
};