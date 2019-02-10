#include "cvector3d.h"
#include <cmath>

CVector3D CVector3D::operator + (const CVector3D& b) const { return CVector3D(_x + b.x(), _y + b.y(), _z + b.z()); }
CVector3D CVector3D::operator - (const CVector3D& b) const { return CVector3D(_x - b.x(), _y - b.y(), _z - b.z()); }
CVector3D CVector3D::operator * (const CVector3D& b) const { return CVector3D(_x * b.x(), _y * b.y(), _z * b.z()); }
CVector3D CVector3D::operator / (const CVector3D& b) const { 
	if (b.x() == 0.0f || b.y() == 0.0f || b.z() == 0.0f) { return CVector3D(); }
	return CVector3D(_x / b.x(), _y / b.y(), _z / b.z()); 
}
CVector3D CVector3D::operator +=(const CVector3D& b) { _x += b.x(); _y += b.y(); _z += b.z(); return (*this); }

float CVector3D::dot(const CVector3D& b) const { return _x * b.x() + _y * b.y() + _z * b.z(); }
CVector3D CVector3D::cross(const CVector3D& b) const {
	return CVector3D(_y*b.z() - b.y()*_z, b.x()*_z - _x*b.z(), _x*b.y() - b.x()*_y); 
}

void CVector3D::normalize() { _x /= length(); _y /= length(); _z /= length(); }

float CVector3D::length() const { return std::sqrt(_x*_x + _y*_y + _z*_z); }
