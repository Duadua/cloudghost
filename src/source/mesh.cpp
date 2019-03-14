#include "mesh.h"
#include "assetmanager.h"
#include <QDebug>

IMPLEMENT_CLASS(Mesh)

Mesh::Mesh() : use_default_mt(true) {}
Mesh::~Mesh() {}

void Mesh::draw(const std::string& shader) {
	for (auto rd : render_datas) { 
		std::string t_name = rd.material;
		if (t_name.compare("") == 0) { t_name = rd.rd->get_material_name(); }
		auto t_material = AssetManager::get_material(t_name);
		if (t_material == nullptr && use_default_mt) { t_material = AssetManager::get_material(Material::default_material_name); }
		if (t_material != nullptr) { t_material->use(shader); }
		rd.rd->draw(); 
		Material::un_use(shader);
	}
}

void Mesh::add_render_data(SPTR_RenderData rd) { 
	if (rd != nullptr) render_datas.push_back(RenderDataInstance(rd)); 
}