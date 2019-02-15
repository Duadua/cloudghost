#pragma once

#include "mesh.h"
#include "shader.h"
#include <QMap>
#include <map>
#include <string>
#include <QVector>

class AssetManager {
public:
	static QMap<std::string, SPTR_Shader> map_shaders;
	static QMap<std::string, SPTR_Mesh> map_meshs;
public:
	static SPTR_Shader load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path = "");
	static SPTR_Shader get_shader(const std::string& key);
	static bool delete_shader(const std::string& key);
	static bool clear_shaders();

	static SPTR_Mesh load_mesh(const std::string& key, const std::string& src, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Mesh get_mesh(const std::string& key);

private:
	AssetManager() {}
};

