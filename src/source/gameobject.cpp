#include "gameobject.h"
#include "scenecomponent.h"

IMPLEMENT_CLASS(GameObject)

GameObject::GameObject(SPTR_SceneComponent root) { root_component = root; }
GameObject::~GameObject() {} 

void GameObject::set_root(SPTR_SceneComponent root) { root_component = root; }
SPTR_SceneComponent GameObject::get_root() { return root_component; }

void GameObject::draw(SPTR_Shader shader) {
	if (root_component == nullptr) return;
	root_component->draw(shader);
}

void GameObject::bind_input() {

}
