#pragma once

class CVector3D;

class CVector2D {
public:
	CVector2D(float a, float b) { v[0] = a; v[1] = b; }
	CVector2D(float a = 0.0f) { v[0] = v[1] = a; }
	CVector2D(const CVector3D& abc);
	~CVector2D() {}
	
	void set_x(float a) { v[0] = a; }
	void set_y(float b) { v[1] = b; }
	
	float x() const { return v[0]; }
	float y() const { return v[1]; }
	
	CVector2D operator + (const CVector2D& b) const;
	CVector2D operator - (const CVector2D& b) const;
	CVector2D operator * (const CVector2D& b) const;
	CVector2D operator * (const float& b) const;
	CVector2D operator / (const CVector2D& b) const;
	CVector2D operator +=(const CVector2D& b);

	float dot(const CVector2D& b) const;
	float cross(const CVector2D& b) const;

	CVector2D normalize();

	float length() const;

private:
	float v[2];

};
