#pragma once

#include "cvector3d.h"
#include "cvector4d.h"

// 四元数

class CQuaternion {
public:
	CQuaternion(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) { normalize(); }
	CQuaternion(const CVector3D& euler_angle);
	CQuaternion(const CVector4D& angle_axis);
	~CQuaternion(){}

	CVector3D get_euler_angle() const;				// (pitch, yaw, roll) -- (x, y, z)
	CVector4D get_angle_axis() const;				// (angle, x, y, z)

	static CQuaternion llerp(const CQuaternion& a, const CQuaternion& b);	// linear lerp
	static CQuaternion slerp(const CQuaternion& a, const CQuaternion& b);	// spherical lerp

private:

	float x;			// x = v_x * sin(angle / 2)
	float y;			// y = v_y * sin(angle / 2)
	float z;			// z = v_z * sin(angle / 2)
	float w;			// w = cos(angle / 2)

	void normalize();	// x*x + y*y + z*z + w*w = 1

};
