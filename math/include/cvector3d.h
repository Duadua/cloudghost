#pragma once

#include "cvector2d.h"
#include <memory>

class CVector4D;

class CVector3D {
public:
	CVector3D(float a, float b, float c) { v[0] = a; v[1] = b; v[2] = c; }
	CVector3D(float a = 0.0f) { v[0] = v[1] = v[2] = a; }
	CVector3D(const CVector2D& ab, float c = 0.0f) { v[0] = ab[0]; v[1] = ab[1]; v[2] = c; }
	CVector3D(const CVector4D& abcd);
	CVector3D(const float* d) { memcpy(v, d, sizeof(v)); }
	~CVector3D() {}

	void set_x(float a) { v[0] = a; }
	void set_y(float b) { v[1] = b; }
	void set_z(float c) { v[2] = c; }

	float x() const { return v[0]; }
	float y() const { return v[1]; }
	float z() const { return v[2]; }
	
	CVector2D xy() const { return CVector2D(v[0], v[1]); }
	CVector2D xz() const { return CVector2D(v[0], v[2]); }
	CVector2D yz() const { return CVector2D(v[1], v[2]); }

	friend CVector3D operator + (const CVector3D& a, const CVector3D& b);
	friend CVector3D operator - (const CVector3D& a, const CVector3D& b);
	friend CVector3D operator * (const CVector3D& a, const CVector3D& b);
	friend CVector3D operator / (const CVector3D& a, const CVector3D& b);

	CVector3D operator +=(const CVector3D& b);
	CVector3D operator *=(const CVector3D& b);

	CVector3D operator + () const;
	CVector3D operator - () const;

	float& operator[](int i) { if (i < 0) i = 0; if (i > 2) i = 2; return v[i]; }
	const float& operator[](int i) const { if (i < 0) i = 0; if (i > 2) i = 2; return v[i]; }

	float dot(const CVector3D& b) const;
	CVector3D cross(const CVector3D& b) const;
	float mix(const CVector3D& b, const CVector3D& c) const;	// 混合积 -- 有向六面体休积

	CVector3D& normalize();
	CVector3D get_normalize() const;

	float length() const;

	const float* data() const { return v; }

private:
	float v[3];
};