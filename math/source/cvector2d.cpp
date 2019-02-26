#include "cvector2d.h"
#include <cmath>

CVector2D operator + (const CVector2D& a, const CVector2D& b) { return CVector2D(a[0] + b[0], a[1] + b[1]); }
CVector2D operator - (const CVector2D& a, const CVector2D& b) { return CVector2D(a[0] - b[0], a[1] - b[1]); }
CVector2D operator * (const CVector2D& a, const CVector2D& b) { return CVector2D(a[0] * b[0], a[1] * b[1]); }
CVector2D operator / (const CVector2D& a, const CVector2D& b) {
	if (b[0] == 0.0f || b[1] == 0.0f) { return CVector2D(); }
	return CVector2D(a[0] / b[0], a[1] / b[1]);
}

CVector2D CVector2D::operator +=(const CVector2D& b) { v[0] += b[0]; v[1] += b[1]; return (*this); }
CVector2D CVector2D::operator *=(const CVector2D& b) { v[0] *= b[0]; v[1] *= b[1]; return (*this); }

CVector2D CVector2D::operator + () const { return (*this); }
CVector2D CVector2D::operator - () const { return CVector2D(-v[0], -v[1]); }

float CVector2D::dot(const CVector2D& b) const { return v[0] * b[0] + v[1] * b[1]; }
float CVector2D::cross(const CVector2D& b) const { return v[0] * b[1] - b[0] * v[1]; }

CVector2D CVector2D::normalize() { float len = length(); v[0] /= len; v[1] /= len; return (*this); }

float CVector2D::length() const { return std::sqrt(v[0]*v[0] + v[1]*v[1]); }


