#include "material.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(Material)

Material::Material() {
	
}

void Material::use(const std::string& shader) {
	auto t_shader = AssetManager::get_shader(shader);
	if (t_shader == nullptr) return;

	t_shader->set_vec3("u_material.ka", ka);
	t_shader->set_vec3("u_material.kd", kd);
	t_shader->set_vec3("u_material.ks", ks);

	t_shader->set_float("u_material.shininess", shininess);
	

}
