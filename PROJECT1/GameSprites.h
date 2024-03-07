#pragma once
#include "Graphics.h" 
#include <SDL_image.h>
#include <string>

using namespace std;

const short NUM_SPRITES = 7;

class GameSprites {
public:
	GameSprites();
	~GameSprites();
	static int LoadSprites();
	static int LoadSprite(char* file, SDL_Texture*& someSprite);
private:
};