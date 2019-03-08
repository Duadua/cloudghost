#pragma once

// RT -- fbo

#include "cmath.h"

class RenderTarget {
public:
	RenderTarget() {}
	~RenderTarget() {}

	void add_attach_texture();
	void add_attach_renderbuffer();

	bool init();

	void use();
	void un_use();

private:
	uint id;


};

