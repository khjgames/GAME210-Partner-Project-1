#include "GameplayManager.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../Graphics.h"
// creating a basic gameobject
#include "../Objects/ColourBlock.h"
#include "../GameObjectDrawer.h"
#include "../EventHandler.h"
#include "../GameSpaces.h"
#include "../Leaderboard.h"

using namespace GameTime;
using namespace GameVars;
using namespace LeaderboardConst;

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
	AddGameObject(Graphics::WINDOW_WIDTH / 2 + 119, Graphics::WINDOW_HEIGHT - 90, GameObject::ObjectTypes::PLAYER_2, NumPlayers == 2);
	AddGameObject(Graphics::WINDOW_WIDTH / 2 - 19, Graphics::WINDOW_HEIGHT - 150, GameObject::ObjectTypes::PLAYER_1_PROJECTILE, false);
	AddGameObject(Graphics::WINDOW_WIDTH / 2 + 119, Graphics::WINDOW_HEIGHT - 150, GameObject::ObjectTypes::PLAYER_2_PROJECTILE, false);
	AddGameObject(Graphics::WINDOW_WIDTH, Graphics::WINDOW_HEIGHT / 4 - 104, GameObject::ObjectTypes::UFO, false);

}

void GameplayManager::LoadLevel(short X_GAP, short Y_GAP, short PER_ROW, short NUM_ROWS){
	for (int a = 0; a < (PER_ROW); a++) {
		for (int b = 0; b < NUM_ROWS; b++) {
			AddGameObject(Graphics::WINDOW_WIDTH / 2 - 16 - (PER_ROW / 2 - a) * X_GAP, Graphics::WINDOW_HEIGHT / 4 - (((2+max(0,NUM_ROWS-6)) - b) * Y_GAP), 2 + (b / 2 % 3 * 2), true);
		}
	}
}

void GameplayManager::Init(){
	srand(time(NULL));
	LastTick = tick(); LastSpriteAnim = LastTick;
	GameSprites::LoadSprites();
	DeleteGameObjects();
	InitGameObjects();
	LoadGame();
	InitGameVars();		// Reset and init a bunch of game vars
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
		ProjGameObject->transform.y = max(35, ProjGameObject->transform.y - ProjectileSpeed);
		if (ProjGameObject->transform.y == 35) ProjGameObject->visible = false;
	}
}

void ManageProjectile(GameObject* ProjGameObject, GameObject* PlayerGameObject) {
	if (ProjGameObject->transform.w != ProjSizeX) {
		ProjGameObject->transform.w = ProjSizeX;
		ProjGameObject->transform.h = ProjSizeY;
	}
	MoveProjectile(ProjGameObject);
	if (GameOver == 0) { // Can't shoot on GameOver
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

void HandleUFO(GameObject* UFOGameObject, GameObject* Player1Proj, GameObject* Player2Proj) {
	short UFOSpeed = 2 + InvaderSpeed;
	if (OwnedUpgrades[3] >= 1) UFOSpeed -= 1; //UFO Study Tier 1 upgrade
	if (OwnedUpgrades[3] >= 2) UFOSpeed -= 1; //UFO Study Tier 2 upgrade
	UFOGameObject->transform.x -= UFOSpeed;
	if ((Player1Proj->visible == true) && ObjectsCollide(UFOGameObject, Player1Proj)) {
		Player1Proj->visible = false; UFOGameObject->visible = false; 
		if (AtMenu == false && OwnedUpgrades[3] > 3) { //UFO Study Tier 3 upgrade
			Score += 50; VoidBits += floor(Level* 1.68);
		}
		if (AtMenu == false) Score += 100; 
		VoidBits += 2 + floor(Level * 1.68);
	}
	if ((Player2Proj->visible == true) && ObjectsCollide(UFOGameObject, Player2Proj)) {
		Player2Proj->visible = false; UFOGameObject->visible = false; 
		if (AtMenu == false && OwnedUpgrades[3] > 3) { //UFO Study Tier 3 upgrade
			Score += 50; VoidBits += floor(Level * 1.68);
		}
		if (AtMenu == false) Score += 100;
		VoidBits += 2 + floor(Level * 1.68);
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
	if (GamePaused == false) {
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
					if (TempMaxX > MaxX) {
						MaxX = TempMaxX;
						MaxX_W = CurGameObject->transform.w;
					}
					LivingInvaders++;
					if (GameOver == 0) {
						if (CurGameObject->transform.y + CurGameObject->transform.h >= FirstGameObject->transform.y) GameOver = 1;
					}
				}
				break;
			case GameObject::ObjectTypes::UFO:
				if (CurGameObject->visible == true) {
					HandleUFO(CurGameObject, Player1Proj, Player2Proj);
				}
				if (AtMenu == true) {
					MenuTime += dif(tick(), LastTick);
					if (MenuTime > 45 * 1000) { MenuTime = 0; // 45 seconds
						CurGameObject->transform.x = Graphics::WINDOW_WIDTH;
						CurGameObject->visible = true; // Free void bits!
					} // James did you notice the menu ufo easter egg? 
				}
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
		if (LivingInvaders > 0) {
			//
			for (int i = 1; i < min(LowSurvivors, INVADER_GEAR_SPEEDS); i++) { // Move faster the fewer survivors < LowSurvivors there are.
				if (LivingInvaders <= (i + max(0, LowSurvivors - INVADER_GEAR_SPEEDS))) AdvanceAmount += AccelLowSurvivors;
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
				case GameObject::ObjectTypes::UFO:
					if (SpawnedUFOs < Level && LivingInvaders >= 27 && LivingInvaders <= 36) {
						SpawnedUFOs++;
						CurGameObject->transform.x = Graphics::WINDOW_WIDTH;
						CurGameObject->visible = true;
					}
					if (CurGameObject->visible == false && OwnedUpgrades[3] >= 4 && ExtraSpawnedUFOs < Level && LivingInvaders >= 5 && LivingInvaders <= 18) { //UFO Study Tier 4 upgrade
						ExtraSpawnedUFOs++;
						CurGameObject->transform.x = Graphics::WINDOW_WIDTH;
						CurGameObject->visible = true;
					}
				}
				//
				if (NextGameObject == nullptr) break; // we reached the last object
				else CurGameObject = NextGameObject;
			}
			//
			if (ReachedEdge == true) {
				AdvanceCd += AccelPerAdvance;
				if (AdvanceCd >= 1) { AdvanceCd -= 1; InvaderSpeed++; }
				AdvancingLeft = !AdvancingLeft;
			}
			//
		}
		else {
			if (AtMenu == false) { // You beat the current level
				Level++; VoidBits++; VoidBits += floor(Level * 1.68);
				short NumInvade = 9 + min((short)floor(Level/15),(short)3);
				short NumWaves = 6 + floor(Level / 4.5);
				if (Level % 2 == 0) NumInvade = 10; 
				if (Level % 5 == 1) {
					if (Level > 6) InvaderSpeed = min((short) (InvaderSpeed+1), (short) 4);
					LowSurvivors++;
				}
				if (Level % 3 == 0) {
					if (Level > 6) NumInvade = 11;
					LowSurvivors++; 
				}
				if (Level % 4 == 0) {
					LowSurvivors++; AccelLowSurvivors++; 
					AccelPerAdvance = min(AccelPerAdvance + 0.06f, 0.667f); 
				}
				LoadLevel(52, 52, NumInvade, NumWaves); // Load in enemy invaders also load in time, and score, when you resume a level resume from the start of the level you left off on.
			}
		}
		// ------------ ||

		if (GameOver == 1) {
			if (Player1Proj != nullptr && Player2Proj != nullptr && Player1Proj->visible == false && Player2Proj->visible == false) {
				GameOver = 2;
			}
		}
		//
		if (GamePaused == false && AtMenu == false && AtShop == false && ViewingLeaderboard == false && GameOver == 0) TimePlayed += dif(tick(), LastTick);
	}
	LastTick = tick();
}

void GameplayManager::Render(){
	// render next frame / all game objects	
	//
	string VoidBitsString = "Void Bits: " + to_string(VoidBits);
	Graphics::DrawText(VoidBitsString.c_str(), Graphics::WINDOW_WIDTH - VoidBitsString.size() * 25, 20, VoidBitsString.size() * 15, 40, ArialFont);
	//
	ostringstream format_timeplayed_str;
	format_timeplayed_str << "Time Played: " << fixed << setprecision(2) << ((float)TimePlayed) / 1000.0f / 60.0f;
	std::string TimePlayedString = format_timeplayed_str.str();
	Graphics::DrawText(TimePlayedString.c_str(), Graphics::WINDOW_WIDTH - TimePlayedString.size() * 18, 60, TimePlayedString.size() * 10, 25, ArialFont);
	//
	if (AtMenu == false) DrawScore();
	else {
		if (ViewingLeaderboard) {
			DrawLeaderboard();
		}
		else {
			DrawMenu();
		}
	}
	GameObjectDrawer::DrawGameObjects(FirstGameObject);
}

bool MouseInRect(int x, int y, int w, int h) {
	return EventHandler::MouseX >= x && EventHandler::MouseX <= x + w && EventHandler::MouseY >= y && EventHandler::MouseY <= y + h;
}

void GameplayManager::DrawMenu() {
	string TitleString = "Void Invaders";
	string PlayerNameString = NameEntry + PlayerName + NameEdit;

	Graphics::DrawText(TitleString.c_str(), Graphics::WINDOW_WIDTH / 2 - 15 * TitleString.size(), 30, TitleString.size() * 30, 70, ArialFont);
	//
	if (AtShop == false) {
		string p1Controls = "Player 1: W & D to Move, Space to Shoot";
		string p2Controls = "Player 2: < & > to Move, KP0 to Shoot";
		Graphics::DrawText(p1Controls.c_str(), 50, Graphics::WINDOW_HEIGHT - 170, p1Controls.size() * 10, 30, ArialFont);
		Graphics::DrawText(p2Controls.c_str(), Graphics::WINDOW_WIDTH - 50 - p1Controls.size() * 10, Graphics::WINDOW_HEIGHT - 170, p1Controls.size() * 10, 30, ArialFont);

		if (MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * CoopBtnString.size(), 145, CoopBtnString.size() * 18, 44) == true) {
			CoopBtnString = "> Players: " + to_string(NumPlayers) + "P <";
			if (EventHandler::MClicked(1) == true) ToggleNumPlayers(FirstGameObject);
		}
		else {
			CoopBtnString = "Players: " + to_string(NumPlayers) + "P";
		}
		Graphics::DrawText(CoopBtnString.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * CoopBtnString.size(), 145, CoopBtnString.size() * 18, 44, ArialFont);

		Graphics::DrawText(PlayerNameString.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * PlayerNameString.size(), 220, PlayerNameString.size() * 18, 44, ArialFont);
		if (EventHandler::MClicked(1) == true) {
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

			for (int i = 0; i < TYPESET_SIZE; i++) {
				if (EventHandler::KeyHit(i)) {
					if (PlayerName.size() <= MAX_NAME_SIZE - 1) {
						PlayerName = PlayerName + Typeset[i];
					}
				}
			}

			if (EventHandler::KeyHit(GameEvents::ENTER_PRESSED)) {
				NameEntry = "PlayerName: "; NameEdit = "";
			}
		}

		Graphics::DrawText(StartBtnString.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * StartBtnString.size(), 300, StartBtnString.size() * 18, 44, ArialFont);
		if (MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * StartBtnString.size(), 300, StartBtnString.size() * 18, 44) == true) {
			StartBtnString = "> Start Game <";
			if (EventHandler::MClicked(1) == true && AtMenu == true) {
				AtMenu = false;
				LoadLevel(52, 52, 8, 6); // Load in enemy invaders also load in time, and score, when you resume a level resume from the start of the level you left off on.
			}
		}
		else StartBtnString = "Start Game";

		Graphics::DrawText(ShopBtnString.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * ShopBtnString.size(), 380, ShopBtnString.size() * 18, 44, ArialFont);
		if (MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * ShopBtnString.size(), 380, ShopBtnString.size() * 18, 44) == true) {
			ShopBtnString = "> Shop <";
			if (EventHandler::MClicked(1) == true && AtMenu == true && AtShop == false) {
				AtShop = true;
			}
		}
		else ShopBtnString = "Shop";


		string lbtext = "Leaderboard";
		if (MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * lbtext.size(), 460, lbtext.size() * 18, 44) == true) {
			lbtext = "> Leaderboard <";
			if (EventHandler::MClicked(1) == true) {
				ViewingLeaderboard = true;
			}
		}
		Graphics::DrawText(lbtext.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * lbtext.size(), 460, lbtext.size() * 18, 44, ArialFont);

		string quit = "Quit";
		if (MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * quit.size(), 530, quit.size() * 18, 44) == true) {
			quit = "> Quit <";
			if (EventHandler::MClicked(1) == true) {
				run = false;
			}
		}
		Graphics::DrawText(quit.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * quit.size(), 540, quit.size() * 18, 44, ArialFont);
	}
	else {

		string ShopHint = "Earn VoidBits from UFOs & Levels! Higher the better!";
		Graphics::DrawText(ShopHint.c_str(), Graphics::WINDOW_WIDTH/2 - ShopHint.size() * 5.325, 110, ShopHint.size() * 10, 25, ArialFont);

		Graphics::DrawText(ExitShopBtnString.c_str(), Graphics::WINDOW_WIDTH / 2 - 9 * ExitShopBtnString.size(), 500, ExitShopBtnString.size() * 18, 44, ArialFont);
		if (MouseInRect(Graphics::WINDOW_WIDTH / 2 - 9 * ExitShopBtnString.size(), 500, ExitShopBtnString.size() * 18, 44) == true) {
			ExitShopBtnString = "> Exit Shop <";
			if (EventHandler::MClicked(1) == true && AtMenu == true && AtShop == true) {
				AtShop = false;
			}
		}
		else ExitShopBtnString = "Exit Shop";

		for (short i = 0; i < SHOP_UPGRADES; i++){
			short u = OwnedUpgrades[i];
			short UpgCost = floor(((UpgradeCosts[u] + (10 * i)) * (1 + ((float)i / 10.0f))) * 0.7);
			string ShopUpgradeString = UpgradeNames[i] + " " + to_string(u) + " [MAXXED]";
			//
			if (u < MaxUpgrades[i]) { // If any more can be purchased
				ShopUpgradeString = UpgradeNames[i] + " " + to_string(u) + " [" + to_string(UpgCost) + "]";
				Graphics::DrawText(ShopBuyStrings[i].c_str(), Graphics::WINDOW_WIDTH / 2 + 75 - ShopBuyStrings[i].size() * 9, 160 + 80 * i, ShopBuyStrings[i].size() * 18, 44, ArialFont);
				//
				if (MouseInRect(Graphics::WINDOW_WIDTH / 2 + 75 - ShopBuyStrings[i].size() * 9, 160 + 80 * i, ShopBuyStrings[i].size() * 18, 44) == true) {
					ShopBuyStrings[i] = "> Buy <";
					if (EventHandler::MClicked(1) == true && AtMenu == true && AtShop == true && VoidBits >= UpgCost) {
						VoidBits -= UpgCost;
						OwnedUpgrades[i]++;
						ApplyUpgrades();
						SaveGame();
					}
				}
				else ShopBuyStrings[i] = "Buy";
			}
			//
			Graphics::DrawText(ShopUpgradeString.c_str(), Graphics::WINDOW_WIDTH / 3 + 50 - 9 * ShopUpgradeString.size(), 160 + 80 * i, ShopUpgradeString.size() * 18, 44, ArialFont);
			//
			if (u > 0) { // If any can be refunded
				Graphics::DrawText(ShopRefundStrings[i].c_str(), Graphics::WINDOW_WIDTH / 2 + 240 - ShopRefundStrings[i].size() * 9, 160 + 80 * i, ShopRefundStrings[i].size() * 18, 44, ArialFont);
				//
				if (MouseInRect(Graphics::WINDOW_WIDTH / 2 + 240 - ShopRefundStrings[i].size() * 9, 160 + 80 * i, ShopRefundStrings[i].size() * 18, 44) == true) {
					ShopRefundStrings[i] = "> Refund <";
					if (EventHandler::MClicked(1) == true && AtMenu == true && AtShop == true) {
						VoidBits += floor(((UpgradeCosts[(u - 1)] + (10 * i)) * (1 + ((float)i / 10.0f))) * 0.7);
						OwnedUpgrades[i]--;
						ApplyUpgrades();
						SaveGame();
					}
				}
				else ShopRefundStrings[i] = "Refund";
			}
			//
		}

	}
	//
}

void GameplayManager::DrawScore(){
	// draw the score
	string ScoreString = "Score: " + to_string(Score);
	Graphics::DrawText(ScoreString.c_str(), 50, 20, ScoreString.size() * 15, 40, ArialFont);

	string LevelString = "Level: " + to_string(Level);
	Graphics::DrawText(LevelString.c_str(), 300, 20, LevelString.size() * 15, 40, ArialFont);

	string HighScoreString = "High Score: " + to_string(lb.GetEntry(0, NumPlayers - 1).score);
	Graphics::DrawText(HighScoreString.c_str(), 50, 60, HighScoreString.size() * 10, 25, ArialFont);
	//
	
	if (EventHandler::KeyHit(GameEvents::ESCAPE_PRESSED)) {
		if (GamePaused == false && AtMenu == false && AtShop == false && ViewingLeaderboard == false && GameOver == 0) GamePaused = true;
		else GamePaused = false;
	}

	if (GamePaused == false) {

		string PauseString = "Pause (ESC)";
		Graphics::DrawText(PauseString.c_str(), Graphics::WINDOW_WIDTH / 2 - PauseString.size() * 5, 50, PauseString.size() * 10, 25, ArialFont);
	}
	else {
		string PauseString = "GAME PAUSED (ESC)";
		Graphics::DrawText(PauseString.c_str(), Graphics::WINDOW_WIDTH / 2 - PauseString.size() * 10, 50, PauseString.size() * 20, 50, ArialFont);
	}
	//
	if (GameOver >= 1) {
		string GameOverString = "GAME OVER.";
		Graphics::DrawText(GameOverString.c_str(), Graphics::WINDOW_WIDTH / 2 - 18 * GameOverString.size(), 110, GameOverString.size() * 36, 80, ArialFont);
		if (GameOver == 2) { 
			GameOver = 3;
			ScorePlaced = lb.AddEntry(PlayerName.c_str(), Score, NumPlayers - 1);
			lb.Save();
			SaveGame();
		}
		if (GameOver >= 3){
			string PlaceGrammar = "th";
			if (ScorePlaced > 0) {
				switch (ScorePlaced){
				case 1: PlaceGrammar = "st"; break;
				case 2: PlaceGrammar = "nd"; break;
				case 3: PlaceGrammar = "rd"; break;
				}
				string ScorePlaceString = "New " + to_string(ScorePlaced) + PlaceGrammar + " place score!";
				Graphics::DrawText(ScorePlaceString.c_str(), Graphics::WINDOW_WIDTH / 2 - 10 * ScorePlaceString.size(), 200, ScorePlaceString.size() * 20, 50, ArialFont);
			}

			string buttonText = "BACK TO MENU";
			if (MouseInRect(
				Graphics::WINDOW_WIDTH / 2 - buttonText.size() * 20 / 2, Graphics::WINDOW_HEIGHT - 175
				- 35, buttonText.size() * 20, 35) == true) {
				buttonText = "> BACK TO MENU <";
				if (EventHandler::MClicked(1) == true) {
					AtMenu = true;
					DeleteGameObjects();
					InitGameObjects();
					InitGameVars();		// Reset and init a bunch of game vars
				}
			}
			Graphics::DrawText(buttonText.c_str(),
				Graphics::WINDOW_WIDTH / 2 - buttonText.size() * 20 / 2, Graphics::WINDOW_HEIGHT - 175
				- 35, buttonText.size() * 20, 35, ArialFont);
		}
	}
}

void GameplayManager::DrawLeaderboard() {
	string TitleString = "Void Invaders";
	Graphics::DrawText(TitleString.c_str(), Graphics::WINDOW_WIDTH / 2 - 15 * TitleString.size(), 30, TitleString.size() * 30, 70, ArialFont);

	const short CHAR_WIDTH = 20;
	const short CHAR_HEIGHT = 35;

	// get the size of the high score entry to get the leaderboard centered
	int largestScoreSize = (to_string(lb.GetEntry(0).score).size() > to_string(lb.GetEntry(0, TWO_PLAYER).score).size()) ?
		to_string(lb.GetEntry(0).score).size() :
		to_string(lb.GetEntry(0, TWO_PLAYER).score).size();
	int x = Graphics::WINDOW_WIDTH / 4 - (largestScoreSize * CHAR_WIDTH / 2 + MAX_NAME_SIZE * CHAR_WIDTH / 2 + CHAR_WIDTH / 2) ;
	int y = 170;

	// draw each entry in the 1 player leaderboard with title
	string onePlayerTitle = "1 Player";
	Graphics::DrawText(onePlayerTitle.c_str(), Graphics::WINDOW_WIDTH * 0.25 - onePlayerTitle.size() * 15 / 2, y - 50, onePlayerTitle.size() * 15, CHAR_HEIGHT, ArialFont);
	for (int i = 0; i < MAX_ENTRIES; i++) {
		LBEntry entry = lb.GetEntry(i);
		string name = entry.name;
		Graphics::DrawText(name.c_str(),
			x, y + i * CHAR_HEIGHT, name.size() * CHAR_WIDTH, CHAR_HEIGHT, ArialFont);
		string scoreString = to_string(entry.score);
		Graphics::DrawText(scoreString.c_str(),
			x + (MAX_NAME_SIZE + 1) * CHAR_WIDTH, y + i * CHAR_HEIGHT,
			scoreString.size() * CHAR_WIDTH, CHAR_HEIGHT, ArialFont);
	}

	// draw each entry in the 2 player leaderboard with title
	string twoPlayerTitle = "2 Player";
	Graphics::DrawText(twoPlayerTitle.c_str(), Graphics::WINDOW_WIDTH * 0.75 - twoPlayerTitle.size() * 15 / 2, y - 50, twoPlayerTitle.size() * 15, CHAR_HEIGHT, ArialFont);
	x += Graphics::WINDOW_WIDTH / 2;
	for (int i = 0; i < MAX_ENTRIES; i++) {
		LBEntry entry = lb.GetEntry(i, TWO_PLAYER);
		string name = entry.name;
		Graphics::DrawText(name.c_str(),
			x, y + i * CHAR_HEIGHT, name.size() * CHAR_WIDTH, CHAR_HEIGHT, ArialFont);
		string scoreString = to_string(entry.score);
		Graphics::DrawText(scoreString.c_str(),
			x + (MAX_NAME_SIZE + 1) * CHAR_WIDTH, y + i * CHAR_HEIGHT,
			scoreString.size() * CHAR_WIDTH, CHAR_HEIGHT, ArialFont);
	}

	string buttonText = "BACK TO MENU";
	if (MouseInRect(
		Graphics::WINDOW_WIDTH / 2 - buttonText.size() * CHAR_WIDTH / 2, Graphics::WINDOW_HEIGHT - 125
		- CHAR_HEIGHT, buttonText.size() * CHAR_WIDTH, CHAR_HEIGHT) == true) 
	{
		buttonText = "> BACK TO MENU <";
		if (EventHandler::MClicked(1) == true) {
			ViewingLeaderboard = false;
		}
	}
	Graphics::DrawText(buttonText.c_str(),
		Graphics::WINDOW_WIDTH / 2 - buttonText.size() * CHAR_WIDTH / 2, Graphics::WINDOW_HEIGHT - 125
		- CHAR_HEIGHT, buttonText.size() * CHAR_WIDTH, CHAR_HEIGHT, ArialFont);
}