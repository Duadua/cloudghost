#include "mesh.h"
#include <QDebug>

IMPLEMENT_CLASS(Mesh)

Mesh::Mesh() {}
Mesh::~Mesh() {}

void Mesh::draw() {
	for (auto rd : render_datas) { rd->draw(); }
}

void Mesh::add_render_data(SPTR_RenderData rd) { if (rd != nullptr) render_datas.push_back(rd); }