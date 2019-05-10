#pragma once

// RT -- fbo

#include "cmath.h"
#include "reflex.h"

PRE_DECLARE_CLASS(Texture2D)
USING_SPTR(Texture2D)
PRE_DECLARE_CLASS(Texture3D)
USING_SPTR(Texture3D)

#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_DEPTH24_STENCIL8			0x88F0
#define GL_RGB						0x1907
#define GL_UNSIGNED_BYTE			0x1401
#define GL_FRAMEBUFFER				0x8D40
#define GL_TEXTURE_2D				0x0DE1
#define GL_TEXTURE_CUBE_MAP			0x8513

struct TextureBuffer {
	SPTR_Texture2D texture;
	uint attach_type;					// 纹理附件的类型 -- 颜色附件 or 深度附件 or 模板附件
};

struct TextureBuffer3D {
	SPTR_Texture3D texture;
	uint attach_type;
};

DECLARE_AUTO_PTR(RenderBuffer)
class RenderBuffer : public CObject, public std::enable_shared_from_this<RenderBuffer>{
	DECLARE_CLASS(RenderBuffer)
public:
	RenderBuffer();
	~RenderBuffer() {}
	
	void init(uint w, uint h, uint at_type = GL_DEPTH_STENCIL_ATTACHMENT, uint fmt = GL_DEPTH24_STENCIL8, bool b_m = false, uint msaa_number = 4);
	void resize(uint w, uint h);

	SPTR_RenderBuffer use();
	SPTR_RenderBuffer un_use();

    GET(uint, id)
    GET(uint, attach_type)

private:
	uint id;

	uint attach_type;
	uint format;
	uint width;
	uint heigh;
	uint msaa_num;

	bool b_multisample;				// 是否为多重采样缓冲附件

};

// =====================================================================================

DECLARE_AUTO_PTR(RenderTarget)
class RenderTarget : public CObject, public std::enable_shared_from_this<RenderTarget> {
	DECLARE_CLASS(RenderTarget)

public:
	RenderTarget();
	~RenderTarget() {}

	SPTR_RenderTarget add_attach_texture(uint at_type, uint width, uint heigh, uint type = GL_TEXTURE_2D, uint internal_format = GL_RGBA, uint format = GL_RGBA, uint data_type = GL_UNSIGNED_BYTE);
	SPTR_RenderTarget add_attach_texture_3d(uint at_type, uint width, uint heigh, uint type = GL_TEXTURE_CUBE_MAP, uint internal_format = GL_RGBA, uint format = GL_RGBA, uint data_type = GL_UNSIGNED_BYTE);
	SPTR_RenderTarget add_attach_renderbuffer(uint w, uint h, bool b_multisample = false, uint at_type = GL_DEPTH_STENCIL_ATTACHMENT, uint fmt = GL_DEPTH24_STENCIL8);

	bool init(uint tg = GL_FRAMEBUFFER);
	bool init_normal(uint w, uint h);						// 一个颜色纹理附件 一个深度模板渲染缓冲附件
	bool init_simple(uint w, uint h);						// 只有一个颜色纹理附件
	bool init_normal_multisample(uint w, uint h);			// 多重采样 fbo

	bool init_normal_f(uint w, uint h);						// 浮点帧缓冲 -- 用于支持 HDR
	bool init_normal_multisample_f(uint w, uint h);		

	SPTR_RenderTarget use();
	SPTR_RenderTarget un_use();

	SPTR_RenderTarget use_r(uint cid = 0);					// cid -- 颜色附件的 id
	SPTR_RenderTarget un_use_r();
	SPTR_RenderTarget use_w(uint cid = 0);
	SPTR_RenderTarget un_use_w();

	SPTR_RenderTarget use_texture_3d(uint tid, uint texture_id, uint level = 0);
	SPTR_RenderTarget use_texture_3d_all(uint tid, uint level = 0);

	// rt 的块传送 -- 可以把 msaa 转为正常纹理
	friend void blit(SPTR_RenderTarget a, SPTR_RenderTarget b, uint w, uint h, uint a_cid = 0, uint b_cid = 0);

    GET(std::vector<TextureBuffer>, attach_textures)
    GET(std::vector<TextureBuffer3D>, attach_texture_3ds)
    GET(std::vector<SPTR_RenderBuffer>, attach_renderbuffers)

	uint get_msaa_num() { return CMath_ins().pow(2u, msaa_level); }
	void set_msaa_level(uint l) { l = CMath_ins().clamp(l, 2u, 4u); msaa_level = l; }

private:
	uint id;
	uint target;			// GL_FRAMEBUFFER GL_READ_FRAMEBUFFER GL_DRAW_FRAMEBUFFER

	std::vector<TextureBuffer> attach_textures;
	std::vector<TextureBuffer3D> attach_texture_3ds;
	std::vector<SPTR_RenderBuffer>  attach_renderbuffers;

	uint msaa_level;		// msaa 等级 [2 ,, 4] -- 2^(msaa_level) 个采样点
};



