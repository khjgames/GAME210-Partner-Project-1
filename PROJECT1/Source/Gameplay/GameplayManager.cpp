#include "GameplayManager.h"
#include <ctime>
#include <iostream>
#include "../Graphics.h"
// creating a basic gameobject
#include "../Objects/ColourBlock.h"
#include "../GameObjectDrawer.h"
#include "../GameSpaces.h"
#include "../EventHandler.h"

using namespace GameTime;
using namespace GameSettings;

GameObject* GameplayManager::AddGameObject(short x, short y, short type, bool visible){
	GameObject* NextGameObject = new GameObject(x, y, type, visible);
	LastGameObject->SetNext(NextGameObject); // append new dynamic memory allocated object to end of linked-list
	LastGameObject = NextGameObject;
	return LastGameObject;
}; // I don't need a RemoveGameObject function at present.

void GameplayManager::DeleteGameObjects(){
	GameObject* CurGameObject = FirstGameObject;
	while (true) { // Delete all GameObjects from first to last 
		if (CurGameObject == nullptr) break;
		GameObject* NextGameObject = CurGameObject->GetNext();
		delete CurGameObject; 
		if (NextGameObject == nullptr) break; // we reached the last object
		else CurGameObject = NextGameObject;
	}
	FirstGameObject = nullptr;
	LastGameObject = nullptr;
}

void GameplayManager::InitGameObjects(){
	FirstGameObject = new GameObject(Graphics::WINDOW_WIDTH/2-19, Graphics::WINDOW_HEIGHT - 90, GameObject::ObjectTypes::PLAYER_1, true);
	LastGameObject = FirstGameObject;
	AddGameObject(Graphics::WINDOW_WIDTH / 2 + 119, Graphics::WINDOW_HEIGHT - 90, GameObject::ObjectTypes::PLAYER_2, true);
	AddGameObject(Graphics::WINDOW_WIDTH / 2 - 19, Graphics::WINDOW_HEIGHT - 150, GameObject::ObjectTypes::PLAYER_1_PROJECTILE, false);
	AddGameObject(Graphics::WINDOW_WIDTH / 2 + 119, Graphics::WINDOW_HEIGHT - 150, GameObject::ObjectTypes::PLAYER_2_PROJECTILE, false);
}

void GameplayManager::LoadLevel(short X_GAP, short Y_GAP, short PER_ROW, short NUM_ROWS){
	for (int a = 0; a < (PER_ROW); a++) {
		for (int b = 0; b < NUM_ROWS; b++) {
			AddGameObject(Graphics::WINDOW_WIDTH / 2 - 16 - (PER_ROW / 2 - a) * X_GAP, Graphics::WINDOW_HEIGHT / 3 - ((2 - b) * Y_GAP), 2 + (b / 2 % 3 * 2), true);
		}
	}
}

void GameplayManager::Init(){
	srand(time(NULL));
	LastTick = tick(); LastSpriteAnim = LastTick;
	GameSprites::LoadSprites();
	DeleteGameObjects();
	InitGameObjects();
	CurSpeed = 1;
	AccelPerWave = 1;
	AccelLowSurvivors = 1;
	FrameSpeedBonus = 0;
	LoadLevel(52, 52, 13, 6); // Load in enemy invaders also load in time, and score, when you resume a level resume from the start of the level you left off on.
} 
// 11x4 space invaders
// + 1 ship
// + 1 ufo

void MoveInvaders(){

}

void ManageProjectile(GameObject* CurGameObject, GameObject* FirstGameObject) {
	switch (CurGameObject->type) {
	case GameObject::ObjectTypes::PLAYER_1_PROJECTILE: { 
		if (CurGameObject->visible == true) { // Projectile fly and explode
			CurGameObject->transform.y = max(90, CurGameObject->transform.y - ProjectileSpeed);
			if (CurGameObject->transform.y == 90) CurGameObject->visible = false;
		}
		//
		if (EventHandler::events[GameEvents::SPACE_PRESSED] == true) {
			if (CurGameObject->visible == false){ // Projectile spawn
				CurGameObject->transform.x = FirstGameObject->transform.x + FirstGameObject->transform.w/2 - 2;
				CurGameObject->transform.y = FirstGameObject->transform.y;
				CurGameObject->visible = true;
			}
		}
		break; 
	}
	case GameObject::ObjectTypes::PLAYER_2_PROJECTILE: { 
		GameObject* SecondGameObject = FirstGameObject->GetNext();
		if (SecondGameObject != nullptr) {
			if (CurGameObject->visible == true) { // Projectile fly and explode
				CurGameObject->transform.y = max(90, CurGameObject->transform.y - ProjectileSpeed);
				if (CurGameObject->transform.y == 90) CurGameObject->visible = false;
			} 
			//
			if ((EventHandler::events[GameEvents::KP0_PRESSED] == true) || (EventHandler::events[GameEvents::KP1_PRESSED] == true) ||
				(EventHandler::events[GameEvents::KP2_PRESSED] == true) || (EventHandler::events[GameEvents::KP3_PRESSED] == true)) { 
				if (CurGameObject->visible == false) { // Projectile spawn
						CurGameObject->transform.x = SecondGameObject->transform.x + FirstGameObject->transform.w / 2 - 2;
						CurGameObject->transform.y = SecondGameObject->transform.y;
						CurGameObject->visible = true;
				}
			}
		}
		break; 
	}
	}

}

void HandleInput(GameObject* CurGameObject){
	switch (CurGameObject->type) {
	case GameObject::ObjectTypes::PLAYER_1: {
		if (EventHandler::events[GameEvents::A_PRESSED] == true) {
			CurGameObject->transform.x = max(0, CurGameObject->transform.x - ShipSpeed);
		}
		if (EventHandler::events[GameEvents::D_PRESSED] == true) {
			CurGameObject->transform.x = min(Graphics::WINDOW_WIDTH - CurGameObject->transform.w, CurGameObject->transform.x + ShipSpeed);
		}
		break;
	}
	case GameObject::ObjectTypes::PLAYER_2: {
		if (EventHandler::events[GameEvents::LEFT_PRESSED] == true) {
			CurGameObject->transform.x = max(0, CurGameObject->transform.x - ShipSpeed);
		}
		if (EventHandler::events[GameEvents::RIGHT_PRESSED] == true) {
			CurGameObject->transform.x = min(Graphics::WINDOW_WIDTH - CurGameObject->transform.w, CurGameObject->transform.x + ShipSpeed);
		}
		break;
	}
	}
}

void GameplayManager::Update(){
	GameObject* CurGameObject = FirstGameObject;

	while (true) {
		if (CurGameObject == nullptr) break;
		GameObject* NextGameObject = CurGameObject->GetNext();
		switch (CurGameObject->type) {
		case GameObject::ObjectTypes::PLAYER_1:
			HandleInput(CurGameObject);
			break;
		case GameObject::ObjectTypes::PLAYER_2:
			HandleInput(CurGameObject);
			break;
		case GameObject::ObjectTypes::INVADER_1A:
			break;
		case GameObject::ObjectTypes::INVADER_1B:
			break;
		case GameObject::ObjectTypes::INVADER_2A:
			break;
		case GameObject::ObjectTypes::INVADER_2B:
			break;
		case GameObject::ObjectTypes::INVADER_3A:
			break;
		case GameObject::ObjectTypes::INVADER_3B:
			break;
		case GameObject::ObjectTypes::UFO:
			break;
		case GameObject::ObjectTypes::PLAYER_1_PROJECTILE:
			ManageProjectile(CurGameObject, FirstGameObject);
			break;
		case GameObject::ObjectTypes::PLAYER_2_PROJECTILE:
			ManageProjectile(CurGameObject, FirstGameObject);
			break;
		}
		if (CurGameObject->visible == true) {
			
		}
		//
		if (NextGameObject == nullptr) break; // we reached the last object
		else CurGameObject = NextGameObject;
	}
	LastTick = tick();
}

void GameplayManager::Render(){
	// render next frame / all game objects	
	DrawScore();
	GameObjectDrawer::DrawGameObjects(FirstGameObject);
}

void GameplayManager::DrawScore(){
	// draw the score
	Graphics::DrawText("HIGH SCORE AND SCORE WILL GO HERE ISH", 250, 50, 500, 50);
}