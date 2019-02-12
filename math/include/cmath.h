#pragma once

#include "cvector2d.h"
#include "cvector3d.h"

using uint = unsigned int;

class CMath {
public:
	~CMath() {}
	static const float pi;

	template<class T>
	static T clamp(T x, T a, T b) { if (x < a) x = a; if (x > b) x = b; return x; }

	static float rad_to_deg(float rad);
	static float deg_to_rad(float deg);

private:
	CMath() {}

};
