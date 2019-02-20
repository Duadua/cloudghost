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

class AssetManager {
public:
	static std::map<std::string, SPTR_Shader> map_shaders;
	static std::map<std::string, SPTR_Mesh> map_meshs;
	static std::map<std::string, SPTR_Material> map_materials;
	static std::map<std::string, SPTR_Texture2D> map_textures;

public:
	static SPTR_Shader load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path = "");
	static SPTR_Shader get_shader(const std::string& key);
	static bool delete_shader(const std::string& key);
	static bool clear_shaders();

	static SPTR_Mesh load_mesh(const std::string& key, const std::string& src, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Mesh get_mesh(const std::string& key);

	static bool load_materials(const std::string& src, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Material get_material(const std::string& key);

	static bool load_texture(const std::string& path, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Texture2D get_texture(const std::string& key);

private:
	AssetManager() {}
};

