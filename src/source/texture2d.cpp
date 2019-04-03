#define GLEW_STATIC
#include <GL/glew.h>

#include "cdebuger.h"
#include "texture2d.h"

IMPLEMENT_CLASS(Texture2D)

Texture2D::Texture2D() : type(GL_TEXTURE_2D), name(""), width(0), heigh(0), 
	internal_format(GL_RGBA), image_format(GL_BGRA), data_type(GL_UNSIGNED_BYTE),
	wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR_MIPMAP_LINEAR), filter_max(GL_LINEAR) { 

	if (glewExperimental == false) {
		glewExperimental = true;
        GLenum flag = glewInit();
		if (flag != GLEW_OK) {
			c_debuger() << "[error][glew]init fail";
            c_debuger() << reinterpret_cast<const char*>(glewGetErrorString(flag));
		}
	}

}

void Texture2D::init(uint w, uint h, SPTR_uchar data, uint t, uint msaa_num) {
	width = w; heigh = h; type = t;
	glGenTextures(1, &id);

	glBindTexture(type, id); {
		if (type == GL_TEXTURE_2D) {
			// 创建纹理
			glTexImage2D(type, 0, static_cast<GLint>(internal_format), static_cast<GLsizei>(width), static_cast<GLsizei>(heigh),
						 0, image_format, data_type, data.get());
			// 设置纹理属性
			if (filter_min == GL_LINEAR_MIPMAP_LINEAR) { glGenerateMipmap(type); }
			if (internal_format == GL_RGBA || internal_format == GL_BGRA) { wrap_s = GL_CLAMP_TO_EDGE; wrap_t = GL_CLAMP_TO_EDGE; }

			glTexParameteri(type, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_s));
			glTexParameteri(type, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_t));
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter_min));
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter_max));

			/*texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
			texture->setData(t_img, QOpenGLTexture::GenerateMipMaps);
			*/
		} 
		else if(type == GL_TEXTURE_2D_MULTISAMPLE) {
			glTexImage2DMultisample(type, msaa_num, static_cast<GLint>(internal_format), static_cast<GLsizei>(width), static_cast<GLsizei>(heigh), GL_TRUE);
		}

	} glBindTexture(type, 0);
}

void Texture2D::gen(uint w, uint h, uint in_fmt, uint fmt, uint d_type, uint t, uint msaa_num) {
	width = w; heigh = h;
	internal_format = in_fmt;
	image_format = fmt;
	data_type = d_type;
	filter_min = GL_LINEAR;

	type = t;
	glGenTextures(1, &id);

	glBindTexture(type, id); {
		if (type == GL_TEXTURE_2D) {
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(internal_format), static_cast<GLsizei>(width), static_cast<GLsizei>(heigh),
						 0, image_format, data_type, nullptr);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter_min));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter_max));
		}
		else if(type == GL_TEXTURE_2D_MULTISAMPLE) {
			glTexImage2DMultisample(type, msaa_num, static_cast<GLint>(internal_format), static_cast<GLsizei>(width), static_cast<GLsizei>(heigh), GL_TRUE);
		}

	} glBindTexture(type, 0);
}

void Texture2D::bind(uint tid) const {
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(type, id);
	glActiveTexture(GL_TEXTURE0);
	//texture->bind(tid);
}

void Texture2D::un_bind(uint tid) {
	glActiveTexture(GL_TEXTURE0 + tid);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

