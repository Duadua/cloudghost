#pragma once

#include "cvector3d.h"
#include <memory>

class CVector3D;

class CVector4D {
public:
	CVector4D(float a, float b, float c, float d) { v[0] = a; v[1] = b; v[2] = c; v[3] = d; }
	CVector4D(float a = 0.0f) { v[0] = v[1] = v[2] = v[3] = a; }
	CVector4D(const CVector3D& abc, float d = 0.0f) { v[0] = abc[0]; v[1] = abc[1]; v[2] = abc[2]; v[3] = d; }
	CVector4D(const float* d) { memcpy(v, d, sizeof(v)); }
	~CVector4D() {}

	void set_x(float a) { v[0] = a; }
	void set_y(float b) { v[1] = b; }
	void set_z(float c) { v[2] = c; }
	void set_w(float d) { v[3] = d; }

	float x() const { return v[0]; }
	float y() const { return v[1]; }
	float z() const { return v[2]; }
	float w() const { return v[3]; }

	CVector3D xyz() const { return CVector3D(v[0], v[1], v[2]); }
	CVector3D xyw() const { return CVector3D(v[0], v[1], v[3]); }
	CVector3D xzw() const { return CVector3D(v[0], v[2], v[3]); }
	CVector3D yzw() const { return CVector3D(v[1], v[2], v[3]); }

	CVector4D operator + (const CVector4D& b) const;
	CVector4D operator - (const CVector4D& b) const;
	CVector4D operator * (const CVector4D& b) const;
	CVector4D operator / (const CVector4D& b) const;

	friend CVector4D operator + (const float& v, const CVector4D& b);
	friend CVector4D operator - (const float& v, const CVector4D& b);
	friend CVector4D operator * (const float& v, const CVector4D& b);
	friend CVector4D operator / (const float& v, const CVector4D& b);

	CVector4D operator +=(const CVector4D& b);
	CVector4D operator *=(const CVector4D& b);

	CVector4D operator + () const;
	CVector4D operator - () const;

	inline float& operator[](int i) { if (i < 0) i = 0; if (i > 3) i = 3; return v[i]; }
	inline const float& operator[](int i) const { if (i < 0) i = 0; if (i > 3) i = 3; return v[i]; }

	float dot(const CVector4D& b) const;
	CVector4D cross(const CVector4D& b, const CVector4D& c) const;					// a X b X c
	float mix(const CVector4D& b, const CVector4D& c, const CVector4D& d) const;	// (a X b X c) dot d

	CVector4D normalize();

	float length() const;

	inline const float* data() const { return v; }
private:
	float v[4];
};
