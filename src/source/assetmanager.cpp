#define GLEW_STATIC
#include <GL/glew.h>

#include "mesh.h"
#include "shader.h"
#include "skeleton.h"
#include "skeletalmesh.h"
#include "renderdata.h"
#include "material.h"
#include "animsequence.h"
#include "assetmanager.h"
#include "texture2d.h"
#include "texture3d.h"

#include "tools/meshloader.h"
#include "tools/textureloader.h"
#include "tools/materialloader.h"

SPTR_Shader AssetManager::load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path, SourceType source_type) {
	//if (map_shaders.count(key)) { c_debuger() << "[asset][shader][load] already loaded shader " + key; return nullptr; }
	if (map_shaders.count(key)) { return nullptr; }

	std::string v_code, f_code, g_code;
	if (source_type == SourceType::BY_FILE) {
		v_code = FileHelper_ins().load_txt(v_path); 
		f_code = FileHelper_ins().load_txt(f_path); 
		if(g_path.compare("") != 0) g_code = FileHelper_ins().load_txt(g_path);
	}
	else if (source_type == SourceType::BY_STRING) {
		v_code = v_path; 
		f_code = f_path; 
		g_code = g_path; 
	}

	if (v_code.compare("") == 0 || f_code.compare("") == 0) {
		c_debuger() << "[warning][asset][shader]cannot load shader \"" + key + "\"";
		return nullptr;
	}

	map_shaders[key] = CREATE_CLASS(Shader);
	map_shaders[key]->set_name(key);
	map_shaders[key]->compile(v_code, f_code, g_code);
	return map_shaders[key];
	
}
SPTR_Shader AssetManager::get_shader(const std::string& key) {
	if (!map_shaders.count(key)) {
		c_debuger() << "[warning][asset][shader]no shader calls \"" + key + "\"";
		return nullptr;
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
	// if (map_meshs.count(key)) { c_debuger() << "[asset][mesh][load] already loaded mesh " + key; return nullptr; }
	if (map_meshs.count(key)) { return nullptr; }

	std::vector<MeshData> t_md;
	std::vector<std::string> t_mt_files;
	bool res = false;

	if (source_type == SourceType::BY_FILE) {
		std::string suf = FileHelper_ins().get_suff_of_file(src);			// 获得文件路径后缀
		if (suf.compare(".txt") == 0) { res = MeshLoader::load_mesh_txt(src, t_md, t_mt_files, SourceType::BY_FILE); }
		else if (suf.compare(".obj") == 0) { res = MeshLoader::load_mesh_obj(src, t_md, t_mt_files, SourceType::BY_FILE); }
	}
	else if (source_type == SourceType::BY_STRING) {
		res = MeshLoader::load_mesh_txt(src, t_md, t_mt_files, SourceType::BY_STRING);
	}

	map_meshs[key] = CREATE_CLASS(Mesh);
	if (res) {
		for (auto i : t_mt_files) { AssetManager::load_materials(i); }
		
		for (auto i : t_md) {
			auto t_rd = CREATE_CLASS(RenderData);
			std::vector<CVertex> t_v;
			for (auto j = 0; j < i.vertices.size(); ++j) {
				t_v.push_back(CVertex(i.vertices[j].position, i.vertices[j].normal, i.vertices[j].tex_coord, i.vertices[j].tangent, i.vertices[j].bitangent));
			}
			t_rd->init(t_v, i.indices);
			t_rd->set_material_name(i.material.name);
			map_meshs[key]->add_render_data(t_rd);
		}
	}
	else { c_debuger() << "[warning][asset][mesh]load mesh failed called \"" + key + "\""; }

	return map_meshs[key];
}
SPTR_Mesh AssetManager::load_mesh_x(const std::string& key, const std::string& path) {
	// if (map_meshs.count(key)) { c_debuger() << "[asset][mesh][load] already loaded mesh " + key; return nullptr; }
	if (map_meshs.count(key)) { return nullptr; }

	std::vector<MeshData> t_mds;
	bool res = MeshLoader::load_mesh_x(path, t_mds);

	map_meshs[key] = CREATE_CLASS(Mesh);
	if (res) {
		for (auto md : t_mds) {
			auto t_rd = CREATE_CLASS(RenderData);
			std::vector<CVertex> t_v;
			for (auto j = 0; j < md.vertices.size(); ++j) {
				t_v.push_back(CVertex(md.vertices[j].position, md.vertices[j].normal, md.vertices[j].tex_coord, md.vertices[j].tangent, md.vertices[j].bitangent));
			}
			t_rd->init(t_v, md.indices);
			if (md.material.name.compare("") != 0) {
				if (md.material.map_ka.compare("") != 0) { load_texture_x(md.material.map_ka); }
				if (md.material.map_kd.compare("") != 0) { load_texture_x(md.material.map_kd); }
				if (md.material.map_ks.compare("") != 0) { load_texture_x(md.material.map_ks, false); }
				if (md.material.map_normal.compare("") != 0) { load_texture_x(md.material.map_normal, false); }

				auto t_mt = CREATE_CLASS(Material);
				t_mt->set_name(md.material.name);
				t_mt->set_ka(md.material.ka);
				t_mt->set_kd(md.material.kd);
				t_mt->set_ks(md.material.ks);
				t_mt->set_shininess(md.material.shininess);
				t_mt->set_map_ka(FileHelper_ins().get_name_of_file(md.material.map_ka));
				t_mt->set_map_kd(FileHelper_ins().get_name_of_file(md.material.map_kd));
				t_mt->set_map_ks(FileHelper_ins().get_name_of_file(md.material.map_ks));
				t_mt->set_map_normal(FileHelper_ins().get_name_of_file(md.material.map_normal));
				t_mt->set_map_albedo(FileHelper_ins().get_name_of_file(md.material.map_kd));
				map_materials[t_mt->get_name()] = t_mt;
				t_rd->set_material_name(t_mt->get_name());
			}
			map_meshs[key]->add_render_data(t_rd);
		}
	}
	else { c_debuger() << "[warning][asset][mesh]load mesh failed called \"" + key + "\""; }

	return map_meshs[key];
}
SPTR_Mesh AssetManager::get_mesh_o(const std::string& key) {
	if (!map_meshs.count(key)) {
		c_debuger() << "[warning][asset][mesh]no mesh calls \"" + key + "\"";
		return nullptr;
	}
	return map_meshs[key];
}
SPTR_Mesh AssetManager::get_mesh(const std::string& key) {
	if (!map_meshs.count(key)) {
		c_debuger() << "[warning][asset][mesh]no mesh calls \"" + key + "\"";
		return nullptr;
	}
	auto t_mi = CREATE_CLASS(Mesh);
	if (t_mi) { t_mi->copy_from(map_meshs[key]); }
	return t_mi;
}

SPTR_SkeletalMesh AssetManager::load_mesh_skeletal(const std::string& key, const std::string& path) {
	// if (map_skeletalmeshs.count(key)) { c_debuger() << "[asset][skeletal_mesh][load] already loaded skeletal_mesh " + key; return nullptr; }
	if (map_skeletalmeshs.count(key)) { return nullptr; }

	std::vector<SkeletalMeshData> t_mds;
	MSkeleton t_skeleton;
	std::vector<MBone> t_mbones;
	CMatrix4x4 mat_global;
	std::vector<AnimData> t_ads;
	bool res = MeshLoader::load_mesh_skeletal(path, t_mds, t_skeleton, t_mbones, mat_global, t_ads);

	map_skeletalmeshs[key] = CREATE_CLASS(SkeletalMesh);
	if (res) {
		// load skeleton
		auto t_sk = CREATE_CLASS(Skeleton);
		t_sk->set_name(key);

		std::vector<SkeletonNode> t_nodes;
		for (auto t_n : t_skeleton.nodes) {
			SkeletonNode t_sn;
			t_sn.name = t_n.name;
			t_sn.id = t_n.id;
			t_sn.father = t_n.father;
			t_sn.children.clear(); t_sn.children.assign(t_n.children.begin(), t_n.children.end());
			t_sn.bone_id = t_n.bone_id;
			t_sn.mat_trans = t_n.mat_trans;
			t_nodes.push_back(t_sn);
		}
	
		t_sk->init(t_skeleton.root_node, t_skeleton.map_nodes, t_nodes);
		map_skeletons[key] = t_sk;

		// load skeletal mesh
		map_skeletalmeshs[key]->set_skeleton(t_sk);	
		map_skeletalmeshs[key]->set_mat_global(mat_global);
		// set mesh's bone data
		std::vector<Bone> t_bones;
		for (auto t_b : t_mbones) {
			Bone t_bb;
			t_bb.mat_offset = t_b.mat_offset;
			t_bb.mat_finall = t_b.mat_finall;
			t_bones.push_back(t_bb);
		}
		map_skeletalmeshs[key]->set_bones(t_bones);
		// set mesh's rd data
		for (auto md : t_mds) {
			std::vector<CVertex> t_vs;
			for (auto j = 0; j < md.vertices.size(); ++j) {
				t_vs.push_back(CVertex(md.vertices[j].position, md.vertices[j].normal, md.vertices[j].tex_coord, md.vertices[j].tangent, md.vertices[j].bitangent));
			}
			std::vector<CVertexBone> t_vbs;
			for (auto j = 0; j < md.bones.size(); ++j) {
				CVertexBone t_vb;
				for (auto k = 0; k < std::min(bone_num_per_vertex, m_bone_num_per_vertex); ++k) {
					t_vb.add(md.bones[j].ids[k], md.bones[j].weights[k]);
				}
				t_vbs.push_back(t_vb);
			}
			auto t_rd = CREATE_CLASS(RenderData);
			t_rd->init_with_bone(t_vs, md.indices, t_vbs);
			if (md.material.name.compare("") != 0) {
				if (md.material.map_ka.compare("") != 0) { load_texture_x(md.material.map_ka); }
				if (md.material.map_kd.compare("") != 0) { load_texture_x(md.material.map_kd); }
				if (md.material.map_ks.compare("") != 0) { load_texture_x(md.material.map_ks, false); }
				if (md.material.map_normal.compare("") != 0) { load_texture_x(md.material.map_normal, false); }

				auto t_mt = CREATE_CLASS(Material);
				t_mt->set_name(md.material.name);
				t_mt->set_ka(md.material.ka);
				t_mt->set_kd(md.material.kd);
				t_mt->set_ks(md.material.ks);
				t_mt->set_shininess(md.material.shininess);
				t_mt->set_map_ka(FileHelper_ins().get_name_of_file(md.material.map_ka));
				t_mt->set_map_kd(FileHelper_ins().get_name_of_file(md.material.map_kd));
				t_mt->set_map_ks(FileHelper_ins().get_name_of_file(md.material.map_ks));
				t_mt->set_map_normal(FileHelper_ins().get_name_of_file(md.material.map_normal));
				t_mt->set_map_albedo(FileHelper_ins().get_name_of_file(md.material.map_kd));
				map_materials[t_mt->get_name()] = t_mt;
				t_rd->set_material_name(t_mt->get_name());
			}
			map_skeletalmeshs[key]->add_render_data(t_rd);
		}

		// load animation sequence -- if have
		for (auto ad : t_ads) {
			if (map_anim_sequence.count(ad.name)) { continue; }
			auto t_anim = CREATE_CLASS(AnimSequence);
			t_anim->set_skeleton_name(key);

			t_anim->set_name(ad.name);
			t_anim->set_duration(ad.duration);
			t_anim->set_ticks_per_seconds(ad.ticks_per_seconds);

			std::vector<AnimNode> t_ans;
			for (auto an : ad.anim_nodes) {
				AnimNode t_an;
				t_an.id = an.id;
				t_an.name = an.name;

				t_an.position_keys.clear();
				for (auto pk : an.position_keys) { t_an.position_keys.push_back(AnimPositionKey(pk.time, pk.value)); }

				t_an.rotation_keys.clear();
				for (auto rk : an.rotation_keys) { t_an.rotation_keys.push_back(AnimRotationKey(rk.time, rk.value)); }

				t_an.scale_keys.clear();
				for (auto sk : an.scale_keys) { t_an.scale_keys.push_back(AnimScaleKey(sk.time, sk.value)); }

				t_ans.push_back(t_an);
			}

			t_anim->init(ad.map_anim_nodes, t_ans);

			map_anim_sequence[t_anim->get_name()] = t_anim;
		}

	}
	else { c_debuger() << "[warning][asset][skeletal_mesh]load skeletal_mesh failed called \"" + key + "\""; }

	return map_skeletalmeshs[key];
}
SPTR_SkeletalMesh AssetManager::get_mesh_skeletal_o(const std::string& key) {
	if (!map_skeletalmeshs.count(key)) {
		c_debuger() << "[warning][asset][skeletal_mesh]no skeletal_mesh calls \"" + key + "\"";
		return nullptr;
	}
	return map_skeletalmeshs[key];
}
SPTR_SkeletalMesh AssetManager::get_mesh_skeletal(const std::string& key) {
	if (!map_skeletalmeshs.count(key)) {
		c_debuger() << "[warning][asset][skeletal_mesh]no skeletal_mesh calls \"" + key + "\"";
		return nullptr;
	}
	auto t_mi = CREATE_CLASS(SkeletalMesh);
	if (t_mi) { t_mi->copy_from(map_skeletalmeshs[key]); }
	return t_mi;
}

SPTR_Skeleton AssetManager::get_skeleton(const std::string& key) {
	if (!map_skeletons.count(key)) {
		c_debuger() << "[warning][asset][skeleton]no skeleton calls \"" + key + "\"";
		return nullptr;
	}
	return map_skeletons[key];
}

bool AssetManager::load_anim_sequences(const std::string& path, const std::string& skeleton_name) {
	if (!map_skeletons.count(skeleton_name)) { c_debuger() << "[error][asset][anim] the skeleton this anim sequence depending is not exist called \"" + skeleton_name + "\""; return false; }

	std::vector<AnimData> t_ads;
	bool res = MeshLoader::load_mesh_animation(path, t_ads);

	if (res) {
		for (auto ad : t_ads) {
			if (map_anim_sequence.count(ad.name)) { continue; }
			auto t_anim = CREATE_CLASS(AnimSequence);
			t_anim->set_skeleton_name(skeleton_name);

			t_anim->set_name(ad.name);
			t_anim->set_duration(ad.duration);
			t_anim->set_ticks_per_seconds(ad.ticks_per_seconds);

			std::vector<AnimNode> t_ans;
			for (auto an : ad.anim_nodes) {
				AnimNode t_an;
				t_an.id = an.id;
				t_an.name = an.name;

				t_an.position_keys.clear(); 
				for (auto pk : an.position_keys) { t_an.position_keys.push_back(AnimPositionKey(pk.time, pk.value)); }

				t_an.rotation_keys.clear(); 
				for (auto rk : an.rotation_keys) { t_an.rotation_keys.push_back(AnimRotationKey(rk.time, rk.value)); }

				t_an.scale_keys.clear(); 
				for (auto sk : an.scale_keys) { t_an.scale_keys.push_back(AnimScaleKey(sk.time, sk.value)); }

				t_ans.push_back(t_an);
			}
			t_anim->init(ad.map_anim_nodes, t_ans);

			map_anim_sequence[t_anim->get_name()] = t_anim;
			c_debuger() << "[yep][asset][anim] load animation sequence success call " + t_anim->get_name();

		} // 每一个动画序列
	}
	else { c_debuger() << "[warning][asset][anim] load anim_sequence failed from \"" + path + "\""; }

	return true;
}
SPTR_AnimSequence AssetManager::get_anim_sequence(const std::string& key) {
	if (!map_anim_sequence.count(key)) {
		c_debuger() << "[warning][asset][anim]no anim_sequence calls \"" + key + "\"";
		return nullptr;
	}
	return map_anim_sequence[key];
}

bool AssetManager::load_materials(const std::string& src, SourceType source_type) {
	bool res = false;
	
	std::vector<MaterialData> t_mds;

	if (source_type == SourceType::BY_FILE) {
		std::string suf = FileHelper_ins().get_suff_of_file(src);			// 获得文件路径后缀
		if (suf.compare(".txt") == 0) { res = MaterialLoader::load_material_txt(src,t_mds, SourceType::BY_FILE); }
		else if (suf.compare(".mtl") == 0) { res = MaterialLoader::load_material_mtl(src,t_mds, SourceType::BY_FILE); }
	}
	else if(source_type == SourceType::BY_STRING) { 
		res = MaterialLoader::load_material_txt(src,t_mds, SourceType::BY_STRING);
	}

	for (auto i : t_mds) {
		// if (map_materials.count(i.name)) { c_debuger() << "[asset][materials][load] already loaded material " + i.name; continue; }
		if (map_materials.count(i.name)) { continue; }

		auto t_md = CREATE_CLASS(Material);
		t_md->set_name(i.name);
		t_md->set_ka(i.ka);
		t_md->set_kd(i.kd);
		t_md->set_ks(i.ks);
		t_md->set_shininess(i.shininess);
		t_md->set_map_ka(i.map_ka);
		t_md->set_map_kd(i.map_kd);
		t_md->set_map_ks(i.map_ks);
		t_md->set_map_normal(i.map_normal);
		// pbr
		t_md->set_albedo(i.albedo);
		t_md->set_metallic(i.metallic);
		t_md->set_roughness(i.roughness);
		t_md->set_ao(i.ao);
		t_md->set_map_albedo(i.map_albedo);
		t_md->set_map_metallic(i.map_metallic);
		t_md->set_map_roughness(i.map_roughness);
		t_md->set_map_ao(i.map_ao);
		map_materials[t_md->get_name()] = t_md;
	}

	return res;
}
SPTR_Material AssetManager::get_material_o(const std::string& key) {
	if (!map_materials.count(key)) {
		if (key.compare("") != 0) { c_debuger() << "[warning][asset][material]no material calls \"" + key + "\""; }
		return nullptr;
	}
	return map_materials[key];
}
SPTR_Material AssetManager::get_material(const std::string& key) {
	if (!map_materials.count(key)) {
		if (key.compare("") != 0) { c_debuger() << "[warning][asset][material]no material calls \"" + key + "\""; }
		return nullptr;
	}
	auto t_mi = CREATE_CLASS(Material);
	if (t_mi) { t_mi->copy_from(map_materials[key]); }
	return t_mi;
}

bool AssetManager::load_texture(const std::string& path, SourceType source_type, bool b_srgb) {
	std::string t_name = FileHelper_ins().get_name_of_file(path);			// 获得文件名
	// if (map_textures.count(t_name)) { c_debuger() << "[asset][texture][load] already loaded texture " + t_name; return false; }
	if (map_textures.count(t_name)) { return false; }

	std::string t_suf = FileHelper_ins().get_suff_of_file(path);				// 获得文件路径后缀
    uint width = 0, heigh = 0;
	SPTR_uchar t_res = nullptr;

	if (source_type == SourceType::BY_FILE) {

		/*if (t_suf.compare(".png") == 0) { 
			uint t_size;
			auto t_data = TextureLoader::load_texture_png(path, t_size); 
			if (t_data == nullptr) return false;

			// 暂时使用 QImage 解析 png 数据 --也是使用了 libpng 库
            QByteArray t_ba(reinterpret_cast<char*>(t_data.get()), static_cast<int>(t_size));
			QImage t_img;
			t_img.loadFromData(t_ba, "png");
			t_img = t_img.mirrored();

			// 从 QImage 提取出像素数据，以传给 texture
            width = static_cast<uint>(t_img.width());
            heigh = static_cast<uint>(t_img.height());
            t_res = make_shared_array<uchar>(static_cast<size_t>(t_img.byteCount() + 1));
            memcpy(t_res.get(), t_img.bits(), static_cast<size_t>(t_img.byteCount()));
			
		}
		else*/
		if (t_suf.compare(".txt") == 0) { t_res = TextureLoader::load_texture_txt(path, width, heigh, SourceType::BY_FILE); }

	}
	else if (source_type == SourceType::BY_STRING) { 
		t_res = TextureLoader::load_texture_txt(path, width, heigh, SourceType::BY_STRING); 
	} 

	if (t_res == nullptr) {
		c_debuger() << "[warning][asset][texture]load texture failed called \"" + path + "\"";
		return false;
	}

	// 传给 texture
	auto t_texture = CREATE_CLASS(Texture2D);
	t_texture->set_name(t_name);
	if (b_srgb) { t_texture->set_internal_format(GL_SRGB_ALPHA); }
	else { t_texture->set_internal_format(GL_RGBA); }
	t_texture->set_image_format(GL_RGBA);
	t_texture->init(width, heigh, t_res);
	map_textures[t_name] = t_texture;

	return true;
}
bool AssetManager::load_texture_x(const std::string& path, bool b_srgb) {
	std::string t_name = FileHelper_ins().get_name_of_file(path);			// 获得文件名
	// if (map_textures.count(t_name)) { c_debuger() << "[asset][texture][load] already loaded texture " + t_name; return false; }
	if (map_textures.count(t_name)) { return false; }

	std::string t_suf = FileHelper_ins().get_suff_of_file(path);				// 获得文件路径后缀
    int width = 0, heigh = 0, channel = 0;
	SPTR_uchar t_res = nullptr;

	if (t_suf.compare(".dds") == 0) { t_res = TextureLoader::load_texture_dds(path, width, heigh, channel); }
	else { t_res = TextureLoader::load_texture_x(path, width, heigh, channel); }
	
	if (t_res == nullptr) {
		c_debuger() << "[warning][asset][texture]load texture failed called \"" + path + "\"";
		return false;
	}

	// 传给 texture
	auto t_texture = CREATE_CLASS(Texture2D);
	t_texture->set_name(t_name);
	if (channel == 1) { t_texture->set_internal_format(GL_RED); t_texture->set_image_format(GL_RED); }
	else if (channel == 3) { 
		if (b_srgb) { t_texture->set_internal_format(GL_SRGB); }
		else t_texture->set_internal_format(GL_RGB); 
		t_texture->set_image_format(GL_RGB); 
	}
	else if (channel == 4) {
		if (b_srgb) { t_texture->set_internal_format(GL_SRGB_ALPHA); }
		else { t_texture->set_internal_format(GL_RGBA); }
		t_texture->set_image_format(GL_RGBA);
		t_texture->set_wrap_s(GL_CLAMP_TO_EDGE); t_texture->set_wrap_t(GL_CLAMP_TO_EDGE);
	}
	t_texture->init(width, heigh, t_res);
	map_textures[t_name] = t_texture;

	return true;
}
SPTR_Texture2D AssetManager::get_texture(const std::string& key) {
	if (!map_textures.count(key)) {
		c_debuger() << "[warning][asset][texture]no texture calls \"" + key + "\"";
		return nullptr;
	}
	return map_textures[key];
		
}
SPTR_Texture2D AssetManager::gen_blank_texture(const std::string& key, uint width, uint heigh, uint internal_format, uint format, uint data_type, uint type) {
	if (map_textures.count(key)) { c_debuger() << "[asset][texture][gen] already gen texture " + key; return false; }
	auto t_texture = CREATE_CLASS(Texture2D);
	t_texture->set_name(key);
	t_texture->gen(width, heigh, internal_format, format, data_type, type);
	map_textures[key] = t_texture;
	return map_textures[key];
}

bool AssetManager::load_texture_3d(const std::string& path, bool b_srgb) {
	std::string f_name = FileHelper_ins().get_name_of_file(path);			// 获得文件名
	// if (map_textures.count(f_name)) { c_debuger() << "[asset][texture][load] already loaded texture " + f_name; return false; }
	if (map_textures.count(f_name)) { return false; }

	static std::string suf_name[6] = { "rt", "lf", "up", "dn", "bk", "ft" };	// 右左上下后前 -- 012345

	std::vector<std::string> t_names;
	FileHelper_ins().get_all_files_from_dir(path, t_names);

	//for (int i = 0; i < t_names.size(); ++i) { c_debuger() << t_names[i]; }

	std::vector<SPTR_uchar> t_datas(6);
	std::vector<TextureData> t_texture_datas(6);

	for (int i = 0; i < t_names.size(); ++i) {
		// load one texture of texture3d
		for (int j = 0; j < 6; ++j) {
			if (t_names[i].find("_" + suf_name[j] + ".") != std::string::npos) {
				std::string t_name = FileHelper_ins().get_name_of_file(t_names[i]);
				std::string t_suf = FileHelper_ins().get_suff_of_file(t_names[i]);				// 获得文件路径后缀

				int width = 0, heigh = 0, channel = 0;
				SPTR_uchar t_res = nullptr;

				if (t_suf.compare(".dds") == 0) { t_res = TextureLoader::load_texture_dds(t_names[i], width, heigh, channel); }
				else { t_res = TextureLoader::load_texture_x(t_names[i], width, heigh, channel); }

				if (t_res == nullptr) {
					c_debuger() << "[warning][asset][texture3d]load cube texture failed called \"" + t_names[i] + "\"";
					return false;
				}

				// fill data
				t_datas[j] = t_res;

				t_texture_datas[j].type = GL_TEXTURE_CUBE_MAP_POSITIVE_X + j;
				t_texture_datas[j].name = t_name;

				t_texture_datas[j].width = width;
				t_texture_datas[j].heigh = heigh;

				if (channel == 1) { t_texture_datas[j].internal_format = GL_RED; t_texture_datas[j].image_format = GL_RED; }
				else if (channel == 3) {
					if (b_srgb) { t_texture_datas[j].internal_format = GL_SRGB; }
					else t_texture_datas[j].internal_format = GL_RGB;
					t_texture_datas[j].image_format = GL_RGB;
				}
				else if (channel == 4) {
					if (b_srgb) { t_texture_datas[j].internal_format = GL_SRGB_ALPHA; }
					else { t_texture_datas[j].internal_format = GL_RGBA; }
					t_texture_datas[j].image_format = GL_RGBA;
					t_texture_datas[j].wrap_s = GL_CLAMP_TO_EDGE; 
					t_texture_datas[j].wrap_t = GL_CLAMP_TO_EDGE;
					t_texture_datas[j].wrap_r = GL_CLAMP_TO_EDGE;
				}

				break;
			}
		}
	}

	auto res = CREATE_CLASS(Texture3D);
	res->set_name(f_name);
	res->init(t_texture_datas, t_datas);

	map_texture3Ds[f_name] = res;
	return true;
}
SPTR_Texture3D AssetManager::get_texture3D(const std::string& key) {
	if (!map_texture3Ds.count(key)) {
		c_debuger() << "[warning][asset][texture3D]no cube texture calls \"" + key + "\"";
		return nullptr;
	}
	return map_texture3Ds[key];
}
SPTR_Texture3D AssetManager::gen_blank_texture_3d(const std::string& key, uint width, uint heigh, uint internal_format, uint format, uint data_type, uint type) {
	if (map_texture3Ds.count(key)) { c_debuger() << "[asset][texture_3d][gen] already gen texture_3d " + key; return false; }
	auto t_texture = CREATE_CLASS(Texture3D);
	t_texture->set_name(key);
	t_texture->gen(width, heigh, internal_format, format, data_type, type);
	map_texture3Ds[key] = t_texture;
	return map_texture3Ds[key];
}
