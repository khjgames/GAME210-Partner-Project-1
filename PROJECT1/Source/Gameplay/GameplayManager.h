#pragma once
// creating a basic gameobject
#include "../Objects/ColourBlock.h"
#include "../GameObject.h"
#include "../Leaderboard.h"
#include <SDL_ttf.h>

class GameplayManager {
public:
	GameplayManager() { Init(); ArialFont = TTF_OpenFont("arialbd.ttf", 24); }; // fixed memory leak
	~GameplayManager() { TTF_CloseFont(ArialFont); };
	
	void Update();
	void Render();
	void LoadLevel(short X_GAP, short Y_GAP, short PER_ROW, short NUM_ROWS);

	TTF_Font* ArialFont;
private: 
	void InitGameObjects();
	void DeleteGameObjects();

	GameObject* FirstGameObject;
	GameObject* LastGameObject;

	GameObject* AddGameObject(short x, short y, short type, bool visible);

	Leaderboard lb = Leaderboard("SaveData.txt");

	int LastSpriteAnim;
	void Init(); 
	void DrawScore(); 
	void DrawMenu();
	void DrawHighScore();
	void DrawLeaderboard();
};