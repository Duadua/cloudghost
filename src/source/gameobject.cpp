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

void GameObject::_begin_play() {
	if (root_component) root_component->_begin_play();
	begin_play();
}
void GameObject::_tick(float time) {
	if (root_component) root_component->_tick(time);
	tick(time);
}
void GameObject::_draw(SPTR_Shader shader) {
	if (root_component == nullptr) return;
	if (shader != nullptr) {
		shader->use();
		shader->set_uint("u_object_id", id);
        draw(shader);
		root_component->_draw(shader);
	}


}
