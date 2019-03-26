#include "cquaternion.h"
#include "cmath.h"
#include <sstream>

void CQuaternion::from_euler_angle(const CVector3D& euler_angle) {
    if(euler_angle.x() != 0.0f) {}
}
void CQuaternion::from_angle_axis(const CVector4D& angle_axis) {
	CVector3D t_r = angle_axis.yzw().normalize();
	float t_a = CMath_ins().deg_to_rad(angle_axis[0] / 2.0f);		// angle / 2.0
	x = t_r[0] * (std::sin(t_a));
	y = t_r[1] * (std::sin(t_a));
	z = t_r[2] * (std::sin(t_a));
	w = std::cos(t_a);
}
void CQuaternion::from_angle_axis(float angle, const CVector3D& axis) {
	CVector3D t_r = axis.get_normalize();
	float t_a = CMath_ins().deg_to_rad(angle / 2.0f);		// angle / 2.0
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
	float angle = CMath_ins().rad_to_deg(t_a * 2.0f);
	float v_x = x / std::sin(t_a);
	float v_y = y / std::sin(t_a);
	float v_z = z / std::sin(t_a);
	return CVector4D(angle, v_x, v_y, v_z);
}

CQuaternion CQuaternion::operator + (const CQuaternion& b) const {
	return CQuaternion(x + b.x, y + b.y, z + b.z, w + b.w);
}
CQuaternion CQuaternion::operator - (const CQuaternion& b) const {
	return CQuaternion(x - b.x, y - b.y, z - b.z, w - b.w);
}
CQuaternion CQuaternion::operator *(const CQuaternion& b) const {
	CVector3D u(x, y, z);
	CVector3D v(b.x, b.y, b.z);
	float t_w = w * b.w - u.dot(v);
	CVector3D t_r = w * v + b.w * u + u.cross(v);
	return CQuaternion(t_r[0], t_r[1], t_r[2], t_w);
}

CQuaternion CQuaternion::operator *(float b) const { return CQuaternion(x*b, y*b, z*b, w*b); }

CQuaternion operator *(float a, const CQuaternion& b) { return b * a; }

std::ostream& operator << (std::ostream& out, const CQuaternion& b) {
	out << "CQuaternion( [" << b.x << ", " << b.y << ", " << b.z << "], " << b.w << " )";
	return out;

}
std::string CQuaternion::to_string() const {
	std::ostringstream oss;
	oss << (*this);
	return oss.str();
}

CQuaternion CQuaternion::power(float t) const {
	CVector4D t_aa = get_angle_axis();
	float t_a = CMath_ins().deg_to_rad(t_aa.x() * 0.5f);
	float t_c = std::cos(t*t_a);
	float t_s = std::sin(t*t_a);
	return CQuaternion(t_aa.y() * t_s, t_aa.z() * t_s, t_aa.w() * t_s, t_c);	// [cos(t*theta), sin(t*theta)u]
}

void CQuaternion::rotate(CVector3D& v) const {
	CQuaternion t_q = get_normalize();
	CQuaternion t_v(v[0], v[1], v[2], 0.0f);
	CQuaternion t_r = t_q * t_v * t_q.inverse();
	v[0] = t_r.x; v[1] = t_r.y; v[2] = t_r.z;
}

CQuaternion llerp(const CQuaternion& a, const CQuaternion& b, float t) { 
	CQuaternion res = (1.0f - t)*a + t * b; 
	res.normalize();
	return res;
}

CQuaternion slerp(const CQuaternion& a, const CQuaternion& b, float t) {
	CQuaternion bb(b);

	float t_dot = a.dot(b);
	if (CMath_ins().fcmp(t_dot, 0.0f) < 0) {
		t_dot = -t_dot;
		bb = -bb;
	}	// 保证插值路线最短 -- 钝角时反转

	float t_a = (1.0f - t);
	float t_b = t;

	if (CMath_ins().fcmp(t_dot, 1.0f) < 0) {
		t_dot = CMath_ins().clamp(t_dot, -1.0f, 1.0f);
		float t_theta = std::acos(t_dot);
		float t_sin = std::sin(t_theta);
		if (CMath_ins().fcmp(t_sin, 0.0f) > 0) {
			t_a = std::sin((1.0f-t)*t_theta) / t_sin;
			t_b = std::sin(t*t_theta) / t_sin;
		}	// 此时才用 slerp

	} // t_dot 趋近1时角度过小 -- 直接使用线插

	return t_a * a + t_b * b;
}
CQuaternion squad(const CQuaternion& a, const CQuaternion& tga, const CQuaternion& tgb, const CQuaternion& b, float t) {
	CQuaternion t_sa = tga;				// 临时 -- 不是这么算的
	CQuaternion t_sb = tgb;

	return slerp(slerp(a, b, t), slerp(t_sa, t_sb, t), 2.0f*t*(1.0f-t));
}
