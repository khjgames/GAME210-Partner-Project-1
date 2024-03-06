#include "GameObjectDrawer.h"

void GameObjectDrawer::DrawGameObjects(GameObject* FirstGameObject){
	GameObject* CurGameObject = FirstGameObject;
	while (true){
		if (CurGameObject == nullptr) break;
		GameObject* NextGameObject = CurGameObject->GetNext();
		switch (CurGameObject->type) {
			case GameObject::ObjectTypes::PLAYER:

				//SDL_RenderCopy(renderer, sprite1, NULL, position);
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
		if (NextGameObject == nullptr) break; // we reached the last object
		else CurGameObject = NextGameObject;
	}
}