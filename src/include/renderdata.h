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
	
	void bump(	const std::vector<MVertex>& res_v, std::vector<uint>& res_i, 
				const std::vector<MVertex>& v, const std::vector<uint>& i);

	void smooth(const std::vector<MVertex>& res_v, std::vector<uint>& res_i, 
				const std::vector<MVertex>& v, const std::vector<uint>& i);
	

	static RenderDataReprocess* get_instance();



private:
	RenderDataReprocess() {}

	static RenderDataReprocess* instance;

};
RenderDataReprocess& renderdata_reprocess();

