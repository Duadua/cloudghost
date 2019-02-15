#include "assetmanager.h"
#include <QDebug>

QMap<std::string, SPTR_Shader> AssetManager::map_shaders;
QMap<std::string, SPTR_Mesh> AssetManager::map_meshs;

SPTR_Shader AssetManager::load_shader(const std::string& key, const std::string& v_path, const std::string& f_path, const std::string& g_path) {
	map_shaders[key] = CREATE_CLASS(Shader);
	map_shaders[key]->compile(v_path, f_path, g_path);
	return map_shaders[key];
}
SPTR_Shader AssetManager::get_shader(const std::string& key) {
	if (!map_shaders.count(key)) {
		qDebug() << "¡¾error¡¿¡¾asset¡¿¡¾shader¡¿no shader calls " << QString::fromStdString(key) << endl;
		return map_shaders[key] = nullptr;
	}
	return map_shaders[key];
}
bool AssetManager::delete_shader(const std::string& key) {
	if (!map_shaders.count(key)) return false;
	map_shaders[key].reset();
	map_shaders.remove(key);
	return true;
}
bool AssetManager::clear_shaders() {
	for (auto sp : map_shaders) { sp.reset(); }
	map_shaders.clear();
	return true;
}

SPTR_Mesh AssetManager::load_mesh(const std::string& key, const std::string& src, SourceType source_type) {
	map_meshs[key] = CREATE_CLASS(Mesh);
	map_meshs[key]->load(src, source_type);
	return map_meshs[key];
}
SPTR_Mesh AssetManager::get_mesh(const std::string& key) {
	if (!map_meshs.count(key)) {
		qDebug() << "¡¾error¡¿¡¾asset¡¿¡¾mesh¡¿no mesh calls " << QString::fromStdString(key) << endl;
		return map_meshs[key] = nullptr;
	}
	return map_meshs[key];
}