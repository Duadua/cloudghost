#pragma once

#include "reflex.h"
#include "typehelper.h"

DECLARE_AUTO_PTR(UniformBuffer)
class UniformBuffer : public CObject, public std::enable_shared_from_this<UniformBuffer> {
	DECLARE_CLASS(UniformBuffer)
public:
	UniformBuffer();
	~UniformBuffer() {}

	bool init(uint len, uint idx = 0, uint offset = 0);						

	SPTR_UniformBuffer use();
	SPTR_UniformBuffer un_use();

	SPTR_UniformBuffer fill_data(uint d_offset, uint d_size, const void* data);

private:
	uint ubo;
	uint length;	// length -- 分配空间的大小 -- 单位字节
	uint index;		// 绑定点
	uint offset;	// 绑定点偏移
	
};



