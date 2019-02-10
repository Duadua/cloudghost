#include "cvector2d.h"
#include "cvector3d.h"
#include <cmath>

CVector2D::CVector2D(const CVector3D& abc) { _x = abc.x(); _y = abc.y(); }

CVector2D CVector2D::operator + (const CVector2D& b) const { return CVector2D(_x + b.x(), _y + b.y()); }
CVector2D CVector2D::operator - (const CVector2D& b) const { return CVector2D(_x - b.x(), _y - b.y()); }
CVector2D CVector2D::operator * (const CVector2D& b) const { return CVector2D(_x * b.x(), _y * b.y()); }
CVector2D CVector2D::operator / (const CVector2D& b) const {
	if (b.x() == 0.0f || b.y() == 0.0f) return CVector2D();
	return CVector2D(_x / b.x(), _y / b.y());
}
CVector2D CVector2D::operator +=(const CVector2D& b) { _x += b.x(); _y += b.y(); return (*this); }

float CVector2D::dot(const CVector2D& b) const { return _x * b.x() + _y * b.y(); }
float CVector2D::cross(const CVector2D& b) const { return _x * b.y() - b.x()*_y; }

void CVector2D::normalize() { _x /= length(); _y /= length(); }

float CVector2D::length() const { return std::sqrt(_x*_x + _y*_y); }
