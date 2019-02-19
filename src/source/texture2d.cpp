#include "texture2d.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

IMPLEMENT_CLASS(Texture2D)

Texture2D::Texture2D() : name(""), width(0), heigh(0), internal_format(GL_RGBA), image_format(GL_BGRA),
	wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR) { 
	auto core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	core->glGenTextures(1, &id);
}

void Texture2D::init(uint w, uint h, SPTR_uchar data) {
	auto core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	width = w; heigh = h;

	// 创建纹理
	core->glBindTexture(GL_TEXTURE_2D, id);
	core->glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, heigh, 0, image_format, GL_UNSIGNED_BYTE, data.get());
	// 设置纹理属性
	if (filter_min == GL_LINEAR_MIPMAP_LINEAR) { core->glGenerateMipmap(GL_TEXTURE_2D); }
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

void Texture2D::bind(uint tid) const {
	auto core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
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
