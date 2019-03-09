#pragma once

// RT -- fbo

#include "cmath.h"
#include "reflex.h"
#include "texture2d.h"
#include <QOpenGLFunctions_3_3_core>

struct TextureBuffer {
	SPTR_Texture2D texture;
	uint attach_type;					// 纹理附件的类型 -- 颜色附件 or 深度附件 or 模板附件
};

DECLARE_AUTO_PTR(RenderBuffer)
class RenderBuffer : public CObject, public std::enable_shared_from_this<RenderBuffer>{
	DECLARE_CLASS(RenderBuffer)
public:
	RenderBuffer();
	~RenderBuffer() {}
	
	void init(uint w, uint h, uint at_type = GL_DEPTH_STENCIL_ATTACHMENT, uint fmt = GL_DEPTH24_STENCIL8);

	SPTR_RenderBuffer use();
	SPTR_RenderBuffer un_use();

	GET(uint, id);
	GET(uint, attach_type);

private:
	uint id;

	uint attach_type;
	uint format;
	uint width;
	uint heigh;

private:
	QOpenGLFunctions_3_3_Core * core;
};

// =====================================================================================

DECLARE_AUTO_PTR(RenderTarget)
class RenderTarget : public CObject, public std::enable_shared_from_this<RenderTarget> {
	DECLARE_CLASS(RenderTarget)

public:
	RenderTarget();
	~RenderTarget() {}

	SPTR_RenderTarget add_attach_texture();
	SPTR_RenderTarget add_attach_renderbuffer();

	bool init(uint tg = GL_FRAMEBUFFER);

	SPTR_RenderTarget use();
	SPTR_RenderTarget un_use();


private:
	uint id;
	uint target;			// GL_FRAMEBUFFER GL_READ_FRAMEBUFFER GL_DRAW_FRAMEBUFFER

	std::vector<TextureBuffer> attach_textures;
	std::vector<SPTR_RenderBuffer>  attach_renderbuffers;

private:
	QOpenGLFunctions_3_3_Core * core;

};



