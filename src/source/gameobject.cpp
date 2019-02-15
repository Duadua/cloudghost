#include "gameobject.h"
#include "assetmanager.h"
#include "scenecomponent.h"

IMPLEMENT_CLASS(GameObject)

GameObject::GameObject(SPTR_SceneComponent root) { root_component = root; }
GameObject::~GameObject() {} 

void GameObject::set_root(SPTR_SceneComponent root) { root_component = root; }
SPTR_SceneComponent GameObject::get_root() { return root_component; }

void GameObject::bind_input() {}
void GameObject::begin_play() {}
void GameObject::tick() {}
void GameObject::draw(const std::string& shader) {
	if (root_component == nullptr) return;
	auto t_shader = AssetManager::get_shader(shader);
	if(t_shader != nullptr) t_shader->use();
	root_component->draw(shader);
}

