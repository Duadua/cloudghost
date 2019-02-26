#include "cvector3d.h"
#include "cvector4d.h"
#include <cmath>

CVector3D::CVector3D(const CVector4D& abcd) { v[0] = abcd[0]; v[1] = abcd[1]; v[2] = abcd[2]; }

CVector3D CVector3D::operator + (const CVector3D& b) const { return CVector3D(v[0] + b[0], v[1] + b[1], v[2] + b[2]); }
CVector3D CVector3D::operator - (const CVector3D& b) const { return CVector3D(v[0] - b[0], v[1] - b[1], v[2] - b[2]); }
CVector3D CVector3D::operator * (const CVector3D& b) const { return CVector3D(v[0] * b[0], v[1] * b[1], v[2] * b[2]); }
CVector3D CVector3D::operator / (const CVector3D& b) const { 
	if (b[0] == 0.0f || b[1] == 0.0f || b[2] == 0.0f) { return CVector3D(); }
	return CVector3D(v[0] / b[0], v[1] / b[1], v[2] / b[2]); 
}

CVector3D operator + (const float& v, const CVector3D& b) { return b + v;  }
CVector3D operator - (const float& v, const CVector3D& b) { return -b + v; }
CVector3D operator * (const float& v, const CVector3D& b) { return b * v;  }
CVector3D operator / (const float& v, const CVector3D& b) {
	if (b[0] == 0.0f || b[1] == 0.0f || b[2] == 0.0f) { return CVector3D(); }
	return CVector3D(v / b[0], v / b[1], v / b[2]);
}

CVector3D CVector3D::operator +=(const CVector3D& b) { v[0] += b[0]; v[1] += b[1]; v[2] += b[2]; return (*this); }
CVector3D CVector3D::operator *=(const CVector3D& b) { v[0] *= b[0]; v[1] *= b[1]; v[2] *= b[2]; return (*this); }

CVector3D CVector3D::operator +() const { return (*this); }
CVector3D CVector3D::operator -() const { return CVector3D(-v[0], -v[1], -v[2]); } 

float CVector3D::dot(const CVector3D& b) const { return v[0] * b[0] + v[1] * b[1] + v[2] * b[2]; }
CVector3D CVector3D::cross(const CVector3D& b) const {
	return CVector3D(v[1]*b[2] - b[1]*v[2], b[0]*v[2] - v[0]*b[2], v[0]*b[1] - b[0]*v[1]); 
}
float CVector3D::mix(const CVector3D& b, const CVector3D& c) const { return dot(b.cross(c)); }

CVector3D CVector3D::normalize() { float len = length(); v[0] /= len; v[1] /= len; v[2] /= len; return (*this); }

float CVector3D::length() const { return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }
