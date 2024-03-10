#pragma once
#include "Graphics.h" 
#include <string>

using namespace std;

struct Vector2 {
	Vector2() { x = 0; y = 0; };
	Vector2(short ax, short ay) : x(ax), y(ay) {};
	short x;
	short y;
};

class GameSprites {
public:
	GameSprites();
	~GameSprites();
	static int LoadSprites();
	static int LoadSprite(short i);
	static const short NUM_SPRITES = 11;
	static string Paths[GameSprites::NUM_SPRITES];
	static Vector2 Dimensions[GameSprites::NUM_SPRITES];
	static SDL_Texture* Sprites[GameSprites::NUM_SPRITES];
private:
};