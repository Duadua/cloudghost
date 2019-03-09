#include "assetmanager.h"
#include <QImage>
#include <QDebug>

std::map<std::string, SPTR_Shader> AssetManager::map_shaders;
std::map<std::string, SPTR_Mesh> AssetManager::map_meshs;
std::map<std::string, SPTR_Material> AssetManager::map_materials;
std::map<std::string, SPTR_Texture2D> AssetManager::map_textures;

SPTR_Shader AssetManager::load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path, SourceType source_type) {
	std::string v_code, f_code, g_code;
	if (source_type == SourceType::BY_FILE) {
		v_code = load_txt(v_path); 
		f_code = load_txt(f_path); 
		if(g_path.compare("") != 0) g_code = load_txt(g_path);
	}
	else if (source_type == SourceType::BY_STRING) {
		v_code = v_path; 
		f_code = f_path; 
		g_code = g_path; 
	}

	if (v_code.compare("") == 0 || f_code.compare("") == 0) {
		qDebug() << "【error】【asset】【shader】cannot load shader " << QString::fromStdString(key);
		return nullptr;
	}

	map_shaders[key] = CREATE_CLASS(Shader);
	map_shaders[key]->set_name(key);
	map_shaders[key]->compile(v_code, f_code, g_code);
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
		std::string suf = get_suff_of_file(src);			// 获得文件路径后缀
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

bool AssetManager::load_materials(const std::string& src, SourceType source_type) {
	bool res = false;
	
	std::vector<MaterialData> t_mds;

	if (source_type == SourceType::BY_FILE) {
		std::string suf = get_suff_of_file(src);			// 获得文件路径后缀
		if (suf.compare(".txt") == 0) { res = MaterialLoader::load_material_txt(src,t_mds, SourceType::BY_FILE); }
		else if (suf.compare(".mtl") == 0) { res = MaterialLoader::load_material_mtl(src,t_mds, SourceType::BY_FILE); }
	}
	else if(source_type == SourceType::BY_STRING) { 
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
		t_md->set_map_kd(i.map_kd);
		t_md->set_map_ks(i.map_ks);
		map_materials[t_md->get_name()] = t_md;
	}

	return res;
}
SPTR_Material AssetManager::get_material(const std::string& key) {
	if (!map_materials.count(key)) {
		qDebug() << "【warning】【asset】【material】no material calls " << QString::fromStdString(key) << endl;
		return map_materials[key] = nullptr;
	}
	return map_materials[key];
}

bool AssetManager::load_texture(const std::string& path, SourceType source_type) {
	std::string t_name = get_name_of_file(path);			// 获得文件名
	std::string t_suf = get_suff_of_file(path);				// 获得文件路径后缀
	uint width, heigh;
	SPTR_uchar t_res;

	if (source_type == SourceType::BY_FILE) {

		if (t_suf.compare(".png") == 0) { 
			uint t_size;
			auto t_data = TextureLoader::load_texture_png(path, t_size); 
			if (t_data == nullptr) return false;

			// 暂时使用 QImage 解析 png 数据 --也是使用了 libpng 库
			QByteArray t_ba((char*)t_data.get(), t_size);
			QImage t_img;
			t_img.loadFromData(t_ba, "png");
			t_img = t_img.mirrored();

			// 从 QImage 提取出像素数据，以传给 texture
			width = t_img.width(); 
			heigh = t_img.height();
			t_res = make_shared_array<uchar>(t_img.byteCount() + 1);
			memcpy(t_res.get(), t_img.bits(), t_img.byteCount());
			
		}
		else if (t_suf.compare(".txt") == 0) { t_res = TextureLoader::load_texture_txt(path, width, heigh, SourceType::BY_FILE); }

	}
	else if (source_type == SourceType::BY_STRING) { 
		t_res = TextureLoader::load_texture_txt(path, width, heigh, SourceType::BY_STRING); 
	} 

	if (t_res == nullptr) return false;

	// 传给 texture
	auto t_texture = CREATE_CLASS(Texture2D);
	t_texture->set_name(t_name);
	t_texture->set_internal_format(GL_BGRA);
	t_texture->set_image_format(GL_BGRA);
	t_texture->init(width, heigh, t_res);
	map_textures[t_name] = t_texture;

	return true;
}
SPTR_Texture2D AssetManager::get_texture(const std::string& key) {
	if (!map_textures.count(key)) {
		qDebug() << "【error】【asset】【texture】no texture calls " << QString::fromStdString(key) << endl;
		return map_textures[key] = nullptr;
	}
	return map_textures[key];
		
}
SPTR_Texture2D AssetManager::gen_blank_texture(const std::string& key, uint width, uint heigh, uint internal_format, uint format, uint data_type) {
	auto t_texture = CREATE_CLASS(Texture2D);
	t_texture->set_name(key);
	t_texture->gen(width, heigh, internal_format, format, data_type);
	map_textures[key] = t_texture;
	return map_textures[key];
}

