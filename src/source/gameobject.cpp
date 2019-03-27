#include "shader.h"
#include "gameobject.h"
#include "assetmanager.h"
#include "scenecomponent.h"

IMPLEMENT_CLASS(GameObject)

uint GameObject::num = 0;

GameObject::GameObject(SPTR_SceneComponent root) { 
	root_component = root; 
	if (root_component == nullptr) root_component = CREATE_CLASS(SceneComponent);

	id = ++num;
}
GameObject::~GameObject() {} 

void GameObject::bind_input() {}
void GameObject::begin_play() {}
void GameObject::tick() {}
void GameObject::draw(const std::string& shader) {
	if (root_component == nullptr) return;
	auto t_shader = AssetManager_ins().get_shader(shader);
	if (t_shader != nullptr) {
		t_shader->use();
		t_shader->set_uint("u_object_id", id);
		root_component->draw(shader);
	}
}

