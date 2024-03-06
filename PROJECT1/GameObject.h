#pragma once

class GameObject {
public:
	GameObject();
	GameObject(short ax, short by, short ctype, bool dvisible);
	~GameObject();
	GameObject* GetNext();
	void SetNext(GameObject* nx);
	enum ObjectTypes {
		PLAYER,
		INVADER_1,
		INVADER_2,
		INVADER_3,
		UFO,
		PLAYER_PROJECTILE
	};
	unsigned long long id;
	short x;
	short y;
	short type;
	bool visible;
private:
	GameObject* next;
}; 