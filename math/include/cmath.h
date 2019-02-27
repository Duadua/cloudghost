#pragma once

#include "ccolor.h"
#include "cvector2d.h"
#include "cvector3d.h"
#include "cvector4d.h"
#include "cmatrix4x4.h"
#include <memory>

using uint = unsigned int;
using uchar = unsigned char;

using UPTR_char = std::unique_ptr<char>;
using SPTR_char = std::shared_ptr<char>;
using WPTR_char = std::weak_ptr<char>;
using UPTR_uchar = std::unique_ptr<uchar>;
using SPTR_uchar = std::shared_ptr<uchar>;
using WPTR_uchar = std::weak_ptr<uchar>;

class CMath {
public:
	~CMath() {}
	static const float pi;

	template<class T>
	static T clamp(T x, T a, T b) { if (x < a) x = a; if (x > b) x = b; return x; }

	template<class T> 
	static T pow(T a, T p);

	static float rad_to_deg(float rad);
	static float deg_to_rad(float deg);

	static float interp_linear(float v, float min, float max);		// 线性插值 v ~ [0.0 ,, 1.0]

private:
	CMath() {}

};

template<class T> 
T CMath::pow(T a, T p) {
	T r = 1;
	while (p) {
		if (p & 1) r *= a;
		a *= a;
		p >>= 1;
	} 
	return r;
}

