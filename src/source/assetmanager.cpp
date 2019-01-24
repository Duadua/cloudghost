#include "assetmanager.h"
#include <QDebug>

QMap<QString, Shader> AssetManager::map_shaders;
QMap<QString, Mesh> AssetManager::map_meshs;

Shader& AssetManager::load_shader(const QString& key, const QString& v_path, const QString& f_path, const QString& g_path) {
	map_shaders[key] = Shader();
	map_shaders[key].compile(v_path, f_path, g_path);
	return map_shaders[key];
}
Shader& AssetManager::get_shader(const QString& key) {
	if (!map_shaders.count(key)) {
		qDebug() << "¡¾error¡¿¡¾asset¡¿¡¾shader¡¿no shader calls " << key << endl;
		return map_shaders[key] = Shader();
	}
	return map_shaders[key];
}
bool AssetManager::delete_shader(const QString& key) {
	if (!map_shaders.count(key)) return false;
	map_shaders.remove(key);
	return true;
}
bool AssetManager::clear_shaders() {
	map_shaders.clear();
	return true;
}

Mesh& AssetManager::load_mesh(const QString& key, const QString path) {
	map_meshs[key] = Mesh();
	map_meshs[key].load(path);
	return map_meshs[key];
}
Mesh& AssetManager::get_mesh(const QString& key) {
	if (!map_meshs.count(key)) {
		qDebug() << "¡¾error¡¿¡¾asset¡¿¡¾mesh¡¿no mesh calls " << key << endl;
		return map_meshs[key] = Mesh();
	}
	return map_meshs[key];
}