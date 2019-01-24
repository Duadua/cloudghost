#include "scenecomponent.h"

SceneComponent::SceneComponent() : parent_component(nullptr), scale(QVector3D(1.0f, 1.0f, 1.0f)){}
SceneComponent::~SceneComponent() {
	for (auto cc : child_components) {
		delete cc;
	}
}

void SceneComponent::draw(Shader& shader) {

}

void SceneComponent::attach_to(SceneComponent* parent) {
	if (parent == nullptr) return;
	parent->add_child(this);
	parent_component = parent;
}

void SceneComponent::add_child(SceneComponent* child) {
	if (child != nullptr) child_components.append(child);
}

QMatrix4x4 SceneComponent::get_transform() {
	QMatrix4x4 t_transform;
	// 通过 lrs 计算 transform
	t_transform.translate(location);
	t_transform.rotate(rotation.x(), QVector3D(1.0f, 0.0f, 0.0f));
	t_transform.rotate(rotation.y(), QVector3D(0.0f, 1.0f, 0.0f));
	t_transform.rotate(rotation.z(), QVector3D(0.0f, 0.0f, 1.0f));
	t_transform.scale(scale);

	// 乘以 parent 的 transform
	if (parent_component != nullptr) {
		t_transform = parent_component->get_transform() * t_transform;
	}

	return t_transform;
}
