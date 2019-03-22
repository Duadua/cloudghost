#pragma once

#include "reflex.h"
#include "tools/meshloader.h"
const int bone_num_per_vertex = 4;

struct CVertex {
	CVector3D position;
	CVector3D normal;
	CVector2D tex_coord;
	CVertex(CVector3D pos = CVector3D(), CVector3D norm = CVector3D(), CVector2D coord = CVector2D())
		: position(pos), normal(norm), tex_coord(coord) {}

};

struct CVertexBone {
	uint ids[bone_num_per_vertex];
	float weights[bone_num_per_vertex];
	CVertexBone();
	CVertexBone& add(uint id, float weigtht);
};

class RenderData : public CObject{
	DECLARE_CLASS(RenderData)
public:
	RenderData();
	virtual ~RenderData();

	void init(const std::vector<CVertex>& v, const std::vector<uint>& i);
	void init_with_bone(const std::vector<CVertex>& v, const std::vector<uint>& i, const std::vector<CVertexBone>& vb);
	void draw();

	GET_SET(std::string, material_name)
	GET_SET(std::vector<CVertex>, vertices)
	GET_SET(std::vector<uint>, indices)

private:
	uint vao;
	uint vbo;
	uint ebo;
	
	std::vector<CVertex> vertices;
	std::vector<uint> indices;		// 索引

	std::string material_name;
	
	// bone info
	bool has_bone;
	uint vbo_bone;
	std::vector<CVertexBone> bones;

};
DECLARE_AUTO_PTR(RenderData)

// ===================================
// 用于对获得 rd 再加工 -- 随机凹凸, 平滑等

class RenderDataReprocess {
public:
	~RenderDataReprocess() {}
	
	void bump(std::vector<CVertex>& res_v, const std::vector<CVertex>& v, int step = 5, float out_scope = 0.1f, float in_scope = 0.01f);

	void smooth(std::vector<CVertex>& res_v, const std::vector<CVertex>& v, const std::vector<uint>& v_i);
	

	static RenderDataReprocess* get_instance();



private:
	RenderDataReprocess() {}

	static RenderDataReprocess* instance;

};
RenderDataReprocess& renderdata_reprocess();

