#include "shader.h"
#include "lightcomponent.h"
#include "stringhelper.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(LightComponent)

LightComponent::LightComponent() {

	color = CVector3D(1.0f);								// default color
	intensity = 1.0f;										// default intensity
	k = CVector3D(0.2f, 0.6f, 0.2f);
}

CVector3D LightComponent::get_dirction() {
	CVector3D res;
	auto t_r = get_rotation();

	float yaw = CMath_ins().deg_to_rad(rotation.y());
	float pitch = CMath_ins().deg_to_rad(rotation.x());
    //float roll = CMath_ins().deg_to_rad(rotation.z());

	res.set_x(std::sin(yaw) * std::cos(pitch));
	res.set_z(std::cos(yaw) * std::cos(pitch));
	res.set_y(std::sin(pitch));
	res.normalize();

	return res;
}

// =======================================================

IMPLEMENT_CLASS(DirectLightComponent)

int DirectLightComponent::direct_light_num = 0;

DirectLightComponent::DirectLightComponent() {
	rotation = CVector3D(-46.0f, 0.0f, 0.0f);		// 初始方向
	//k = CVector3D(1.0f, 1.0f, 1.0f);
	k = CVector3D(0.2f, 0.6f, 0.2f);

}

bool DirectLightComponent::use(const std::string& shader) {

	auto t_shader = AssetManager_ins().get_shader(shader);
	if (t_shader == nullptr) return false;

	// get uniform name in shader 
	std::string t_name = "u_direct_light[" + StringHelper_ins().int_to_string(direct_light_num) + "]";

	// bind uniform value for t_shader
	t_shader->use();
	t_shader->set_vec3(t_name + ".color", color);
	t_shader->set_float(t_name + ".intensity", intensity);
	t_shader->set_vec3(t_name + ".k", k);
	t_shader->set_vec3(t_name + ".dirction", get_dirction());
	
    t_shader->set_int("u_direct_light_num", ++direct_light_num);

	return true;

}

// =======================================================

IMPLEMENT_CLASS(PointLightComponent)

int PointLightComponent::point_light_num = 0;

PointLightComponent::PointLightComponent() { 
	location = CVector3D(0.0f, 1.0f, 0.0f); 
	k = CVector3D(0.1f, 0.9f, 1.0f);

	set_att_radius(50.0f);
}

bool PointLightComponent::use(const std::string& shader) {
	auto t_shader = AssetManager_ins().get_shader(shader);
	if (t_shader == nullptr) return false;

	// get uniform name in shader 
	std::string t_name = "u_point_light[" + StringHelper_ins().int_to_string(point_light_num) + "]";

	// bind uniform value for t_shader
	t_shader->use();
	t_shader->set_vec3(t_name + ".color", color);
	t_shader->set_float(t_name + ".intensity", intensity);
	t_shader->set_vec3(t_name + ".k", k);
	t_shader->set_vec3(t_name + ".position", location);
	t_shader->set_float(t_name + ".att_ka", att_ka);
	t_shader->set_float(t_name + ".att_kb", att_kb);
	t_shader->set_float(t_name + ".att_kc", att_kc);

	t_shader->set_int("u_point_light_num", ++point_light_num);

	return true;
}

void PointLightComponent::set_att_radius(float t_att_radius) {
	att_raduis = t_att_radius;
	update_att();
}
void PointLightComponent::update_att() {
	att_ka = 1.0f;
	att_kb = 4.5f / att_raduis;
	att_kc = 75.0f / att_raduis / att_raduis;
}

// =======================================================

IMPLEMENT_CLASS(SpotLightComponent)

int SpotLightComponent::spot_light_num = 0;

SpotLightComponent::SpotLightComponent() {
	location = CVector3D(0.0f, 1.0f, 0.0f);			// init location
	rotation = CVector3D(-90.0f, 0.0f, 0.0f);		// init dirction
	k = CVector3D(0.0f, 1.0f, 1.0f);

	set_att_radius(50.0f);							// init raduis

	// cut off angle
	inner = 40.0f;
	outer = 45.0f;
}

bool SpotLightComponent::use(const std::string& shader) {
	auto t_shader = AssetManager_ins().get_shader(shader);
	if (t_shader == nullptr) return false;

	// get uniform name in shader 
	std::string t_name = "u_spot_light[" + StringHelper_ins().int_to_string(spot_light_num) + "]";

	// bind uniform value for t_shader
	t_shader->use();
	t_shader->set_vec3(t_name + ".color", color);
	t_shader->set_float(t_name + ".intensity", intensity);
	t_shader->set_vec3(t_name + ".k", k);
	t_shader->set_vec3(t_name + ".position", location);
	t_shader->set_vec3(t_name + ".dirction", get_dirction());
	t_shader->set_float(t_name + ".att_ka", att_ka);
	t_shader->set_float(t_name + ".att_kb", att_kb);
	t_shader->set_float(t_name + ".att_kc", att_kc);

	t_shader->set_float(t_name + ".inner", std::cos(CMath_ins().deg_to_rad(inner)));
	t_shader->set_float(t_name + ".outer", std::cos(CMath_ins().deg_to_rad(outer)));

	t_shader->set_int("u_spot_light_num", ++spot_light_num);

	return true;
}

void SpotLightComponent::set_att_radius(float t_att_radius) {
	att_raduis = t_att_radius;
	update_att();
}
void SpotLightComponent::update_att() {
	att_ka = 1.0f;
	att_kb = 4.5f / att_raduis;
	att_kc = 75.0f / att_raduis / att_raduis;
}

// =======================================================

IMPLEMENT_CLASS(SkyLightComponent)


