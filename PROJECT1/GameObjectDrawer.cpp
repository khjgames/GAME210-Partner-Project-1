#include "GameObjectDrawer.h"
void GameObjectDrawer::DrawGameObjects(GameObject* FirstGameObject){
	GameObject* CurGameObject = FirstGameObject;
	while (true){
		switch (CurGameObject->type) {
			case GameObject::ObjectTypes::PLAYER:
				break;
			case GameObject::ObjectTypes::INVADER_1:
				break;
			case GameObject::ObjectTypes::INVADER_2:
				break;
			case GameObject::ObjectTypes::INVADER_3:
				break;
			case GameObject::ObjectTypes::UFO:
				break;
			case GameObject::ObjectTypes::PLAYER_PROJECTILE:
				break;
		}
		if (CurGameObject->GetNext() == nullptr) break; // we reached the last object
	}
}