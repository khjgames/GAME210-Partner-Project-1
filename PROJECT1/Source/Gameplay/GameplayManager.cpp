#include "GameplayManager.h"
#include <ctime>
#include <iostream>
#include "../Graphics.h"
// creating a basic gameobject
#include "../Objects/ColourBlock.h"
#include "../GameObjectDrawer.h"

GameObject* GameplayManager::AddGameObject(short x, short y, short type, bool visible){
	GameObject* NextGameObject = new GameObject(x, y, type, visible);
	LastGameObject->SetNext(NextGameObject); // append new dynamic memory allocated object to end of linked-list
	LastGameObject = NextGameObject;
	return LastGameObject;
}; // I don't need a RemoveGameObject function at present.

void GameplayManager::DeleteGameObjects(){
	GameObject* CurGameObject = FirstGameObject;
	while (true) { // Delete all GameObjects from first to last
		GameObject* NextGameObject = CurGameObject->GetNext();
		delete CurGameObject; 
		if (NextGameObject == nullptr) break; // we reached the last object
		else CurGameObject = NextGameObject;
	}
	FirstGameObject = nullptr;
	LastGameObject = nullptr;
}

void GameplayManager::InitGameObjects(){
	FirstGameObject = new GameObject(0, 0, GameObject::ObjectTypes::PLAYER, false);
	LastGameObject = FirstGameObject;
}

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
	GameObjectDrawer::DrawGameObjects(FirstGameObject);
}

void GameplayManager::DrawScore(){
	// draw the score
	Graphics::DrawText("HIGH SCORE AND SCORE WILL GO HERE ISH", 250, 100, 500, 50);
}