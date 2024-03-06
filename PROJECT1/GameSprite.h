#pragma once
#include "SDL.h" 
#include <SDL_image.h>
#include <string>

using namespace std;

class GameSprite {
public:
	GameSprite(string spr_path);
	~GameSprite();
	SDL_Texture* GetSprite();
	void SetSprite(SDL_Texture* spr);
private:
	string sprite_path;
	SDL_Texture* sprite; 
};