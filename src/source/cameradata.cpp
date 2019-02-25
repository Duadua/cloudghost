#include "cameradata.h"

IMPLEMENT_CLASS(CameraData)

CameraData::CameraData() {}
CameraData::~CameraData() {}

QMatrix4x4 CameraData::get_view_mat() { return look_at(); }

void CameraData::update(CVector3D l, CVector3D r) {
	location = l;
	rotation = r;

	float yaw = CMath::deg_to_rad(rotation.y());
	float pitch = CMath::deg_to_rad(rotation.x());
	float roll = CMath::deg_to_rad(rotation.z());

	// 计算 world_up
	world_up.set_z(0.0f);
	world_up.set_x(std::sin(roll));
	world_up.set_y(std::cos(roll));
	world_up.normalize();

	// 计算 front -- 初始方向 (0.0, 0.0, 0.0)
	front.set_x(std::sin(yaw) * std::cos(pitch));
	front.set_z(std::cos(yaw) * std::cos(pitch));
	front.set_y(std::sin(pitch));
	front.normalize();

	// 计算 right
	right = front.cross(world_up);
	right.normalize();

	// 计算 up
	up = right.cross(front);
	up.normalize();
}

QMatrix4x4 CameraData::look_at() {
	QMatrix4x4 translate, rotate;

	// 初始化 translate
	translate(0, 3) = -location.x();
	translate(1, 3) = -location.y();
	translate(2, 3) = -location.z();

	// 初始化 rotate
	rotate(0, 0) = -right.x();	rotate(0, 1) = -right.y();	rotate(0, 2) = -right.z(); 
	rotate(1, 0) = up.x();		rotate(1, 1) = up.y();		rotate(1, 2) = up.z(); 
	rotate(2, 0) = -front.x();	rotate(2, 1) = -front.y();	rotate(2, 2) = -front.z(); 

	return rotate * translate;
}
