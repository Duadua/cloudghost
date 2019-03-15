#define GLEW_STATIC
#include <GL/glew.h>

#include "rendertarget.h"

IMPLEMENT_CLASS(RenderBuffer)

RenderBuffer::RenderBuffer() :attach_type(GL_DEPTH_STENCIL_ATTACHMENT) {
}

void RenderBuffer::init(uint w, uint h, uint at_type, uint fmt) {
	attach_type = at_type;
	format = fmt;
	width = w;
	heigh = h;

	glGenRenderbuffers(1, &id);

	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, heigh);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

	glBindFramebuffer(target, id);

	for (auto tbo : attach_textures) {
		glFramebufferTexture2D(target, tbo.attach_type, GL_TEXTURE_2D, tbo.texture->get_id(), 0);
	} // bind texture attachment
	for (auto rbo : attach_renderbuffers) {
		glFramebufferRenderbuffer(target, rbo->get_attach_type(), GL_RENDERBUFFER, rbo->get_id());
	} // bind renderbuffer attachment

	glBindFramebuffer(target, 0);

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

SPTR_RenderTarget RenderTarget::add_attach_texture(uint at_type, uint width, uint heigh, uint internal_format, uint format, uint data_type) {
	auto t_texture = CREATE_CLASS(Texture2D);
	t_texture->gen(width, heigh, internal_format, format, data_type);
	if (t_texture != nullptr) {
		TextureBuffer t_tb;
		t_tb.attach_type = at_type;
		t_tb.texture = t_texture;
		attach_textures.push_back(t_tb);
	}
	return shared_from_this();
}
SPTR_RenderTarget RenderTarget::add_attach_renderbuffer(uint w, uint h, uint at_type, uint fmt) {
	auto t_rb = CREATE_CLASS(RenderBuffer);
	t_rb->init(w, h, at_type, fmt);
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

