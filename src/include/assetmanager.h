#pragma once

#include "mesh.h"
#include "shader.h"
#include <QMap>
#include <map>
#include <QString>
#include <QVector>


class AssetManager {
public:
	static QMap<QString, Shader> map_shaders;
	static QMap<QString, Mesh> map_meshs;
public:
	static Shader& load_shader(const QString& key, const QString& v_path, const QString& f_path, const QString& g_path = nullptr);
	static Shader& get_shader(const QString& key);
	static bool delete_shader(const QString& key);
	static bool clear_shaders();

	static Mesh& load_mesh(const QString& key, const QString path);
	static Mesh& get_mesh(const QString& key);

private:
	AssetManager() {}
};

