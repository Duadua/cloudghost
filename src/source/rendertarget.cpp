#define GLEW_STATIC
#include <GL/glew.h>

#include "texture2d.h"
#include "rendertarget.h"

IMPLEMENT_CLASS(RenderBuffer)

RenderBuffer::RenderBuffer() :attach_type(GL_DEPTH_STENCIL_ATTACHMENT), b_multisample(false) { }

void RenderBuffer::init(uint w, uint h, uint at_type, uint fmt, bool b_m) {
	attach_type = at_type;
	format = fmt;
	width = w;
	heigh = h;
	b_multisample = b_m;

	glGenRenderbuffers(1, &id);

	glBindRenderbuffer(GL_RENDERBUFFER, id); {

		if (!b_multisample) {
			glRenderbufferStorage(GL_RENDERBUFFER, format, 
				static_cast<GLsizei>(width), static_cast<GLsizei>(heigh));
		}
		else {
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, format, 
				static_cast<GLsizei>(width), static_cast<GLsizei>(heigh));
		}

	} glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

SPTR_RenderBuffer RenderBuffer::use() {
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	return shared_from_this();
}
SPTR_RenderBuffer RenderBuffer::un_use() {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return shared_from_this();
}

// =====================================================================================

IMPLEMENT_CLASS(RenderTarget)

RenderTarget::RenderTarget() : target(GL_FRAMEBUFFER) {
}

bool RenderTarget::init(uint tg) {
	target = tg;

	glGenFramebuffers(1, &id);

	glBindFramebuffer(target, id); {

		for (auto tbo : attach_textures) {
			glFramebufferTexture2D(target, tbo.attach_type, tbo.texture->get_type(), tbo.texture->get_id(), 0);
		} // bind texture attachment
		for (auto rbo : attach_renderbuffers) {
			glFramebufferRenderbuffer(target, rbo->get_attach_type(), GL_RENDERBUFFER, rbo->get_id());
		} // bind renderbuffer attachment

	} glBindFramebuffer(target, 0);

	if (glCheckFramebufferStatus(target) == GL_FRAMEBUFFER_COMPLETE) { return true; }
	else { return false; }
	
}

bool RenderTarget::init_normal(uint w, uint h) {
	add_attach_texture(GL_COLOR_ATTACHMENT0, w, h);
	add_attach_renderbuffer(w, h);

	return init();
}
bool RenderTarget::init_simple(uint w, uint h) {
	add_attach_texture(GL_COLOR_ATTACHMENT0, w, h);
	return init();
}
bool RenderTarget::init_normal_multisample(uint w, uint h) {
	add_attach_texture(GL_COLOR_ATTACHMENT0, w, h, GL_TEXTURE_2D_MULTISAMPLE);
	add_attach_renderbuffer(w, h, true);

	return init();
}

SPTR_RenderTarget RenderTarget::add_attach_texture(uint at_type, uint width, uint heigh, uint type, uint internal_format, uint format, uint data_type) {
	auto t_texture = CREATE_CLASS(Texture2D);
	t_texture->gen(width, heigh, internal_format, format, data_type, type);
	if (t_texture != nullptr) {
		TextureBuffer t_tb;
		t_tb.attach_type = at_type;
		t_tb.texture = t_texture;
		attach_textures.push_back(t_tb);
	}
	return shared_from_this();
}
SPTR_RenderTarget RenderTarget::add_attach_renderbuffer(uint w, uint h, bool b_multisample, uint at_type, uint fmt) {
	auto t_rb = CREATE_CLASS(RenderBuffer);
	t_rb->init(w, h, at_type, fmt, b_multisample);
	if (t_rb != nullptr) { attach_renderbuffers.push_back(t_rb); }
	return shared_from_this();
}

SPTR_RenderTarget RenderTarget::use() {
	glBindFramebuffer(target, id);
	return shared_from_this();
}
SPTR_RenderTarget RenderTarget::un_use() {
	glBindFramebuffer(target, 0);
	return shared_from_this();
}

SPTR_RenderTarget RenderTarget::use_r() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
	return shared_from_this();
}
SPTR_RenderTarget RenderTarget::un_use_r() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	return shared_from_this();
}
SPTR_RenderTarget RenderTarget::use_w() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
	return shared_from_this();
}
SPTR_RenderTarget RenderTarget::un_use_w() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return shared_from_this();
}

