#include "cquaternion.h"
#include "cmath.h"

CQuaternion::CQuaternion(const CVector3D& euler_angle) {

}
CQuaternion::CQuaternion(const CVector4D& angle_axis) {
	CVector3D t_r = angle_axis.yzw().normalize();
	float t_a = CMath::deg_to_rad(angle_axis[0] / 2.0f);		// angle / 2.0
	x = t_r[0] * (std::sin(t_a));
	y = t_r[1] * (std::sin(t_a));
	z = t_r[2] * (std::sin(t_a));
	w = std::cos(t_a);
}

CVector3D CQuaternion::get_euler_angle() const {
	return CVector3D();
}
CVector4D CQuaternion::get_angle_axis() const {
	float t_a = std::acos(w);	// angle / 2.0
	float angle = CMath::rad_to_deg(t_a * 2.0f);
	float v_x = x / std::sin(t_a);
	float v_y = y / std::sin(t_a);
	float v_z = z / std::sin(t_a);
	return CVector4D(angle, v_x, v_y, v_z);
}

CQuaternion operator * (const CQuaternion& a, const CQuaternion& b) {
	CVector3D u(a.x, a.y, a.z);
	CVector3D v(b.x, b.y, b.z);
	float t_w = a.w * b.w - u.dot(v);
	CVector3D t_r = a.w * v + b.w * u + u.cross(v);
	return CQuaternion(t_r[0], t_r[1], t_r[2], t_w);
}

CQuaternion CQuaternion::llerp(const CQuaternion& a, const CQuaternion& b) {
	return CQuaternion();
}
CQuaternion CQuaternion::slerp(const CQuaternion& a, const CQuaternion& b) {
	return CQuaternion();
}

void CQuaternion::rotate(CVector3D& v) const {
	CQuaternion t_q = get_normalize();
	CQuaternion t_v(v[0], v[1], v[2], 0.0f);
	CQuaternion t_r = t_q * t_v * t_q.inverse();
	v[0] = t_r.x; v[1] = t_r.y; v[2] = t_r.z;
}
