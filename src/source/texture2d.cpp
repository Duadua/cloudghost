#include "texture2d.h"
#include <QOpenGLContext>

IMPLEMENT_CLASS(Texture2D)

Texture2D::Texture2D() : name(""), width(0), heigh(0), internal_format(GL_RGBA), image_format(GL_BGRA), data_type(GL_UNSIGNED_BYTE),
	wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR) { 
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

void Texture2D::init(uint w, uint h, SPTR_uchar data) {
	width = w; heigh = h;

	// 创建纹理
	core->glGenTextures(1, &id);
	core->glBindTexture(GL_TEXTURE_2D, id);
	core->glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, heigh, 0, image_format, data_type, data.get());
	// 设置纹理属性
	if (filter_min == GL_LINEAR_MIPMAP_LINEAR) { core->glGenerateMipmap(GL_TEXTURE_2D); }
	if (internal_format == GL_RGBA || internal_format == GL_BGRA) { wrap_s = GL_CLAMP_TO_EDGE; wrap_t = GL_CLAMP_TO_EDGE; }

	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
	// 解绑
	core->glBindTexture(GL_TEXTURE_2D, 0);

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

	core->glGenTextures(1, &id);
	core->glBindTexture(GL_TEXTURE_2D, id);
	core->glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, heigh, 0, image_format, data_type, nullptr);
	
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

	core->glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind(uint tid) const {
	core->glActiveTexture(GL_TEXTURE0 + tid);
	core->glBindTexture(GL_TEXTURE_2D, id);
	core->glActiveTexture(GL_TEXTURE0);
	//texture->bind(tid);
}

void Texture2D::un_bind(uint tid) {
	auto core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	core->glActiveTexture(GL_TEXTURE0 + tid);
	core->glBindTexture(GL_TEXTURE_2D, 0);
	core->glActiveTexture(GL_TEXTURE0);
}
