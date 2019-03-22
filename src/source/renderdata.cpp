#define GLEW_STATIC
#include <GL/glew.h>

#include "renderdata.h"

#include <GLFW/glfw3.h>

#include <set>

CVertexBone::CVertexBone() { for (int i = 0; i < bone_num_per_vertex; ++i) { ids[i] = 0; weights[i] = 0.0f; } }

CVertexBone& CVertexBone::add(uint id, float weight) {
	for (int i = 0; i < bone_num_per_vertex; ++i) {
		if (CMath::fcmp(weights[i], 0.0f) == 0) { ids[i] = id; weights[i] = weight; break; }
	}
	return (*this);
}

// ======================================================================

IMPLEMENT_CLASS(RenderData)

RenderData::RenderData() : vao(0), vbo(0), ebo(0), vbo_bone(0), has_bone(false) {
	material_name = "default";		// default material
}
RenderData::~RenderData() {
	//if (vao != 0) core->glDeleteVertexArrays(1, &vao);
	//if (vbo != 0) core->glDeleteBuffers(1, &vbo);
	//if (ebo != 0) core->glDeleteBuffers(1, &ebo);

}

void RenderData::init(const std::vector<CVertex>& v, const std::vector<uint>& i) {
	vertices = v;
	indices = i;

	// 初始化 vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(CVertex)), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 初始化 ebo
	if (indices.size() > 0) {
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint)), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// 初始化 vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (indices.size() > 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), reinterpret_cast<void*>(offsetof(CVertex, normal)));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), reinterpret_cast<void*>(offsetof(CVertex, tex_coord)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (indices.size() > 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void RenderData::init_with_bone(const std::vector<CVertex>& v, const std::vector<uint>& i, const std::vector<CVertexBone>& vb) {
	vertices = v;
	indices = i;
	if (vb.size() > 0) { bones = vb; has_bone = true; }

	// 初始化 vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(CVertex)), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (has_bone) {
		glGenBuffers(1, &vbo_bone);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_bone);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bones.size() * sizeof(CVertexBone)), &bones[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// 初始化 ebo
	if (indices.size() > 0) {
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint)), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// 初始化 vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (indices.size() > 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), reinterpret_cast<void*>(offsetof(CVertex, normal)));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CVertex), reinterpret_cast<void*>(offsetof(CVertex, tex_coord)));
	glEnableVertexAttribArray(2);

	if (has_bone) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo_bone);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(CVertexBone), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(CVertexBone), reinterpret_cast<void*>(offsetof(CVertexBone, weights)));
		glEnableVertexAttribArray(6);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (indices.size() > 0) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void RenderData::draw() {
	glBindVertexArray(vao);
    if(indices.size() <= 0) glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    else glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

// ===========================================================================

RenderDataReprocess* RenderDataReprocess::instance = nullptr;
RenderDataReprocess* RenderDataReprocess::get_instance() {
	if (instance == nullptr) { instance = new RenderDataReprocess(); }
	return instance;
}
RenderDataReprocess& renderdata_reprocess() { return (*RenderDataReprocess::get_instance()); }

void RenderDataReprocess::bump(std::vector<CVertex>& res_v, const std::vector<CVertex>& v, int step, float out_scope, float in_scope) {
	step = CMath::clamp(step, 1, 1000); step *= 3; 
	
	res_v.clear(); res_v.assign(v.begin(), v.end()); int i = 0;
	for (auto& mv : res_v) { if (i%step == 0) mv.position += mv.normal * CMath::random(-in_scope, out_scope); ++i; }
}

void RenderDataReprocess::smooth(std::vector<CVertex>& res_v, const std::vector<CVertex>& v, const std::vector<uint>& v_i) {

	std::vector<std::set<uint> > t_adj(v.size());

	// gen adj
	{
		uint len = static_cast<uint>(v_i.size());
		for (uint i = 0; i < len; i += 3) {
			uint ai = v_i[i + 0]; uint bi = v_i[i + 1]; uint ci = v_i[i + 2];

			t_adj[ai].insert(bi); t_adj[ai].insert(ci);
			t_adj[bi].insert(ai); t_adj[bi].insert(ci);
			t_adj[ci].insert(ai); t_adj[ci].insert(bi);
		}

	}

	// gen new vertex position
	{
		uint len = static_cast<uint>(t_adj.size());
		for (uint i = 0; i < len; ++i) {
			CVertex t_v;
			for (auto tt : t_adj[i]) { t_v.position += 0.8f * v[tt].position; }
			t_v.position = t_v.position / (static_cast<float>(t_adj[i].size()));
			t_v.position += 0.2f * v[i].position;
			res_v.push_back(t_v);
		}

	}

	// recac normal (and texcoord)
	{
		for (auto& t_v : res_v) {
			t_v.normal = t_v.position.get_normalize();
		}
	}

}



