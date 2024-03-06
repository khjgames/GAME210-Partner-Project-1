#include "GameObject.h"

GameObject* GameObject::GetNext() {
	return next;
}

void GameObject::SetNext(GameObject* nx) {
	next = nx;
}

GameObject::GameObject(){
	next = nullptr;	
	x = 0; y = 0; type = 0; visible = false;
	id = num_created; num_created++;
} // Default Constructor

GameObject::GameObject(short ax, short by, short ctype, bool dvisible) : x(ax), y(by), type(ctype), visible(dvisible){
	next = nullptr; 
	id = num_created; num_created++;
} // Default Constructor

GameObject::~GameObject(){
	// delete data here
} // Default Destructor