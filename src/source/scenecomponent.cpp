#include "scenecomponent.h"

SceneComponent::SceneComponent() {}
SceneComponent::~SceneComponent() {
	for (auto cc : child_components) {
		delete cc;
	}
}

void SceneComponent::draw() {

}

void SceneComponent::attach_to(SceneComponent* parent) {
	if (parent == nullptr) return;
	parent->add_child(this);
}

void SceneComponent::add_child(SceneComponent* child) {
	if (child != nullptr) child_components.append(child);
}
