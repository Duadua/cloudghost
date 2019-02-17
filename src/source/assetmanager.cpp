#include "assetmanager.h"
#include <QDebug>

std::map<std::string, SPTR_Shader> AssetManager::map_shaders;
std::map<std::string, SPTR_Mesh> AssetManager::map_meshs;
std::map<std::string, SPTR_Material> AssetManager::map_materials;

SPTR_Shader AssetManager::load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path) {
	map_shaders[key] = CREATE_CLASS(Shader);
	map_shaders[key]->compile(v_path, f_path, g_path);
	return map_shaders[key];
}
SPTR_Shader AssetManager::get_shader(const std::string& key) {
	if (!map_shaders.count(key)) {
		qDebug() << "【error】【asset】【shader】no shader calls " << QString::fromStdString(key) << endl;
		return map_shaders[key] = nullptr;
	}
	return map_shaders[key];
}
bool AssetManager::delete_shader(const std::string& key) {
	if (!map_shaders.count(key)) return false;
	map_shaders[key].reset();
	map_shaders.erase(key);
	return true;
}
bool AssetManager::clear_shaders() {
	for (auto sp : map_shaders) { sp.second.reset(); }
	map_shaders.clear();
	return true;
}

SPTR_Mesh AssetManager::load_mesh(const std::string& key, const std::string& src, SourceType source_type) {
	std::vector<MVertex> t_v;
	std::vector<MeshData> t_md;
	std::vector<std::string> t_mt_files;
	bool res = false;

	if (source_type == SourceType::BY_FILE) {
		// 获得文件路径后缀
		int idx = src.find_last_of('.');
		std::string suf = src.substr(idx);

		// 通过后缀调用函数
		if (suf.compare(".txt") == 0) { res = MeshLoader::load_mesh_txt(src, t_v, t_md, t_mt_files, SourceType::BY_FILE); }
		else if (suf.compare(".obj") == 0) { res = MeshLoader::load_mesh_obj(src, t_v, t_md, t_mt_files, SourceType::BY_FILE); }

	}
	else if (source_type == SourceType::BY_STRING) {
		res = MeshLoader::load_mesh_txt(src, t_v, t_md, t_mt_files, SourceType::BY_STRING);
	}

	map_meshs[key] = CREATE_CLASS(Mesh);
	if (res) {
		for (auto i : t_mt_files) { AssetManager::load_materials(i); }
		
		uint t_idx_cnt = 0;
		for (auto i : t_md) {
			if (i.indices.size() == 0) continue;
			++t_idx_cnt;
			auto t_rd = CREATE_CLASS(RenderData);
			t_rd->init(t_v, i.indices);
			t_rd->set_material_name(i.material);
			map_meshs[key]->add_render_data(t_rd);
		}
		if (t_idx_cnt == 0) {
			auto t_rd = CREATE_CLASS(RenderData);
			t_rd->init(t_v, std::vector<uint>());
			map_meshs[key]->add_render_data(t_rd);
		} // no indices -- by draw_array()
	}
	return map_meshs[key];
}
SPTR_Mesh AssetManager::get_mesh(const std::string& key) {
	if (!map_meshs.count(key)) {
		qDebug() << "【error】【asset】【mesh】no mesh calls " << QString::fromStdString(key) << endl;
		return map_meshs[key] = nullptr;
	}
	return map_meshs[key];
}

bool AssetManager::load_materials(const std::string& src, SourceType source_ype) {
	bool res = false;
	
	std::vector<MaterialData> t_mds;

	if (source_ype == SourceType::BY_FILE) {
		// 获得文件路径后缀
		int idx = src.find_last_of('.');
		std::string suf = src.substr(idx);
		if (suf.compare(".txt") == 0) { res = MaterialLoader::load_material_txt(src,t_mds, SourceType::BY_FILE); }
		else if (suf.compare(".mtl") == 0) { res = MaterialLoader::load_material_mtl(src,t_mds, SourceType::BY_FILE); }
	}
	else if(source_ype == SourceType::BY_STRING) { 
		res = MaterialLoader::load_material_txt(src,t_mds, SourceType::BY_STRING);
	}

	for (auto i : t_mds) {
		auto t_md = CREATE_CLASS(Material);
		t_md->set_name(i.name);
		t_md->set_ka(i.ka);
		t_md->set_kd(i.kd);
		t_md->set_ks(i.ks);
		t_md->set_shininess(i.shininess);
		t_md->set_map_ka(i.map_ka);
		t_md->set_map_ka(i.map_ka);
		t_md->set_map_ka(i.map_ka);
		map_materials[t_md->get_name()] = t_md;
	}

	return res;
}
SPTR_Material AssetManager::get_material(const std::string& key) {
	if (!map_materials.count(key)) {
		if (map_materials.count(Material::default_material_name)) { return map_materials[Material::default_material_name]; }
		qDebug() << "【error】【asset】【material】no mesh calls " << QString::fromStdString(key) << endl;
		return map_materials[key] = nullptr;
	}
	return map_materials[key];
}
