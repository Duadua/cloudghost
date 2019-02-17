#include "material.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(Material)

std::string Material::default_material_name = "cyan_plastic";

Material::Material() {
	ka = CVector3D(1.0f);
	kd = CVector3D(1.0f);
	ks = CVector3D(1.0f);

	shininess = 1.0f;

	map_ka = "";
	map_kd = "";
	map_ks = "";
}

void Material::use(const std::string& shader) {
	auto t_shader = AssetManager::get_shader(shader);
	if (t_shader == nullptr) return;

	t_shader->set_vec3("u_material.ka", ka);
	t_shader->set_vec3("u_material.kd", kd);
	t_shader->set_vec3("u_material.ks", ks);

	t_shader->set_float("u_material.shininess", shininess);
	

}
