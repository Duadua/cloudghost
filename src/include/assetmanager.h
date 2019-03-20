#pragma once

#include "mesh.h"
#include "shader.h"
#include "material.h"
#include "texture2d.h"
#include "tools/meshloader.h"
#include "tools/textureloader.h"
#include "tools/materialloader.h"
#include <map>
#include <string>

#define GL_RGB				0x1907
#define GL_BGRA				0x80E1
#define GL_UNSIGNED_BYTE	0x1401

class AssetManager {
public:
	static std::map<std::string, SPTR_Shader> map_shaders;
	static std::map<std::string, SPTR_Mesh> map_meshs;
	static std::map<std::string, SPTR_Material> map_materials;
	static std::map<std::string, SPTR_Texture2D> map_textures;

public:
	static SPTR_Shader load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path = "", SourceType source_type = SourceType::BY_FILE);
	static SPTR_Shader get_shader(const std::string& key);
	static bool delete_shader(const std::string& key);
	static bool clear_shaders();

	static SPTR_Mesh load_mesh(const std::string& key, const std::string& src, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Mesh load_mesh_x(const std::string& key, const std::string& path);	// load mesh by assimp
	static SPTR_Mesh get_mesh_o(const std::string& key);			// 获得原始 mesh
	static SPTR_Mesh get_mesh(const std::string& key);				// 获得 mesh 实例 -- 深度复制

	static bool load_materials(const std::string& src, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Material get_material(const std::string& key);

	static bool load_texture(const std::string& path, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Texture2D get_texture(const std::string& key);
	static SPTR_Texture2D gen_blank_texture(const std::string& key, uint width, uint heigh, uint internal_format = GL_RGB, uint format = GL_RGB, uint data_type = GL_UNSIGNED_BYTE);		// 创建空白的 texutre2D -- for RT

private:
	AssetManager() {}
};

