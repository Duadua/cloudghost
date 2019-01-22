#pragma once

#include "shader.h"
#include <QMap>
#include <map>
#include <QString>

class AssetManager {
public:
	static QMap<QString, Shader> map_shaders;
	//static std::map<QString, Shader> map_shaders;
public:
	static Shader& load_shader(const QString& key, const QString& v_path, const QString& f_path, const QString& g_path = nullptr);
	static Shader& get_shader(const QString& key);
	static bool delete_shader(const QString& key);
	static bool clear_shaders();

private:
	AssetManager() {}
};

