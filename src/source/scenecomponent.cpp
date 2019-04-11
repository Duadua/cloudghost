#include "scenecomponent.h"
#include <QVector3D>

IMPLEMENT_CLASS(SceneComponent)

SceneComponent::SceneComponent() : is_border(false), scale(CVector3D(1.0f, 1.0f, 1.0f)), world_up(0.0f, 1.0f, 0.0f) {}
SceneComponent::~SceneComponent() {}

void SceneComponent::_begin_play() { 
	for (auto cc : child_components) { cc->_begin_play(); } 
	begin_play();
}
void SceneComponent::_tick(float time) {
	for (auto cc : child_components) { cc->_tick(time); }
	tick(time);
}
void SceneComponent::_draw(SPTR_Shader shader) {
	for (auto cc : child_components) { cc->_draw(shader); }
	draw(shader);
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
void SceneComponent::add_scale(CVector3D s) { scale *= s; }
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

CVector3D SceneComponent::get_front_axis() { update_rotation(); return front_axis; }
CVector3D SceneComponent::get_right_axis() { update_rotation(); return right_axis; }
CVector3D SceneComponent::get_up_axis() { update_rotation(); return up_axis; }
CVector3D SceneComponent::get_world_up() { update_rotation(); return world_up; }

void SceneComponent::update_rotation() {
	float yaw = CMath_ins().deg_to_rad(rotation.y());
	float pitch = CMath_ins().deg_to_rad(rotation.x());
	float roll = CMath_ins().deg_to_rad(rotation.z());

	// 计算 world_up
	world_up.set_z(0.0f);
	world_up.set_x(std::sin(roll));
	world_up.set_y(std::cos(roll));
	world_up.normalize();

	// 计算 front -- 初始方向 (0.0, 0.0, 0.0)
	front_axis.set_x(std::sin(yaw) * std::cos(pitch));
	front_axis.set_z(std::cos(yaw) * std::cos(pitch));
	front_axis.set_y(std::sin(pitch));
	front_axis.normalize();

	// 计算 right
	right_axis = front_axis.cross(world_up);
	right_axis.normalize();

	// 计算 up
	up_axis = right_axis.cross(front_axis);
	up_axis.normalize();
}

