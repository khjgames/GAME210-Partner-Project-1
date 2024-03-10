#include "GameSprites.h"

string GameSprites::Paths[GameSprites::NUM_SPRITES] = { "ship.bmp","ship_2.bmp","blue_1.bmp","blue_2.bmp","green_1.bmp","green_2.bmp","pink_1.bmp","pink_2.bmp","ufo.bmp", "proj.bmp", "proj_2.bmp" };
Vector2 GameSprites::Dimensions[GameSprites::NUM_SPRITES];
SDL_Texture* GameSprites::Sprites[GameSprites::NUM_SPRITES];

GameSprites::GameSprites() {

}

GameSprites::~GameSprites(){

}

int GameSprites::LoadSprites(){
	for (short i = 0; i < NUM_SPRITES; i++) {
		// Loads ryu.png(Left side of the screen) in sprite1 	char* img_path = (char*)ImagePaths[i].data();
		if (LoadSprite(i) < 0) {
			return 1;
		}
	}
	return 0;
}

int GameSprites::LoadSprite(short i) { // pointer to the address someSprite 
	SDL_Surface* temp;
	const char* file = Paths[i].c_str();
	// Load the sprite image
	temp = SDL_LoadBMP(file);
	if (temp == NULL) {
		fprintf(stderr, "Couldn't load %s: %s", file, SDL_GetError());
		return (-1);
	}
	Dimensions[i].x = temp->w;
	Dimensions[i].y = temp->h;

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
	Sprites[i] = SDL_CreateTextureFromSurface(Graphics::renderer, temp);
	if (!Sprites[i]) {
		fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(temp);
		return (-1);
	}
	SDL_FreeSurface(temp);

	return (0);
}