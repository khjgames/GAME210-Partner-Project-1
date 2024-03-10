#include "GameObjectDrawer.h"

void GameObjectDrawer::DrawGameObjects(GameObject* FirstGameObject){
	GameObject* CurGameObject = FirstGameObject;
	while (true){
		if (CurGameObject == nullptr) break;
		GameObject* NextGameObject = CurGameObject->GetNext();
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