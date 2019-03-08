#pragma once

#include "cvector3d.h"
#include "cvector4d.h"

using crgb = unsigned int;

class CColor {
public:
	CColor(crgb v = 0xffffffff) : argb(v) {}	// default is white
	CColor(int r, int g, int b, int a = 0xffu);
	~CColor() {} 

	crgb get_uint() const { return argb; }

	// set rgba -- [0 ,, 255]
	void reset(int r, int g, int b, int a = 0xffu);
	void set_r(int r);
	void set_g(int r);
	void set_b(int r);
	void set_a(int r);

	// [0 ,, 255]
	int r() const { return ((argb >> 16) & 0xff); }	// red
	int g() const { return ((argb >>  8) & 0xff); }	// green
	int b() const { return ((argb >>  0) & 0xff); }	// blue
	int a() const { return ((argb >> 24) & 0xff); }	// alpha

	// [0.0 ,, 1.0]
	float r_f() const { return r() / 255.0f; }		
	float g_f() const { return g() / 255.0f; }			
	float b_f() const { return b() / 255.0f; }			
	float a_f() const { return a() / 255.0f; }			

	CVector3D rgb_f() const { return CVector3D(r_f(), g_f(), b_f()); }
	CVector4D rgba_f() const { return CVector4D(r_f(), g_f(), b_f(), a_f()); }

	// gray
	int		gray() const { return (r() * 11 + g() * 16 + b() * 5) / 32; }
	float	gray_f() const { return gray() / 255.0f; }
	bool	is_gray() const { return r() == g() && r() == b(); }

	CColor rgb() const { return CColor(r(), g(), b()); }
	CColor bgr() const { return CColor(b(), g(), r()); }
	CColor bgra() const { return CColor(b(), g(), r(), a()); }

	static const crgb mask;		// mask rgb value
	
private:
	crgb argb;
	

};



