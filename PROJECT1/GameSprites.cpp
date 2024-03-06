#include "GameSprites.h"

int sprite_w, sprite_h;

string ImagePaths[NUM_SPRITES] = { "ship.png","blue_1.png","blue_2.png","green_1.png","green_2.png","pink_1.png","ufo.png" };
GameSprite Sprites[NUM_SPRITES];

GameSprites::GameSprites() {

}

GameSprites::~GameSprites(){

}

int GameSprites::LoadSprites(){
	for (int i = 0; i < NUM_SPRITES; i++) {
		// Loads ryu.png(Left side of the screen) in sprite1 
		if (LoadSprite(ImagePaths[i].c_str().data(), renderer, sprite1) < 0) {
			return 1;
		}
	}
	return 0;
}

int GameSprites::LoadSprite(char* file, SDL_Renderer* renderer, SDL_Texture*& someSprite) { // pointer to the address someSprite 
	SDL_Surface* temp;

	temp = IMG_Load(file);
	if (temp == NULL) {
		fprintf(stderr, "Couldn't load %s: %s", file, SDL_GetError());
		return (-1);
	}
	sprite_w = temp->w;
	sprite_h = temp->h;

	// Set transparent pixel as the pixel at (0,0)
	if (temp->format->palette) {
		SDL_SetColorKey(temp, SDL_TRUE, *(Uint8*)temp->pixels);
	}
	else {
		switch (temp->format->BitsPerPixel) {
		case 15:
			SDL_SetColorKey(temp, SDL_TRUE, (*(Uint16*)temp->pixels) & 0x00007FFF);
			break;
		case 16:
			SDL_SetColorKey(temp, SDL_TRUE, *(Uint16*)temp->pixels);
			break;
		case 24:
			SDL_SetColorKey(temp, SDL_TRUE, (*(Uint32*)temp->pixels) & 0x00FFFFFF);
			break;
		case 32:
			SDL_SetColorKey(temp, SDL_TRUE, *(Uint32*)temp->pixels);
			break;
		}
	}

	// Create textures from the image
	someSprite = SDL_CreateTextureFromSurface(renderer, temp);
	if (!someSprite) {
		fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(temp);
		return (-1);
	}
	SDL_FreeSurface(temp);

	return (0);
}