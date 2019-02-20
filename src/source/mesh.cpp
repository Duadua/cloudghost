#include "mesh.h"
#include "assetmanager.h"
#include <QDebug>

IMPLEMENT_CLASS(Mesh)

Mesh::Mesh() {}
Mesh::~Mesh() {}

void Mesh::draw(const std::string& shader) {
	for (auto rd : render_datas) { 
		auto t_material = AssetManager::get_material(rd->get_material_name());
		if (t_material == nullptr) { t_material = AssetManager::get_material(Material::default_material_name); }
		if (t_material != nullptr) { t_material->use(shader); }
		rd->draw(); 
		Material::un_use(shader);
	}
}

void Mesh::add_render_data(SPTR_RenderData rd) { 
	if (rd != nullptr) render_datas.push_back(rd); 
}