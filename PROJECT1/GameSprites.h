#pragma once
#include "GameSprite.h"

const short NUM_SPRITES = 7;

class GameSprites {
public:
	GameSprites();
	~GameSprites();
	static int LoadSprites();
	static int LoadSprite(char* file, SDL_Renderer* renderer, SDL_Texture*& someSprite);
private:
};