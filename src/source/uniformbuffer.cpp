#define GLEW_STATIC
#include <GL/glew.h>

#include "uniformbuffer.h"

IMPLEMENT_CLASS(UniformBuffer)

UniformBuffer::UniformBuffer() : length(0), index(0), offset(0) { }

bool UniformBuffer::init(uint len, uint idx, uint ost) {
	length = len;
	index = idx;
	offset = ost;

	glGenBuffers(1, &ubo);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, length, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, offset, ubo, offset, length);

	return true;
}

SPTR_UniformBuffer UniformBuffer::fill_data(uint d_offset, uint d_size, const void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	
	glBufferSubData(GL_UNIFORM_BUFFER, d_offset, d_size, data);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return shared_from_this();
}

SPTR_UniformBuffer UniformBuffer::use() {
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	return shared_from_this();
}
SPTR_UniformBuffer UniformBuffer::un_use() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return shared_from_this();
}

