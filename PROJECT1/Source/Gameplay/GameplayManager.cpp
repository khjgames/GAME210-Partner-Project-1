#include "GameplayManager.h"
#include <ctime>
#include <iostream>
#include "../Graphics.h"
// creating a basic gameobject
#include "../Objects/ColourBlock.h"

const int TITLE_OFFSET_X = 500;
const int TITLE_OFFSET_Y = 50;
const int XOFFSET = 400;
const int YOFFSET = 150;
const int BLOCK_SIZE = 50;
const int BLOCK_SPACING = 2;

void GameplayManager::Init()
{
	srand(time(NULL));

	SetupBoard();
}

void GameplayManager::Update()
{
	// update the board
	for (int x = 0; x < BOARD_ROWS; x++)
	{
		for (int y = 0; y < BOARD_COLS; y++)
		{
			// does nothing right now, but in the future may...
			ColourBlock[x][y].Update();

			// rgb values from hexadecimal, full colour = 0xFF no colour = 0x00
			ColourBlock[x][y].SetColor(0xFF, 0x99, 0x00);

			// Update the drawn paddles
			if (introAnimating)
			{
				// this first check is for the trail
				bool filledInside = (x + y) / 2 == currentPatternIndex - 1;

				if (filledInside) 
				{
					ColourBlock[x][y].SetColor(0x00, 0x00, 0x55);
				}

				// now the actual diagonal
				filledInside = (x + y) / 2 == currentPatternIndex;

				if (filledInside)
				{
					ColourBlock[x][y].SetColor(0x00, 0x00, 0xFF);
				}

			}
		}
	}

}

void GameplayManager::Render()
{
	DrawTitle();

	DrawBoard();
	
	if (introAnimating)
	{
		AdvanceIntroAnim();
	}
}

void GameplayManager::SetupBoard()
{
	for (int x = 0; x < BOARD_ROWS; x++)
	{
		for (int y = 0; y < BOARD_COLS; y++)
		{
			// sets location and size
			int xStart = XOFFSET + (BLOCK_SIZE * x) + (BLOCK_SPACING * x + 1);
			int yStart = YOFFSET + (BLOCK_SIZE * y) + (BLOCK_SPACING * y + 1);
			int width = BLOCK_SIZE;
			int height = BLOCK_SIZE;

			ColourBlock[x][y].Init(xStart, yStart, width, height);
		}
	}
}

void GameplayManager::DrawBoard()
{
	for (int x = 0; x < BOARD_ROWS; x++)
	{
		for (int y = 0; y < BOARD_COLS; y++)
		{
			bool filledInside = (x + y) / 2 == currentPatternIndex || (x + y) / 2 == currentPatternIndex - 1;

			ColourBlock[x][y].Draw(filledInside);
		}
	}
}

void GameplayManager::DrawTitle()
{
	// draw the title
	Graphics::DrawText("PROJECT 1", TITLE_OFFSET_X, TITLE_OFFSET_Y, 200, 50);
}


void GameplayManager::AdvanceIntroAnim()
{
	// toggle the blank frame on
	isBlankFrame = !isBlankFrame;

	if (!isBlankFrame)
	{
		currentPatternIndex++;
		if (currentPatternIndex >= MAX_PATTERN_SIZE || currentPatternIndex >= currentPatternSize)
		{
			// loop for now
			currentPatternIndex = 0;
			//introAnimating = false; // uncomment this to stop the intro anim loop.
		}
	}
}