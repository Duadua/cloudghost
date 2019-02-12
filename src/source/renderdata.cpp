#include "renderdata.h"

IMPLEMENT_CLASS(RenderData)

RenderData::RenderData() : vao(0), vbo(0), ebo(0) {
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}
RenderData::~RenderData() {
	//if (vao != 0) core->glDeleteVertexArrays(1, &vao);
	//if (vbo != 0) core->glDeleteBuffers(1, &vbo);
	//if (ebo != 0) core->glDeleteBuffers(1, &ebo);

}

void RenderData::init(const std::vector<MVertex>& v, const std::vector<uint>& i) {
	vertices = v;
	indices = i;

	// 初始化 vbo
	core->glGenBuffers(1, &vbo);
	core->glBindBuffer(GL_ARRAY_BUFFER,vbo);
	core->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MVertex), &vertices[0], GL_STATIC_DRAW);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 初始化 ebo
	if (indices.size() > 0) {
		core->glGenBuffers(1, &ebo);
		core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);
		core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// 初始化 vao
	core->glGenVertexArrays(1, &vao);
	core->glBindVertexArray(vao);
	core->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (indices.size() > 0) core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void*)0);
	core->glEnableVertexAttribArray(0);
	core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void*)offsetof(MVertex, normal));
	core->glEnableVertexAttribArray(1);
	core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MVertex), (void*)offsetof(MVertex, tex_coord));
	core->glEnableVertexAttribArray(2);

	core->glBindVertexArray(0);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (indices.size() > 0) core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderData::draw() {
	core->glBindVertexArray(vao);
	if(indices.size() <= 0) core->glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	else core->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	core->glBindVertexArray(0);
}
