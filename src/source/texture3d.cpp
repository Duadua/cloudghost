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
void Texture3D::bind(uint tid) const { 
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(type, id);
	glActiveTexture(GL_TEXTURE0);
}

void Texture3D::un_bind(uint tid) {
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}



