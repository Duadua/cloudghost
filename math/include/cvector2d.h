#pragma once

#include <memory>

class CVector3D;

class CVector2D {
public:
	CVector2D(float a, float b) { v[0] = a; v[1] = b; }
	CVector2D(float a = 0.0f) { v[0] = v[1] = a; }
	CVector2D(const CVector3D& abc);
	CVector2D(const float* d) { memcpy(v, d, sizeof(v)); }
	~CVector2D() {}
	
	void set_x(float a) { v[0] = a; }
	void set_y(float b) { v[1] = b; }
	
	float x() const { return v[0]; }
	float y() const { return v[1]; }
	
	CVector2D operator + (const CVector2D& b) const;
	CVector2D operator - (const CVector2D& b) const;
	CVector2D operator * (const CVector2D& b) const;
	CVector2D operator / (const CVector2D& b) const;

	friend CVector2D operator + (const float& v, const CVector2D& b);
	friend CVector2D operator - (const float& v, const CVector2D& b);
	friend CVector2D operator * (const float& v, const CVector2D& b);
	friend CVector2D operator / (const float& v, const CVector2D& b);

	CVector2D operator +=(const CVector2D& b);
	CVector2D operator *=(const CVector2D& b);

	CVector2D operator + () const;
	CVector2D operator - () const;

	inline float& operator[](int i) { if (i < 0) i = 0; if (i > 1) i = 1; return v[i]; }
	inline const float& operator[](int i) const { if (i < 0) i = 0; if (i > 1) i = 1; return v[i]; }

	float dot(const CVector2D& b) const;
	float cross(const CVector2D& b) const;

	CVector2D normalize();

	float length() const;

	inline const float* data() const { return v; }

private:
	float v[2];

};
