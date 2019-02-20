#pragma once

#include "ccolor.h"
#include "cvector2d.h"
#include "cvector3d.h"
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

	static float rad_to_deg(float rad);
	static float deg_to_rad(float deg);

private:
	CMath() {}

};
