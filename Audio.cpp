//All music and sound effect is from https://freesound.org/
#include "Include/Audio.h"

audioManager::audioManager()
{
	//The sound effects that will be used
	reload = NULL;
	swapWeapon = NULL;
	gunEmpty = NULL;
	collectObject = NULL;
	gameLose = NULL;
	gameWin= NULL;
	playerHurt =NULL;
	gunshot = NULL;
	saberswing = NULL;
	hitEnemy = NULL; 
};
void audioManager::freeSound(Mix_Chunk* target)
{
	Mix_FreeChunk(target);
	target = NULL;
}
void audioManager::loadAudio()
{
	gunEmpty = Mix_LoadWAV("Sound/gun empty.wav");
	if (gunEmpty == NULL)
	{
		printf("Failed to load gun empty sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}

	reload = Mix_LoadWAV("Sound/reload.wav");
	if (reload == NULL)
	{
		printf("Failed to load reload sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}

	swapWeapon = Mix_LoadWAV("Sound/swap weapon.mp3");
	if (swapWeapon == NULL)
	{
		printf("Failed to load swap weapon sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}
	collectObject = Mix_LoadWAV("Sound/collect object.mp3");
	if (collectObject == NULL)
	{
		printf("Failed to load collect object effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}

	gameLose = Mix_LoadWAV("Sound/lose.wav");
	if (gameLose == NULL)
	{
		printf("Failed to load game lose effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}

	gameWin = Mix_LoadWAV("Sound/win.mp3");
	if (gameWin == NULL)
	{
		printf("Failed to load game win effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}
	gunshot = Mix_LoadWAV("Sound/gun empty.wav");
	if (gunshot == NULL)
	{
		printf("Failed to load gun shot sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}
	saberswing = Mix_LoadWAV("Sound/sabersound.mp3");
	if ( saberswing == NULL)
	{
		printf("Failed to load saber swing sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}
	playerHurt == Mix_LoadWAV("Sound/hurt.mp3");
	if (playerHurt == NULL)
	{
		printf("Failed to load hurt sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}
	hitEnemy == Mix_LoadWAV("Sound/bullethit.mp3");
	if (playerHurt == NULL)
	{
		printf("Failed to load bullet hit sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		exit(1);
	}
	//set sound effect volume
	Mix_Volume(-1, 64);
}
void audioManager::setSoundEffect(int channel, Mix_Chunk* soundEffect)
{
	//Mix_HaltChannel(channel);  //to stop the last sound effect from playing
	Mix_SetPanning(channel, 255, 255); //reset the directional sound effect
	Mix_PlayChannel(channel, soundEffect, 0);
}
void audioManager::playPlayerHurt()
{
		setSoundEffect(PLAYER_HURT_CHANNEL, playerHurt);
}
void audioManager::playReload()
{
	setSoundEffect(RELOAD_CHANNEL, reload);
}

void audioManager::stopReload()
{
	Mix_HaltChannel(RELOAD_CHANNEL); //to stop the last sound effect from playing
}

void audioManager::playSwapWeapon() {
	setSoundEffect(SWAP_WEAPON_CHANNEL, swapWeapon);
}

void audioManager::playGunshot() {
	setSoundEffect(-1, gunshot);
}
void audioManager::playSaberswing(){
	setSoundEffect(-1, saberswing);
}
void audioManager::playHitEnemy() {
	setSoundEffect(-1, hitEnemy);
}

void audioManager::playGunEmpty()
{
	setSoundEffect(4, gunEmpty);
}

void audioManager::playCollectObject()
{
	setSoundEffect(-1, collectObject);
}

void audioManager::playGameLose()
{
	setSoundEffect(-1, gameLose);
}

void audioManager::playGameWin()
{
	setSoundEffect(-1, gameWin);
}

void audioManager::freeAudio()
{
	freeSound(reload);
	freeSound(swapWeapon);
	freeSound(gunEmpty);
	freeSound(collectObject);
	freeSound(gameLose);
	freeSound(gameWin);
	freeSound(playerHurt);
	freeSound(gunshot);
	freeSound(saberswing);
	freeSound(hitEnemy);

}