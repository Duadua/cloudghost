#pragma once

#include "cvector3d.h"
#include "cvector4d.h"

// 四元数

class CQuaternion {
public:
	CQuaternion(float a = 0.0f, float b = 0.0f, float c = 0.0f, float d = 1.0f) : x(a), y(b), z(c), w(d) {}
	CQuaternion(const CVector3D& euler_angle);
	CQuaternion(const CVector4D& angle_axis);
	CQuaternion(float angle, const CVector3D& axis);
	~CQuaternion(){}

	CVector3D get_euler_angle() const;				// (pitch, yaw, roll) -- (x, y, z)
	CVector4D get_angle_axis() const;				// (angle, x, y, z)

	friend CQuaternion operator * (const CQuaternion& a, const CQuaternion& b);
	CQuaternion operator / (const float v) const { return CQuaternion(x / v, y / v, z / v, w / v); }

	CQuaternion& normalize() { float len = length(); x /= len; y /= len; z /= len; w /= len; return (*this); }	// x*x + y*y + z*z + w*w = 1
	CQuaternion get_normalize() const { CQuaternion res = (*this); return res.normalize(); }

	float length() const { return std::sqrt(x*x + y * y + z * z + w * w); }
	float length_2() const { return x * x + y * y + z * z + w * w; }

	CQuaternion conjugate() const { return CQuaternion(-x, -y, -z, w); }	// 共轭四元数
	CQuaternion inverse() const { return conjugate() / length_2(); }		// 四元数的 逆

	void rotate(CVector3D& v) const;				// 旋转向量

	static CQuaternion llerp(const CQuaternion& a, const CQuaternion& b);	// linear lerp
	static CQuaternion slerp(const CQuaternion& a, const CQuaternion& b);	// spherical lerp

private:

	float x;			// x = v_x * sin(angle / 2)
	float y;			// y = v_y * sin(angle / 2)
	float z;			// z = v_z * sin(angle / 2)
	float w;			// w = cos(angle / 2)

	friend class CMatrix4x4;


};
