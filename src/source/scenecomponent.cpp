#include "scenecomponent.h"

IMPLEMENT_CLASS(SceneComponent)

SceneComponent::SceneComponent() : scale(QVector3D(1.0f, 1.0f, 1.0f)) {}
SceneComponent::~SceneComponent() {}

void SceneComponent::draw(const std::string& shader) {
	for (auto cc : child_components) { cc->draw(shader); }
}

void SceneComponent::attach_to(SPTR_SceneComponent parent) {
	if (parent == nullptr) return;
	parent->add_child(shared_from_this());
	parent_component = parent;
}

void SceneComponent::add_child(SPTR_SceneComponent child) {
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
	if (!parent_component.expired()) {
		t_transform = parent_component.lock()->get_transform() * t_transform;
	} // expired() 返回 false 时 lock() 会返回一个 shared_ptr 对象

	return t_transform;
}

void SceneComponent::set_location(QVector3D l) { location = l; }
void SceneComponent::set_roataion(QVector3D r) { rotation = r; }
void SceneComponent::set_scale(QVector3D s) { scale = s; }

QVector3D SceneComponent::get_location() { 
	QVector3D t_location = location;
	if (!parent_component.expired()) {
		t_location += parent_component.lock()->get_location();
	}
	return t_location; 
}
QVector3D SceneComponent::get_rotation() { 
	QVector3D t_rotation = rotation;
	if (!parent_component.expired()) {
		t_rotation += parent_component.lock()->get_rotation();
	}
	return t_rotation; 
}
QVector3D SceneComponent::get_scale() { 
	QVector3D t_scale = scale;
	if (!parent_component.expired()) {
		t_scale *= parent_component.lock()->get_scale();
	}
	return t_scale; 
}
