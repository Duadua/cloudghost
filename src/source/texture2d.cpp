#define GLEW_STATIC
#include <GL/glew.h>

#include "loader.h"
#include "texture2d.h"

IMPLEMENT_CLASS(Texture2D)

Texture2D::Texture2D() : name(""), width(0), heigh(0), internal_format(GL_RGBA), image_format(GL_BGRA), data_type(GL_UNSIGNED_BYTE),
	wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR) { 
	if (glewExperimental == false) {
		glewExperimental = true;
		int flag = glewInit();
		if (flag != GLEW_OK) {
			c_debug() << "【error】【glew】init fail";
			c_debug() << (char*)glewGetErrorString(flag);
		}
	}
}

void Texture2D::init(uint w, uint h, SPTR_uchar data) {
	width = w; heigh = h;

	// 创建纹理
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, heigh, 0, image_format, data_type, data.get());
	// 设置纹理属性
	if (filter_min == GL_LINEAR_MIPMAP_LINEAR) { glGenerateMipmap(GL_TEXTURE_2D); }
	if (internal_format == GL_RGBA || internal_format == GL_BGRA) { wrap_s = GL_CLAMP_TO_EDGE; wrap_t = GL_CLAMP_TO_EDGE; }

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
	// 解绑
	glBindTexture(GL_TEXTURE_2D, 0);

	/*texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	texture->setData(t_img, QOpenGLTexture::GenerateMipMaps);
	*/
}

void Texture2D::gen(uint w, uint h, uint in_fmt, uint fmt, uint d_type) {
	width = w; heigh = h;
	internal_format = in_fmt;
	image_format = fmt;
	data_type = d_type;
	filter_min = GL_LINEAR;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, heigh, 0, image_format, data_type, nullptr);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind(uint tid) const {
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(GL_TEXTURE_2D, id);
	glActiveTexture(GL_TEXTURE0);
	//texture->bind(tid);
}

void Texture2D::un_bind(uint tid) {
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}
