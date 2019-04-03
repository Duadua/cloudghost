#pragma once

#include "cvector3d.h"
#include "cvector4d.h"

using crgb = unsigned int;
using uint = unsigned int;

class CColor {
public:
	CColor(crgb v = 0xffffffff) : argb(v) {}	// default is white
    CColor(uint r, uint g, uint b, uint a = 0xffu);
    CColor(const CColor& c) { argb = c.argb; }
	~CColor() {} 

	crgb get_uint() const { return argb; }
	crgb get_rgba() const { return CColor(r(), g(), a(), b()).argb; }

	// set rgba -- [0 ,, 255]
    void reset(uint r, uint g, uint b, uint a = 0xffu);
    void set_r(uint r);
    void set_g(uint r);
    void set_b(uint r);
    void set_a(uint r);

	// [0 ,, 255]
    uint r() const { return ((argb >> 16) & 0xff); }	// red
    uint g() const { return ((argb >>  8) & 0xff); }	// green
    uint b() const { return ((argb >>  0) & 0xff); }	// blue
    uint a() const { return ((argb >> 24) & 0xff); }	// alpha

	// [0.0 ,, 1.0]
	float r_f() const { return r() / 255.0f; }		
	float g_f() const { return g() / 255.0f; }			
	float b_f() const { return b() / 255.0f; }			
	float a_f() const { return a() / 255.0f; }			

	CVector3D rgb_f() const { return CVector3D(r_f(), g_f(), b_f()); }
	CVector4D rgba_f() const { return CVector4D(r_f(), g_f(), b_f(), a_f()); }

	// gray
    uint	gray() const { return (r() * 11 + g() * 16 + b() * 5) / 32; }
	float	gray_f() const { return gray() / 255.0f; }
	bool	is_gray() const { return r() == g() && r() == b(); }

	CColor rgb() const { return CColor(r(), g(), b()); }
	CColor bgr() const { return CColor(b(), g(), r()); }
	CColor bgra() const { return CColor(b(), g(), r(), a()); }

	static const crgb mask;		// mask rgb value
	
private:
	crgb argb;
};



