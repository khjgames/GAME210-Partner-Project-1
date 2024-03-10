#include "GameplayManager.h"
#include <ctime>
#include <iostream>
#include "../Graphics.h"
// creating a basic gameobject
#include "../Objects/ColourBlock.h"
#include "../GameObjectDrawer.h"
#include "../EventHandler.h"
#include "../GameSpaces.h"

using namespace GameTime;
using namespace GameVars;

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
			AddGameObject(Graphics::WINDOW_WIDTH / 2 - 16 - (PER_ROW / 2 - a) * X_GAP, Graphics::WINDOW_HEIGHT / 4 - ((2 - b) * Y_GAP), 2 + (b / 2 % 3 * 2), true);
		}
	}
}

void GameplayManager::Init(){
	srand(time(NULL));
	LastTick = tick(); LastSpriteAnim = LastTick;
	GameSprites::LoadSprites();
	DeleteGameObjects();
	InitGameObjects();
	InitGameVars();		// Reset and init a bunch of game vars

	//LoadLevel(52, 52, 13, 6); // Load in enemy invaders also load in time, and score, when you resume a level resume from the start of the level you left off on.
} 
// 11x4 space invaders
// + 1 ship
// + 1 ufo

void SpawnProjectile(GameObject* ProjGameObject, GameObject* PlayerGameObject){
	if (ProjGameObject->visible == false && PlayerGameObject->visible == true) { // Projectile spawn
		ProjGameObject->transform.x = PlayerGameObject->transform.x + PlayerGameObject->transform.w / 2 - 2;
		ProjGameObject->transform.y = PlayerGameObject->transform.y;
		ProjGameObject->visible = true;
	}
}

void MoveProjectile(GameObject* ProjGameObject) {
	if (ProjGameObject->visible == true) { // Projectile fly and explode
		ProjGameObject->transform.y = max(75, ProjGameObject->transform.y - ProjectileSpeed);
		if (ProjGameObject->transform.y == 75) ProjGameObject->visible = false;
	}
}

void ManageProjectile(GameObject* ProjGameObject, GameObject* PlayerGameObject) {
	MoveProjectile(ProjGameObject);
	switch (ProjGameObject->type) {
	case GameObject::ObjectTypes::PLAYER_1_PROJECTILE: { 
		if (EventHandler::events[GameEvents::SPACE_PRESSED] == true) { SpawnProjectile(ProjGameObject, PlayerGameObject); } 
		break; 
	}
	case GameObject::ObjectTypes::PLAYER_2_PROJECTILE: { 
		if ((EventHandler::events[GameEvents::KP0_PRESSED] == true) || (EventHandler::events[GameEvents::KP1_PRESSED] == true) || (EventHandler::events[GameEvents::KP2_PRESSED] == true) || (EventHandler::events[GameEvents::KP3_PRESSED] == true)) { 
			SpawnProjectile(ProjGameObject, PlayerGameObject);
		} break; 
	}
	}
}

void MovePlayer(GameObject* PlayerGameObject, bool left){
	if (left) PlayerGameObject->transform.x = max(0, PlayerGameObject->transform.x - ShipSpeed);
	else PlayerGameObject->transform.x = min(Graphics::WINDOW_WIDTH - PlayerGameObject->transform.w, PlayerGameObject->transform.x + ShipSpeed);
}

void HandleInput(GameObject* PlayerGameObject){
	switch (PlayerGameObject->type) {
	case GameObject::ObjectTypes::PLAYER_1: {
		if (EventHandler::events[GameEvents::A_PRESSED] == true) MovePlayer(PlayerGameObject, true);
		if (EventHandler::events[GameEvents::D_PRESSED] == true) MovePlayer(PlayerGameObject, false);
		break;
	}
	case GameObject::ObjectTypes::PLAYER_2: {
		if (EventHandler::events[GameEvents::LEFT_PRESSED] == true) MovePlayer(PlayerGameObject, true);
		if (EventHandler::events[GameEvents::RIGHT_PRESSED] == true) MovePlayer(PlayerGameObject, false);
		break;
	}
	}
}

bool ObjectsCollide(GameObject* GameObjectA, GameObject* GameObjectB) {
	SDL_Rect& rectA = GameObjectA->transform; SDL_Rect& rectB = GameObjectB->transform;
	if (rectA.y + rectA.h <= rectB.y || rectA.y >= rectB.y + rectB.h || rectA.x + rectA.w <= rectB.x || rectA.x >= rectB.x + rectB.w) return false;
	return true;
}

void HandleInvader(GameObject* InvaderGameObject, GameObject* Player1Proj, GameObject* Player2Proj) {
	if ((Player1Proj->visible == true) && ObjectsCollide(InvaderGameObject, Player1Proj)){
		Player1Proj->visible = false; InvaderGameObject->visible = false; Score += 10;
	}
	if ((Player2Proj->visible == true) && ObjectsCollide(InvaderGameObject, Player2Proj)) {
		Player2Proj->visible = false; InvaderGameObject->visible = false; Score += 10;
	}
}

void ToggleNumPlayers(GameObject* FirstGameObject){
	NumPlayers = (NumPlayers == 1) ? 2 : 1;
	GameObject* CurGameObject = FirstGameObject;
	//
	while (true) {
		if (CurGameObject == nullptr) break;
		GameObject* NextGameObject = CurGameObject->GetNext();
		switch (CurGameObject->type) {
		case GameObject::ObjectTypes::PLAYER_2:
			if (NumPlayers >= 2) {
				CurGameObject->visible = true;
			}
			else {
				CurGameObject->visible = false;
			}
			break;
		}
		//
		if (NextGameObject == nullptr) break; // we reached the last object
		else CurGameObject = NextGameObject;
	}
	//
}

void GameplayManager::Update(){
	//
	GameObject* CurGameObject = FirstGameObject;
	GameObject* Player1 = nullptr;
	GameObject* Player2 = nullptr;
	GameObject* Player1Proj = nullptr;
	GameObject* Player2Proj = nullptr;
	//
	int MinX = 9999; int MaxX = 0; int MaxX_W = 0; int LivingInvaders = 0; bool ReachedEdge = false; int AdvanceAmount = InvaderSpeed;
	// Get the min max positions of all invaders.

	SpriteInt = (SpriteInt < SpriteIntMax) ? SpriteInt + 1 : 0;

	// ------------ ||
	while (true) {
		if (CurGameObject == nullptr) break;
		GameObject* NextGameObject = CurGameObject->GetNext();
		switch (CurGameObject->type) {
		case GameObject::ObjectTypes::PLAYER_1:
			Player1 = CurGameObject;
			HandleInput(Player1);
			break;
		case GameObject::ObjectTypes::PLAYER_2:
			Player2 = CurGameObject;
			HandleInput(Player2);
			break;
		case GameObject::ObjectTypes::INVADER_1A: // Intentional fallthrough
		case GameObject::ObjectTypes::INVADER_1B: // Intentional fallthrough
		case GameObject::ObjectTypes::INVADER_2A: // Intentional fallthrough
		case GameObject::ObjectTypes::INVADER_2B: // Intentional fallthrough
		case GameObject::ObjectTypes::INVADER_3A: // Intentional fallthroughD
		case GameObject::ObjectTypes::INVADER_3B: // Call a function for all invader cases
			if (SpriteInt == SpriteIntMax) {
				if (CurGameObject->type % 2 == GameObject::ObjectTypes::INVADER_1A % 2) {
					CurGameObject->type++; // Alternate sprite (animation)
				}
				else {
					CurGameObject->type--;  // Alternate sprite (animation)
				}
			}
			if (CurGameObject->visible == true) {
				HandleInvader(CurGameObject, Player1Proj, Player2Proj);
				MinX = min(MinX, CurGameObject->transform.x);
				int TempMaxX = max(MaxX, CurGameObject->transform.x + CurGameObject->transform.w);
				if (TempMaxX > MaxX){
					MaxX = TempMaxX;
					MaxX_W = CurGameObject->transform.w;
				}
				LivingInvaders++;
			}
			break;
		case GameObject::ObjectTypes::UFO:
			break;
		case GameObject::ObjectTypes::PLAYER_1_PROJECTILE:
			Player1Proj = CurGameObject;
			ManageProjectile(Player1Proj, Player1);
			break;
		case GameObject::ObjectTypes::PLAYER_2_PROJECTILE:
			Player2Proj = CurGameObject;
			ManageProjectile(Player2Proj, Player2);
			break;
		}
		//
		if (NextGameObject == nullptr) break; // we reached the last object
		else CurGameObject = NextGameObject;
	}
	//
	if (LivingInvaders > 0){
		//
		for (int i = 1; i < LowSurvivors; i++){ // Move faster the fewer survivors < LowSurvivors there are.
			if (LivingInvaders <= i) AdvanceAmount += AccelLowSurvivors;
		}
		//
		if (AdvancingLeft == true) {
			int TempMinX = max(0, MinX - AdvanceAmount);
			if (TempMinX == 0) { 
				ReachedEdge = true;
				AdvanceAmount = dif(MinX, TempMinX);
			}
			AdvanceAmount = -AdvanceAmount;
		}
		else {
			int TempMaxX = min(Graphics::WINDOW_WIDTH, MaxX + AdvanceAmount);
			if (TempMaxX == Graphics::WINDOW_WIDTH) {  
				ReachedEdge = true;
				AdvanceAmount = dif(MaxX, TempMaxX);
			}
		}
		//
		CurGameObject = FirstGameObject;
		while (true) {			
			if (CurGameObject == nullptr) break;
			GameObject* NextGameObject = CurGameObject->GetNext();
			switch (CurGameObject->type) {
			case GameObject::ObjectTypes::INVADER_1A: // Intentional fallthrough
			case GameObject::ObjectTypes::INVADER_1B: // Intentional fallthrough
			case GameObject::ObjectTypes::INVADER_2A: // Intentional fallthrough
			case GameObject::ObjectTypes::INVADER_2B: // Intentional fallthrough
			case GameObject::ObjectTypes::INVADER_3A: // Intentional fallthrough
			case GameObject::ObjectTypes::INVADER_3B: // Call a function for all invader cases
				if (CurGameObject->visible == true) {
					CurGameObject->transform.x += AdvanceAmount;
					if (ReachedEdge == true) CurGameObject->transform.y += DistPerAdvance;
				}
				break;
			}
			//
			if (NextGameObject == nullptr) break; // we reached the last object
			else CurGameObject = NextGameObject;
		}
		//
		if (ReachedEdge == true) {
			InvaderSpeed += AccelPerAdvance;
			AdvancingLeft = !AdvancingLeft;
		}
		//
	}
	else { // You beat the current level
	
	}
	// ------------ ||

	LastTick = tick();
}

void GameplayManager::Render(){
	// render next frame / all game objects	
	if (AtMenu == false) DrawScore();
	else {
		DrawMenu();
	}
	GameObjectDrawer::DrawGameObjects(FirstGameObject);
}

bool MouseInRect(int x, int y, int w, int h) {
	return EventHandler::MouseX >= x && EventHandler::MouseX <= x + w && EventHandler::MouseY >= y && EventHandler::MouseY <= y + h;
}

void GameplayManager::DrawMenu() {
	string TitleString = "Void Invaders";
	string PlayerString = "Players: " + to_string(NumPlayers) + "P";
	string PlayerNameString = NameEntry + PlayerName + NameEdit;

	Graphics::DrawText(TitleString.c_str(), Graphics::WINDOW_WIDTH / 2 - 15 * TitleString.size(), 30, TitleString.size() * 30, 70, ArialFont);
	Graphics::DrawText(PlayerString.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * PlayerString.size(), 145, PlayerString.size() * 18, 44, ArialFont);
	if (EventHandler::MClicked(1) == true && MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * PlayerString.size(), 145, PlayerString.size() * 18, 44) == true) {
		ToggleNumPlayers(FirstGameObject);
	}

	Graphics::DrawText(PlayerNameString.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * PlayerNameString.size(), 220, PlayerNameString.size() * 18, 44, ArialFont);
	if (EventHandler::MClicked(1) == true){ 
		if (MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * PlayerNameString.size(), 220, PlayerNameString.size() * 18, 44) == true) {
			NameEntry = "-> PlayerName: "; NameEdit = "_";
		}
		else if (NameEdit == "_") {
			NameEntry = "PlayerName: "; NameEdit = "";
		}
	}
	if (NameEdit == "_") {
		if (EventHandler::KeyHit(GameEvents::BACKSPACE_PRESSED)) {
			if (!PlayerName.empty()) PlayerName.pop_back();
		}

		if (EventHandler::KeyHit(GameEvents::ENTER_PRESSED)) {
			NameEntry = "PlayerName: "; NameEdit = "";
		}
	}
}

void GameplayManager::DrawScore(){
	// draw the score
	string ScoreString = "Score: " + to_string(Score);
	Graphics::DrawText(ScoreString.c_str(), 150, 20, ScoreString.size() * 20, 50, ArialFont);
}