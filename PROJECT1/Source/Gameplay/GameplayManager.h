#pragma once
// creating a basic gameobject
#include "../Objects/ColourBlock.h"
#include "../GameObject.h"

class GameplayManager {
public:
	GameplayManager() { Init(); };
	~GameplayManager() {};
	
	void Update();
	void Render();

private: 
	void InitGameObjects();
	void DeleteGameObjects();
	GameObject* FirstGameObject;
	GameObject* LastGameObject;

	GameObject* AddGameObject(short x, short y, short type, bool visible);
	void Init(); 
	void DrawScore(); 
};