#pragma once

#include "reflex.h"
#include "tools/meshloader.h"

class RenderData : public CObject{
	DECLARE_CLASS(RenderData)
public:
	RenderData();
	virtual ~RenderData();

	void init(const std::vector<MVertex>& v, const std::vector<uint>& i);
	void draw();

	GET_SET(std::string, material_name)
	GET_SET(std::vector<MVertex>, vertices)
	GET_SET(std::vector<uint>, indices)

private:
	uint vao;
	uint vbo;
	uint ebo;
	
	std::vector<MVertex> vertices;
	std::vector<uint> indices;		// 索引

	std::string material_name;

};
DECLARE_AUTO_PTR(RenderData)

// ===================================
// 用于对获得 rd 再加工 -- 随机凹凸, 平滑等

class RenderDataReprocess {
public:
	~RenderDataReprocess() {}
	
	void bump(std::vector<MVertex>& res_v, const std::vector<MVertex>& v, int step = 5, float out_scope = 0.1f, float in_scope = 0.01f);

	void smooth(std::vector<MVertex>& res_v, const std::vector<MVertex>& v, const std::vector<uint>& v_i);
	

	static RenderDataReprocess* get_instance();



private:
	RenderDataReprocess() {}

	static RenderDataReprocess* instance;

};
RenderDataReprocess& renderdata_reprocess();

