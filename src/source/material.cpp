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
	map_normal = "";
}
Material::Material(const Material& b) : ka(b.ka), kd(b.kd), ks(b.ks), shininess(b.shininess), 
	map_ka(b.map_ka), map_kd(b.map_kd), map_ks(b.map_ks), map_normal(b.map_normal) { }
void Material::copy_from(const SPTR_Material b) {
	ka = b->ka; kd = b->kd; ks = b->ks; 
	shininess = b->shininess;
	map_ka = b->map_ka; map_kd = b->map_kd; map_ks = b->map_ks;
	map_normal = b->map_normal;
}

void Material::use(SPTR_Shader shader) {
	if (shader == nullptr) return;
	shader->use();

	shader->set_vec3("u_material.ka", ka);
	shader->set_vec3("u_material.kd", kd);
	shader->set_vec3("u_material.ks", ks);

	shader->set_float("u_material.shininess", shininess);

	// map_ka
	if (map_ka.compare("") != 0) {
		shader->set_int("u_material.map_ka", material_map_ka_id);
		shader->set_int("u_material.has_map_ka", true);
		auto t_tex = AssetManager_ins().get_texture(map_ka);
		if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
		if(t_tex != nullptr) { t_tex->bind(material_map_ka_id); }
	}
	else { 
		if (map_kd.compare("") != 0) {
			shader->set_int("u_material.map_ka", material_map_ka_id);
			shader->set_int("u_material.has_map_ka", true);
			auto t_tex = AssetManager_ins().get_texture(map_kd);
			if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
			if(t_tex != nullptr) { t_tex->bind(material_map_ka_id); }

		}
		else { shader->set_int("u_material.has_map_ka", false); }
	}

	// map_kd
	if (map_kd.compare("") != 0) {
		shader->set_int("u_material.map_kd", material_map_kd_id);
		shader->set_int("u_material.has_map_kd", true);
		auto t_tex = AssetManager_ins().get_texture(map_kd);
		if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
		if(t_tex != nullptr) { t_tex->bind(material_map_kd_id); }
	}
	else {
		if (map_ka.compare("") != 0) {
			shader->set_int("u_material.map_kd", material_map_kd_id);
			shader->set_int("u_material.has_map_kd", true);
			auto t_tex = AssetManager_ins().get_texture(map_ka);
			if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
			if(t_tex != nullptr) { t_tex->bind(material_map_kd_id); }
		}
		else { shader->set_int("u_material.has_map_kd", false); }
	}

	// map_ks
	if (map_ks.compare("") != 0) {
		shader->set_int("u_material.map_ks", material_map_ks_id);
		shader->set_int("u_material.has_map_ks", true);
		auto t_tex = AssetManager_ins().get_texture(map_ks);
		if (t_tex == nullptr) { t_tex = AssetManager_ins().get_texture("texture_default.png"); }
		if(t_tex != nullptr) { t_tex->bind(material_map_ks_id); }
	}
	else { shader->set_int("u_material.has_map_ks", false); }

	// map_normal
	if (map_normal.compare("") != 0) {
		auto t_tex = AssetManager_ins().get_texture(map_normal);
		if (t_tex) {
			t_tex->bind(material_map_normal_id);
			shader->set_int("u_material.map_normal", material_map_normal_id);
			shader->set_int("u_material.has_map_normal", true);
		}
		else { shader->set_int("u_material.has_map_normal", false); }
	}
	else { shader->set_int("u_material.has_map_normal", false); }
	
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
	shader->set_int("u_material.has_map_normal", false);

	Texture2D::un_bind(material_map_ka_id);
	Texture2D::un_bind(material_map_kd_id);
	Texture2D::un_bind(material_map_ks_id);
	Texture2D::un_bind(material_map_normal_id);

}
