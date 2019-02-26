#include "cvector4d.h" 

CVector4D operator + (const CVector4D& a, const CVector4D& b) { return CVector4D(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]); }
CVector4D operator - (const CVector4D& a, const CVector4D& b) { return CVector4D(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]); }
CVector4D operator * (const CVector4D& a, const CVector4D& b) { return CVector4D(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);  }
CVector4D operator / (const CVector4D& a, const CVector4D& b) {
	if (b[0] == 0.0f || b[1] == 0.0f || b[2] == 0.0f || b[3] == 0.0f) { return CVector4D(); }
	return CVector4D(a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]);
}

CVector4D CVector4D::operator +=(const CVector4D& b) { v[0] += b[0]; v[1] += b[1]; v[2] += b[2]; v[3] += b[3]; return (*this); }
CVector4D CVector4D::operator *=(const CVector4D& b) { v[0] *= b[0]; v[1] *= b[1]; v[2] *= b[2]; v[3] *= b[3]; return (*this); }

CVector4D CVector4D::operator + () const { return (*this); }
CVector4D CVector4D::operator - () const { return CVector4D(-v[0], -v[1], -v[2], -v[3]); }

float CVector4D::dot(const CVector4D& b) const { return v[0] * b[0] + v[1] * b[1] + v[2] * b[2] + v[3] * b[3]; }
CVector4D CVector4D::cross(const CVector4D& b, const CVector4D& c) const {
	return CVector4D();
} // not complete
float CVector4D::mix(const CVector4D& b, const CVector4D& c, const CVector4D& d) const { return dot(b.cross(c, d)); }

CVector4D CVector4D::normalize() { float len = length(); v[0] /= len; v[1] /= len; v[2] /= len; v[3] /= len; return (*this); }

float CVector4D::length() const { return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]); }


