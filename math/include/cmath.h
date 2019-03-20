#pragma once

#include "ccolor.h"
#include "cvector2d.h"
#include "cvector3d.h"
#include "cvector4d.h"
#include "cmatrix4x4.h"
#include "cquaternion.h"
#include <memory>
#include <random>
#include <chrono>					// c++11 time 库

using uint = unsigned int;
using uchar = unsigned char;
using ll = long long;

using UPTR_char = std::unique_ptr<char>;
using SPTR_char = std::shared_ptr<char>;
using WPTR_char = std::weak_ptr<char>;
using UPTR_uchar = std::unique_ptr<uchar>;
using SPTR_uchar = std::shared_ptr<uchar>;
using WPTR_uchar = std::weak_ptr<uchar>;

using hour_type = std::chrono::duration<ll, std::ratio<60 * 60, 1>>;
using minute_type = std::chrono::duration<ll, std::ratio<60, 1>>;
using second_type = std::chrono::duration<ll, std::ratio<1, 1>>;
using msecnd_type = std::chrono::duration<ll, std::ratio<1, 1000>>;

using system_time_point = std::chrono::system_clock::time_point;
using highrs_time_point = std::chrono::high_resolution_clock::time_point;

class CMath {
public:
	~CMath() {}
	static const float pi;
	static const float eps;

	template<class T> static T clamp(T x, T a, T b) { if (x < a) x = a; if (x > b) x = b; return x; }

	template<class T> static T pow(T a, T p);

	static float rad_to_deg(float rad);
	static float deg_to_rad(float deg);

	static float interp_linear(float v, float min, float max);		// 线性插值 v ~ [0.0 ,, 1.0]

    static int fcmp(float a, float b);								// -1 -- < ; 0 -- = ; 1 -- >

	static std::default_random_engine dre;
	static void set_random_seed(int seed) { dre.seed(seed); }
	template<class T> static T random(T min, T max);				// random in [min, max]
	template<> static float random(float min, float max);			// 显示具体化

private:
	CMath();

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

template<class T>
T CMath::random(T min, T max) {
	std::uniform_int_distribution<T> u(min, max);
	return u(dre);
}

template<> 
float CMath::random(float min, float max) {
	std::uniform_real_distribution<float> u(min, max);
	return u(dre);
}

