#include "GameSprite.h"

SDL_Texture* GameSprite::GetSprite() {
	return sprite;
}

void GameSprite::SetSprite(SDL_Texture* spr) {
	sprite = spr;
}

GameSprite::GameSprite(string spr_path) {
	sprite_path = spr_path;
} // Default Constructor

GameSprite::~GameSprite() {

} // Default Destructor