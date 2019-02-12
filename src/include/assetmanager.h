#pragma once

#include "mesh.h"
#include "shader.h"
#include <QMap>
#include <map>
#include <string>
#include <QString>
#include <QVector>

class AssetManager {
public:
	static QMap<QString, SPTR_Shader> map_shaders;
	static QMap<std::string, SPTR_Mesh> map_meshs;
public:
	static SPTR_Shader load_shader(const QString& key, const QString& v_path, const QString& f_path, const QString& g_path = nullptr);
	static SPTR_Shader get_shader(const QString& key);
	static bool delete_shader(const QString& key);
	static bool clear_shaders();

	static SPTR_Mesh load_mesh(const std::string& key, const std::string& src, SourceType source_type = SourceType::BY_FILE);
	static SPTR_Mesh get_mesh(const std::string& key);

private:
	AssetManager() {}
};

