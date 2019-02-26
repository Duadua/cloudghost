#include "cvector2d.h"
#include "cvector3d.h"
#include <cmath>

CVector2D::CVector2D(const CVector3D& abc) { v[0] = abc.x(); v[1] = abc.y(); }

CVector2D CVector2D::operator + (const CVector2D& b) const { return CVector2D(v[0] + b[0], v[1] + b[1]); }
CVector2D CVector2D::operator - (const CVector2D& b) const { return CVector2D(v[0] - b[0], v[1] - b[1]); }
CVector2D CVector2D::operator * (const CVector2D& b) const { return CVector2D(v[0] * b[0], v[1] * b[1]); }
CVector2D CVector2D::operator / (const CVector2D& b) const {
	if (b[0] == 0.0f || b[1] == 0.0f) return CVector2D();
	return CVector2D(v[0] / b[0], v[1] / b[1]);
}

CVector2D operator + (const float& v, const CVector2D& b) { return b + v;  }
CVector2D operator - (const float& v, const CVector2D& b) { return -b + v; }
CVector2D operator * (const float& v, const CVector2D& b) { return b * v;  }
CVector2D operator / (const float& v, const CVector2D& b) {
	if (b[0] == 0.0f || b[1] == 0.0f) { return CVector2D(); }
	return CVector2D(v / b[0], v / b[1]);
}

CVector2D CVector2D::operator +=(const CVector2D& b) { v[0] += b[0]; v[1] += b[1]; return (*this); }
CVector2D CVector2D::operator *=(const CVector2D& b) { v[0] *= b[0]; v[1] *= b[1]; return (*this); }

CVector2D CVector2D::operator + () const { return (*this); }
CVector2D CVector2D::operator - () const { return CVector2D(-v[0], -v[1]); }

float CVector2D::dot(const CVector2D& b) const { return v[0] * b[0] + v[1] * b[1]; }
float CVector2D::cross(const CVector2D& b) const { return v[0] * b[1] - b[0] * v[1]; }

CVector2D CVector2D::normalize() { float len = length(); v[0] /= len; v[1] /= len; return (*this); }

float CVector2D::length() const { return std::sqrt(v[0]*v[0] + v[1]*v[1]); }
