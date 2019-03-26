#pragma once

#include "cvector3d.h"
#include "cvector4d.h"

// 四元数

class CQuaternion {
public:
	CQuaternion(float a = 0.0f, float b = 0.0f, float c = 0.0f, float d = 1.0f) : x(a), y(b), z(c), w(d) {}
    CQuaternion(const CQuaternion& q) { x = q.x; y = q.y; z = q.z; w = q.w; }
	void from_euler_angle(const CVector3D& euler_angle);
	void from_angle_axis(const CVector4D& angle_axis);
	void from_angle_axis(float angle, const CVector3D& axis);
	~CQuaternion(){}

	CVector3D get_euler_angle() const;						// (pitch, yaw, roll) -- (x, y, z)
	CVector4D get_angle_axis() const;						// (angle, x, y, z)
	inline float get_theta() const { return std::acos(w); }	// angle / 2.0f -- 弧度 rad

	CQuaternion operator +(const CQuaternion& b) const;
	CQuaternion operator -(const CQuaternion& b) const;
	CQuaternion operator *(const CQuaternion& b) const;			// Graßmann 积 格拉斯曼积
	CQuaternion operator *(float b) const;
	friend CQuaternion operator *(float a, const CQuaternion& b);
	CQuaternion operator / (const float v) const { return CQuaternion(x / v, y / v, z / v, w / v); }

	friend std::ostream& operator << (std::ostream& out, const CQuaternion& b);
	std::string to_string() const;

	CQuaternion operator -() const { return CQuaternion(-x, -y, -z, -w); }
	CQuaternion power(float t) const;

	CQuaternion& normalize() { float len = length(); x /= len; y /= len; z /= len; w /= len; return (*this); }	// x*x + y*y + z*z + w*w = 1
	CQuaternion get_normalize() const { CQuaternion res = (*this); return res.normalize(); }

	float length() const { return std::sqrt(x*x + y * y + z * z + w * w); }
	float length_2() const { return x * x + y * y + z * z + w * w; }

	float dot(const CQuaternion& b) const { return x * b.x + y * b.y + z * b.z + w * b.w; }	// 点积
	CQuaternion conjugate() const { return CQuaternion(-x, -y, -z, w); }	// 共轭四元数
	CQuaternion inverse() const { return conjugate() / length_2(); }		// 四元数的 逆

	void rotate(CVector3D& v) const;				// 旋转向量

	friend CQuaternion llerp(const CQuaternion& a, const CQuaternion& b, float t);	// linear lerp -- 线性插值
	friend CQuaternion slerp(const CQuaternion& a, const CQuaternion& b, float t);	// spherical lerp -- 球面插值
	friend CQuaternion squad(const CQuaternion& a, const CQuaternion& tga,
		const CQuaternion& tgb, const CQuaternion& b, float t);						// 球面四形插值 -- 用于插值一组四元数而确保平滑

private:

	float x;			// x = v_x * sin(angle / 2)
	float y;			// y = v_y * sin(angle / 2)
	float z;			// z = v_z * sin(angle / 2)
	float w;			// w = cos(angle / 2)

	friend class CMatrix4x4;


};
