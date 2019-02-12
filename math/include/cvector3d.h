#pragma once

#include "cvector2d.h"

class CVector3D {
public:
	CVector3D(float a, float b, float c) : _x(a), _y(b), _z(c) {}
	CVector3D(float a = 0.0f) : _x(a), _y(a), _z(a) {}
	CVector3D(CVector2D ab, float c = 0.0f) : _z(c) { _x = ab.x(); _y = ab.y(); }
	~CVector3D() {}

	void set_x(float a) { _x = a; }
	void set_y(float b) { _y = b; }
	void set_z(float c) { _z = c; }

	float x() const { return _x; }
	float y() const { return _y; }
	float z() const { return _z; }
	
	CVector2D xy() const { return CVector2D(_x, _y); }
	CVector2D xz() const { return CVector2D(_x, _z); }
	CVector2D yz() const { return CVector2D(_y, _z); }

	CVector3D operator + (const CVector3D& b) const;
	CVector3D operator - (const CVector3D& b) const;
	CVector3D operator * (const CVector3D& b) const;
	CVector3D operator / (const CVector3D& b) const;
	CVector3D operator +=(const CVector3D& b);

	float dot(const CVector3D& b) const;
	CVector3D cross(const CVector3D& b) const;
	float mix(const CVector3D& b, const CVector3D& c) const;	// 混合积 -- 有向六面体休积

	CVector3D normalize();

	float length() const;

private:
	float _x;
	float _y;
	float _z;

};