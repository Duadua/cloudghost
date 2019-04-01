#include "mesh.h"
#include "material.h"
#include "renderdata.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(Mesh)

Mesh::Mesh() : use_default_mt(true) {}
Mesh::Mesh(const Mesh& b) : use_default_mt(b.use_default_mt) {
	render_datas.assign(b.render_datas.begin(), b.render_datas.end());
}
void Mesh::copy_from(const SPTR_Mesh b) {
	use_default_mt = b->use_default_mt;
	render_datas.assign(b->render_datas.begin(), b->render_datas.end());
}
Mesh::~Mesh() {}

void Mesh::tick(float time) { time; }

void Mesh::draw(SPTR_Shader shader) {
	for (auto rd : render_datas) { 
		auto t_material = rd.material;
		if (t_material == nullptr && rd.rd->get_material_name().compare("") != 0) { t_material = AssetManager_ins().get_material(rd.rd->get_material_name()); }
		if (t_material == nullptr && use_default_mt) { t_material = AssetManager_ins().get_material(Material::default_material_name); }
		if (t_material != nullptr) { t_material->use(shader); }
		rd.rd->draw(); 
		Material::un_use(shader);
	}
}

void Mesh::add_render_data(SPTR_RenderData rd) { 
	if (rd != nullptr) render_datas.push_back(RenderDataInstance(rd, AssetManager_ins().get_material(rd->get_material_name()))); 
}
RenderDataInstance& Mesh::render_data(uint id) {
	id = CMath_ins().clamp<uint>(id, 0, static_cast<uint>(render_datas.size() - 1));
	return render_datas[id];
}
