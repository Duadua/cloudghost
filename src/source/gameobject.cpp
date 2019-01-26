#include "gameobject.h"
#include "scenecomponent.h"

GameObject::GameObject(SceneComponent* root) {
	root_component = root;
}
GameObject::~GameObject() {
	delete root_component;
}

void GameObject::set_root(SceneComponent* root) {
	root_component = root;
}
SceneComponent* GameObject::get_root() {
	return root_component;
}

void GameObject::draw(Shader& shader) {
	if (root_component == nullptr) return;
	root_component->draw(shader);
}

void GameObject::bind_input() {

}
