#pragma once

#include "cmath.h"
#include "reflex.h"

#define GL_TEXTURE_CUBE_MAP 0x8513

struct TextureData {
	uint type;						// GL_TEXTURE_CUBE_MAP
	std::string name;

	uint width;
	uint heigh;

	uint internal_format;
	uint image_format;
	uint data_type;

	uint wrap_s;
	uint wrap_t;
	uint wrap_r;		// 第三个维度
	uint filter_min;
	uint filter_max;

	TextureData();

};

class Texture3D : public CObject {
	DECLARE_CLASS(Texture3D)
public:

	Texture3D();
	~Texture3D() {}

	void init(std::vector<TextureData> texure_datas, std::vector<SPTR_uchar> dts);
	void gen(uint w, uint h, uint in_fmt, uint fmt, uint d_type, uint t = GL_TEXTURE_CUBE_MAP);

	void use();
	void un_use();

	void bind(uint tid = 0) const;
	static void un_bind(uint tid = 0);

	GET(uint, id)
	GET(uint, type)
	GET_SET(std::string, name)

	GET_SET(std::vector<TextureData>, datas)

private:
	std::string name;

	uint type;					// GL_TEXTURE_CUBE_MAP
	uint id;

	std::vector<TextureData> datas;

};
DECLARE_AUTO_PTR(Texture3D)

