#include "GameplayManager.h"
#include <ctime>
#include <iostream>
#include "../Graphics.h"
// creating a basic gameobject
#include "../Objects/ColourBlock.h"

void GameplayManager::Init(){
	srand(time(NULL));
} 
// 11x4 space invaders
// + 1 ship
// + 1 ufo

void GameplayManager::Update(){
	// update states of all game objects
}

void GameplayManager::Render(){
	// render next frame / all game objects	
	DrawScore();
}

void GameplayManager::DrawScore(){
	// draw the score
	Graphics::DrawText("HIGH SCORE AND SCORE WILL GO HERE ISH", 250, 100, 500, 50);
}