#pragma once

#include "cmath.h"
#include "reflex.h"

#define GL_TEXTURE_2D 0x0DE1

class Texture2D : public CObject {
	DECLARE_CLASS(Texture2D)
public:

	Texture2D();
	~Texture2D() {}

	void init(uint w, uint h, SPTR_uchar data, uint t = GL_TEXTURE_2D, uint msaa_num = 4);
	void gen(uint w, uint h, uint in_fmt, uint fmt, uint d_type, uint t = GL_TEXTURE_2D, uint msaa_num = 4);

	void bind(uint tid = 0) const;
	static void un_bind(uint tid = 0);

    GET(uint, id)
	GET(uint, type)
    GET(uint, width)
    GET(uint, heigh)


    GET_SET(std::string, name)

    GET_SET(uint, internal_format)
    GET_SET(uint, image_format)
    GET_SET(uint, data_type)

    GET_SET(uint, wrap_s)
    GET_SET(uint, wrap_t)
    GET_SET(uint, filter_min)
    GET_SET(uint, filter_max)

private:
	std::string name;
	
	uint type;					// GL_TEXTURE_2D or GL_TEXTURE_2D_MULTISAMPLE (用来支持多重采样)
	uint id;
	uint width;
	uint heigh;

	uint internal_format;
	uint image_format;
	uint data_type;

	uint wrap_s;
	uint wrap_t;
	uint filter_min;
	uint filter_max;
	
};
DECLARE_AUTO_PTR(Texture2D)


