#pragma once

#include "cvector3d.h"
#include "cvector4d.h"

// 四元数

class CQuaternion {
public:
	CQuaternion(){}
	~CQuaternion(){}
	CQuaternion(const CVector3D& euler_angle){}
	CQuaternion(const CVector4D& angle_axis){}

	CVector3D get_euler_angle() const;
	CVector4D get_angle_axis() const;				// (angle, x, y , z)
private:

};
