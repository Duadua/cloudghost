#include "scenecomponent.h"
#include <QVector3D>

IMPLEMENT_CLASS(SceneComponent)

SceneComponent::SceneComponent() : is_border(false), scale(CVector3D(1.0f, 1.0f, 1.0f)) {}
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
	if (child != nullptr) child_components.push_back(child);
}

CMatrix4x4 SceneComponent::get_transform() {
	CMatrix4x4 t_transform;
	// 通过 lrs 计算 transform
	t_transform.translate(location);
	t_transform.rotate_euler(rotation);
	/*t_transform.rotate(rotation.z(), 0.0f, 0.0f, 1.0f);
	t_transform.rotate(rotation.y(), 0.0f, 1.0f, 0.0f);
	t_transform.rotate(rotation.x(), 1.0f, 0.0f, 0.0f);
	*/
	t_transform.scale(scale);

	// 乘以 parent 的 transform
	if (!parent_component.expired()) {
		t_transform = parent_component.lock()->get_transform() * t_transform;
	} // expired() 返回 false 时 lock() 会返回一个 shared_ptr 对象

	return t_transform;
}

void SceneComponent::set_location(float x, float y, float z) { location = CVector3D(x, y, z); }
void SceneComponent::set_rotation(float x, float y, float z) { rotation = CVector3D(x, y, z); }
void SceneComponent::set_scale(float x, float y, float z) { scale = CVector3D(x, y, z); }

void SceneComponent::add_location(CVector3D l) { location += l; }
void SceneComponent::add_rotation(CVector3D	r) { rotation += r; }
void SceneComponent::add_scale(CVector3D s) { scale += s; }
void SceneComponent::add_location(float x, float y, float z) { location += CVector3D(x, y, z); }
void SceneComponent::add_rotation(float x, float y, float z) { rotation += CVector3D(x, y, z); }
void SceneComponent::add_scale(float x, float y, float z) { scale += CVector3D(x, y, z); }

CVector3D SceneComponent::get_location() { 
	CVector3D t_location = location;
	if (!parent_component.expired()) {
		t_location += parent_component.lock()->get_location();
	}
	return t_location; 
}
CVector3D SceneComponent::get_rotation() { 
	CVector3D t_rotation = rotation;
	if (!parent_component.expired()) {
		t_rotation += parent_component.lock()->get_rotation();
	}
	return t_rotation; 
}
CVector3D SceneComponent::get_scale() { 
	CVector3D t_scale = scale;
	if (!parent_component.expired()) {
		t_scale *= parent_component.lock()->get_scale();
	}
	return t_scale; 
}

void SceneComponent::set_all_border(bool border) {
	is_border = border;
	for (auto cc : child_components) { cc->set_all_border(border); }
}
