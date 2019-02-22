#include "ccolor.h"

const crgb CColor::mask = 0xffffffff;

CColor::CColor(int r, int g, int b, int a) {
	argb = ( (a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | ((b & 0xffu) << 0);
}

void CColor::reset(int r, int g, int b, int a) {
	argb = ( (a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | ((b & 0xffu) << 0);
}

void CColor::set_r(int r) {
	crgb t = (0xffu << 16);
	argb |= t;								// r位置1
	argb &= ((r << 16) & t) ^ (t ^ mask);	// 新r入位
}
void CColor::set_g(int g) {
	crgb t = (0xffu << 8);
	argb |= t;								
	argb &= ((g << 8) & t) ^ (t ^ mask);
}
void CColor::set_b(int b) {
	crgb t = (0xffu << 0);
	argb |= t;						
	argb &= ((b << 0) & t) ^ (t ^ mask);

}
void CColor::set_a(int a) {
	crgb t = (0xffu << 24);
	argb |= t;						
	argb &= ((a << 24) & t) ^ (t ^ mask);
}
