#pragma once

class CVector3D;

class CVector2D {
public:
	CVector2D(float a = 0.0f, float b = 0.0f) : _x(a), _y(b) {}
	CVector2D(const CVector3D& abc);
	~CVector2D() {}
	
	void set_x(float a) { _x = a; }
	void set_y(float b) { _y = b; }
	
	float x() const { return _x; }
	float y() const { return _y; }

	float dot(const CVector2D& b) const;
	float cross(const CVector2D& b) const;

private:
	float _x;
	float _y;

};
