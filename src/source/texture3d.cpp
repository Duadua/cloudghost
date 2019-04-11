#define GLEW_STATIC
#include <GL/glew.h>

#include "texture3d.h"

TextureData::TextureData() : type(GL_TEXTURE_CUBE_MAP_POSITIVE_X), name(""), width(0), heigh(0),
		internal_format(GL_RGBA), image_format(GL_BGRA), data_type(GL_UNSIGNED_BYTE),
		wrap_s(GL_CLAMP_TO_EDGE), wrap_t(GL_CLAMP_TO_EDGE), wrap_r(GL_CLAMP_TO_EDGE), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR) {}

// ==========================================================================================

IMPLEMENT_CLASS(Texture3D)

Texture3D::Texture3D() : type(GL_TEXTURE_CUBE_MAP) { }

void Texture3D::init(std::vector<TextureData> texure_datas, std::vector<SPTR_uchar> dts) {
	datas.assign(texure_datas.begin(), texure_datas.end());
	glGenTextures(1, &id);
	glBindTexture(type, id); {
		for (uint i = 0; i < datas.size(); ++i) {
			glTexImage2D(datas[i].type, 0, datas[i].internal_format, datas[i].width, datas[i].heigh, 
				0, datas[i].image_format, datas[i].data_type, dts[i].get());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	} glBindTexture(type, 0);


}

void Texture3D::gen(uint w, uint h, uint in_fmt, uint fmt, uint d_type, uint t) {
	static const std::string suf_name[6] = { "rt", "lf", "up", "dn", "bk", "ft" };	// 右左上下后前 -- 012345
	type = t;
	glGenTextures(1, &id);
	glBindTexture(type, id); {
		for (uint i = 0; i < 6; ++i) {
			TextureData t_td;
			t_td.type = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			t_td.name = name + suf_name[i];
			t_td.width = w;
			t_td.heigh = h;
			t_td.internal_format = in_fmt;
			t_td.image_format = fmt;
			t_td.data_type = d_type;
			datas.push_back(t_td);

			glTexImage2D(t_td.type, 0, t_td.internal_format, t_td.width, t_td.heigh,
				0, t_td.image_format, t_td.data_type, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	} glBindTexture(type, 0);

}

void Texture3D::use() { glBindTexture(type, id); }
void Texture3D::un_use() { glBindTexture(type, 0); }

void Texture3D::bind(uint tid) const { 
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(type, id);
	glActiveTexture(GL_TEXTURE0);
}

void Texture3D::un_bind(uint tid) {
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(GL_TEXTURE0);
}



