#pragma once
// creating a basic gameobject
#include "../Objects/ColourBlock.h"

class GameplayManager
{
public:
	GameplayManager() { Init(); };
	~GameplayManager() {};

	void Update();
	void Render();

private: 
	void Init();
	void SetupBoard();
	void DrawBoard();
	void DrawTitle();
	void AdvanceIntroAnim();

	static const int BOARD_ROWS = 8;
	static const int BOARD_COLS = 8;
	static const int BOARDSIZE = BOARD_ROWS * BOARD_COLS;
	static const int MAX_PATTERN_SIZE = BOARDSIZE;

	// 2D array
	ColourBlock ColourBlock[BOARD_ROWS][BOARD_COLS];

	int lvl = 1;

	// where in the animation or entry is the current index
	int currentPatternIndex = 0;
	bool introAnimating = true;
	bool isBlankFrame = true;
	int currentPatternSize = BOARD_ROWS;
};

