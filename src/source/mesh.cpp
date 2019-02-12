#include "mesh.h"
#include <QDebug>

IMPLEMENT_CLASS(Mesh)

Mesh::Mesh() {}
Mesh::~Mesh() {}

bool Mesh::load(const std::string& src, SourceType source_type) {
	std::vector<MVertex> t_v;
	std::vector<uint> t_i;
	bool res = false;

	if (source_type == SourceType::BY_FILE) {
		// 获得文件路径后缀
		int idx = src.find_last_of('.');
		std::string suf = src.substr(idx);

		// 通过后缀调用函数
		if (suf.compare(".txt") == 0) { res = MeshLoader::load_mesh_txt(src, t_v, t_i, SourceType::BY_FILE); }
		else if (suf.compare(".obj") == 0) { res = MeshLoader::load_mesh_obj(src, t_v, t_i, SourceType::BY_FILE); }

	}
	else if (source_type == SourceType::BY_STRING) {
		res = MeshLoader::load_mesh_txt(src, t_v, t_i, SourceType::BY_STRING);
	}
	
	if (res) {
		auto t_rd = CREATE_CLASS(RenderData);
		t_rd->init(t_v, t_i);
		render_datas.push_back(t_rd);
		return true;
	}
	return false;

}

void Mesh::draw() {
	for (auto rd : render_datas) { rd->draw(); }
}