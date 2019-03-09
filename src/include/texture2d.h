#pragma once

#include "cmath.h"
#include "reflex.h"
#include <QOpenGLFunctions_3_3_Core>

class Texture2D : public CObject {
	DECLARE_CLASS(Texture2D)
public:

	Texture2D();
	~Texture2D() {}

	void init(uint w, uint h, SPTR_uchar data);
	void gen(uint w, uint h, uint in_fmt, uint fmt, uint d_type);

	void bind(uint tid = 0) const;
	static void un_bind(uint tid = 0);

	GET(uint, id);
	GET(uint, width);
	GET(uint, heigh);

	GET_SET(std::string, name);

	GET_SET(uint, internal_format);
	GET_SET(uint, image_format);
	GET_SET(uint, data_type);

	GET_SET(uint, wrap_s);
	GET_SET(uint, wrap_t);
	GET_SET(uint, filter_min);
	GET_SET(uint, filter_max);


private:
	std::string name;

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
	
private:
	QOpenGLFunctions_3_3_Core * core;

};
DECLARE_AUTO_PTR(Texture2D)
