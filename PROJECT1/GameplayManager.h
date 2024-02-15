#pragma once
// creating a basic gameobject
#include "..Objects/ColourBlock.h"

class GameplayManager
{
public:
	GameplayManager() { Init(); };
	~GameplayManager() {};

	void Update();

private: 
	void Init();

	static const int MAX_PATTERN_SIZE = 9;
	static const int BOARDSIZE = 16;
	static const int BOARD_ROWS = 8;
	static const int BOARD_COLS = 8;

	// 2D array
	ColourBlock ColourBlock[BOARD_ROWS][BOARD_COLS];

	// where in the animation or entry is the current index
	int currentPatternIndex = 0;
	bool isAnimating = true;
	bool isBlankFrame = true;
	int currentPatternSize = 4;

	int indexOrder[MAX_PATTERN_SIZE];
};

