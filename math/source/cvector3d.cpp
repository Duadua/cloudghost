#include "cvector3d.h"
#include <cmath>

CVector3D CVector3D::operator + (const CVector3D& b) const { return CVector3D(_x + b.x(), _y + b.y(), _z + b.z()); }
CVector3D CVector3D::operator - (const CVector3D& b) const { return CVector3D(_x - b.x(), _y - b.y(), _z - b.z()); }
CVector3D CVector3D::operator * (const CVector3D& b) const { return CVector3D(_x * b.x(), _y * b.y(), _z * b.z()); }
CVector3D CVector3D::operator / (const CVector3D& b) const { 
	if (b.x() == 0.0f || b.y() == 0.0f || b.z() == 0.0f) { return CVector3D(); }
	return CVector3D(_x / b.x(), _y / b.y(), _z / b.z()); 
}

CVector3D operator + (const float& v, const CVector3D& b) { return b + v;  }
CVector3D operator - (const float& v, const CVector3D& b) { return -b + v; }
CVector3D operator * (const float& v, const CVector3D& b) { return b * v; }
CVector3D operator / (const float& v, const CVector3D& b) {
	if (b.x() == 0.0f || b.y() == 0.0f || b.z() == 0.0f) { return CVector3D(); }
	return CVector3D(v / b.x(), v / b.y(), v / b.z());
}

CVector3D CVector3D::operator +=(const CVector3D& b) { _x += b.x(); _y += b.y(); _z += b.z(); return (*this); }
CVector3D CVector3D::operator *=(const CVector3D& b) { _x *= b.x(); _y *= b.y(); _z *= b.z(); return (*this); }

CVector3D CVector3D::operator +() const { return (*this); }
CVector3D CVector3D::operator -() const { return CVector3D(-_x, -_y, -_z); } 

float CVector3D::dot(const CVector3D& b) const { return _x * b.x() + _y * b.y() + _z * b.z(); }
CVector3D CVector3D::cross(const CVector3D& b) const {
	return CVector3D(_y*b.z() - b.y()*_z, b.x()*_z - _x*b.z(), _x*b.y() - b.x()*_y); 
}
float CVector3D::mix(const CVector3D& b, const CVector3D& c) const { return dot(b.cross(c)); }

CVector3D CVector3D::normalize() { float len = length(); _x /= len; _y /= len; _z /= len; return (*this); }

float CVector3D::length() const { return std::sqrt(_x*_x + _y*_y + _z*_z); }
