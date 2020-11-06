#include "cameradata.h"

#include "cdebuger.h"

IMPLEMENT_CLASS(CameraData)

CameraData::CameraData() {}
CameraData::~CameraData() {}

CMatrix4x4 CameraData::get_view_mat() { return look_at(); }

CMatrix4x4 CameraData::get_proj_mat() {
	CMatrix4x4 res;
	float t_ratio = static_cast<float>(frustum.width) / (frustum.heigh == 0 ? 1 : frustum.heigh);
	res.perspective(frustum.fov, t_ratio, frustum.near, frustum.far);
	return res;
}

void CameraData::update(CVector3D l, CVector3D r) {
	location = l;
	rotation = r;

	float yaw = CMath_ins().deg_to_rad(rotation.y());
	float pitch = CMath_ins().deg_to_rad(rotation.x());
	float roll = CMath_ins().deg_to_rad(rotation.z());
	// 计算 world_up
	world_up.set_z(0.0f);
	world_up.set_x(std::sin(roll));
	world_up.set_y(std::cos(roll));
	world_up.normalize();

	// 计算 front -- 初始方向 (0.0, 0.0, 1.0)
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

CMatrix4x4 CameraData::look_at() {
	CMatrix4x4 res;
	return res.lookAt(location, location + front, world_up);
}
