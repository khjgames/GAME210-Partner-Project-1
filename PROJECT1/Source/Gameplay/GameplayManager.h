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
	void LoadLevel(short X_GAP, short Y_GAP, short PER_ROW, short NUM_ROWS);

	int CurSpeed;
	int AccelPerWave;
	int AccelLowSurvivors;
	int FrameSpeedBonus;

private: 
	void InitGameObjects();
	void DeleteGameObjects();

	GameObject* FirstGameObject;
	GameObject* LastGameObject;

	GameObject* AddGameObject(short x, short y, short type, bool visible);

	void Init(); 
	void DrawScore(); 
};