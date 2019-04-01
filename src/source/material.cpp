#include "shader.h"
#include "material.h"
#include "texture2d.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(Material)

std::string Material::default_material_name = "default";

Material::Material() {
	ka = CVector3D(1.0f); kd = CVector3D(1.0f); ks = CVector3D(1.0f); 
	shininess = 1.0f;
	map_ka = ""; map_kd = ""; map_ks = "";
}
Material::Material(const Material& b) : ka(b.ka), kd(b.kd), ks(b.ks), shininess(b.shininess), 
	map_ka(b.map_ka), map_kd(b.map_kd), map_ks(b.map_ks) { }
void Material::copy_from(const SPTR_Material b) {
	ka = b->ka; kd = b->kd; ks = b->ks; 
	shininess = b->shininess;
	map_ka = b->map_ka; map_kd = b->map_kd; map_ks = b->map_ks;
}

void Material::use(SPTR_Shader shader) {
	if (shader == nullptr) return;

	shader->set_vec3("u_material.ka", ka);
	shader->set_vec3("u_material.kd", kd);
	shader->set_vec3("u_material.ks", ks);

	shader->set_float("u_material.shininess", shininess);

	// map_ka
	if (map_ka.compare("") != 0) {
		shader->set_int("u_material.map_ka", 0);
		shader->set_int("u_material.has_map_ka", true);
		auto t_tex = AssetManager_ins().get_texture(map_ka);
		if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
		if(t_tex != nullptr) { t_tex->bind(0); }
	}
	else { 
		if (map_kd.compare("") != 0) {
			shader->set_int("u_material.map_ka", 0);
			shader->set_int("u_material.has_map_ka", true);
			auto t_tex = AssetManager_ins().get_texture(map_kd);
			if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
			if(t_tex != nullptr) { t_tex->bind(0); }

		}
		else { shader->set_int("u_material.has_map_ka", false); }
	}

	// map_kd
	if (map_kd.compare("") != 0) {
		shader->set_int("u_material.map_kd", 1);
		shader->set_int("u_material.has_map_kd", true);
		auto t_tex = AssetManager_ins().get_texture(map_kd);
		if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
		if(t_tex != nullptr) { t_tex->bind(1); }
	}
	else {
		if (map_ka.compare("") != 0) {
			shader->set_int("u_material.map_kd", 1);
			shader->set_int("u_material.has_map_kd", true);
			auto t_tex = AssetManager_ins().get_texture(map_ka);
			if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
			if(t_tex != nullptr) { t_tex->bind(1); }
		}
		else { shader->set_int("u_material.has_map_kd", false); }
	}

	// map_ks
	if (map_ks.compare("") != 0) {
		shader->set_int("u_material.map_ks", 2);
		shader->set_int("u_material.has_map_ks", true);
		auto t_tex = AssetManager_ins().get_texture(map_ks);
		if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
		if(t_tex != nullptr) { t_tex->bind(2); }
	}
	else { shader->set_int("u_material.has_map_ks", false); }
	
}

void Material::un_use(SPTR_Shader shader) {
	if (shader == nullptr) return;

	shader->set_vec3("u_material.ka", CVector3D(1.0f));
	shader->set_vec3("u_material.kd", CVector3D(1.0f));
	shader->set_vec3("u_material.ks", CVector3D(1.0f));

	shader->set_float("u_material.shininess", 1.0f);

	shader->set_int("u_material.has_map_ka", false);
	shader->set_int("u_material.has_map_kd", false);
	shader->set_int("u_material.has_map_ks", false);

	Texture2D::un_bind(0);
	Texture2D::un_bind(1);
	Texture2D::un_bind(2);

}
