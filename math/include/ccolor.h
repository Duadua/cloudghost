#pragma once

using crgb = unsigned int;

class CColor {
public:
	CColor(crgb v = 0xffffffff) : argb(v) {}	// default is white
	CColor(int r, int g, int b, int a = 0xffu);
	~CColor() {}

	// set rgba -- [0 ,, 255]
	void reset(int r, int g, int b, int a = 0xffu);
	void set_r(int r);
	void set_g(int r);
	void set_b(int r);
	void set_a(int r);

	// [0 ,, 255]
	inline int r() const { return ((argb >> 16) & 0xff); }	// red
	inline int g() const { return ((argb >>  8) & 0xff); }	// green
	inline int b() const { return ((argb >>  0) & 0xff); }	// blue
	inline int a() const { return ((argb >> 24) & 0xff); }	// alpha

	// [0.0 ,, 1.0]
	inline float r_f() const { return r() / 255.0f; }		
	inline float g_f() const { return g() / 255.0f; }			
	inline float b_f() const { return b() / 255.0f; }			
	inline float a_f() const { return a() / 255.0f; }			

	// gray
	inline int		gray() const { return (r() * 11 + g() * 16 + b() * 5) / 32; }
	inline float	gray_f() const { return gray() / 255.0f; }
	inline bool		is_gray() const { return r() == g() && r() == b(); }

	CColor rgb() const { return CColor(r(), g(), b()); }
	CColor bgr() const { return CColor(b(), g(), r()); }
	CColor bgra() const { return CColor(b(), g(), r(), a()); }

	static const crgb mask;		// mask rgb value

private:
	crgb argb;
	

};
