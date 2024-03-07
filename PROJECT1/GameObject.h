#pragma once
#include "SDL.h"

class GameObject {
public:
	GameObject();
	GameObject(short ax, short by, short ctype, bool dvisible);
	~GameObject();
	GameObject* GetNext();
	void SetNext(GameObject* nx);
	enum ObjectTypes {
		PLAYER,
		INVADER_1A,
		INVADER_1B,
		INVADER_2A,
		INVADER_2B,
		INVADER_3A,
		INVADER_3B,
		UFO,
		PLAYER_PROJECTILE
	};
	unsigned long long id;
	SDL_Rect transform;
	short type;
	bool visible;
	bool loaded;
private:
	GameObject* next;
}; 