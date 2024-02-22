#pragma once
// creating a basic gameobject
#include "../Objects/ColourBlock.h"

class GameplayManager {
public:
	GameplayManager() { Init(); };
	~GameplayManager() {};

	void Update();
	void Render();

private: 

	void Init(); 
	void DrawScore(); 
};