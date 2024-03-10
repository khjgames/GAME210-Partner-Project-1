#include "GameObjectDrawer.h"

void GameObjectDrawer::DrawGameObjects(GameObject* FirstGameObject){
	GameObject* CurGameObject = FirstGameObject;
	while (true){
		if (CurGameObject == nullptr) break;
		GameObject* NextGameObject = CurGameObject->GetNext();
		switch (CurGameObject->type) {
			case GameObject::ObjectTypes::PLAYER_1:
				break;
			case GameObject::ObjectTypes::PLAYER_2:
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
				break;
			case GameObject::ObjectTypes::PLAYER_2_PROJECTILE:
				break;
		}
		//
		SDL_Texture* sprite = GameSprites::Sprites[CurGameObject->type];
		if (sprite != NULL && CurGameObject->loaded == false){
			CurGameObject->transform.w = GameSprites::Dimensions[CurGameObject->type].x;
			CurGameObject->transform.h = GameSprites::Dimensions[CurGameObject->type].y;
			CurGameObject->loaded = true;
		}
		//
		if (CurGameObject->visible == true) SDL_RenderCopy(Graphics::renderer, sprite, NULL, &CurGameObject->transform);
		//
		if (NextGameObject == nullptr) break; // we reached the last object
		else CurGameObject = NextGameObject;
	}
}