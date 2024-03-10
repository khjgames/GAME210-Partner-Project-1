#include "GameObject.h"

static unsigned long long num_created = 0;

GameObject* GameObject::GetNext() {
	return next;
}

void GameObject::SetNext(GameObject* nx) {
	next = nx;
}

GameObject::GameObject(){
	next = nullptr;	
	transform.w = 0; transform.h = 0;
	transform.x = 0; transform.y = 0;
	type = 0; visible = false;
	id = num_created; num_created++;
} // Default Constructor

GameObject::GameObject(short ax, short by, short ctype, bool dvisible){
	next = nullptr;
	transform.w = 0; transform.h = 0;
	transform.x = ax; transform.y = by;
	type = ctype; visible = dvisible;
	id = num_created; num_created++;
} // Default Constructor

GameObject::~GameObject(){

} // Default Destructor