#include "shader.h"
#include "lightcomponent.h"
#include "stringhelper.h"
#include "assetmanager.h"
#include <cassert>

IMPLEMENT_CLASS(LightComponent)

LightComponent::LightComponent() {
	id = 0;
	color = CVector3D(1.0f);								// default color
	intensity = 1.0f;										// default intensity
	k = CVector3D(1.0f, 1.0f, 1.0f);
	//k = CVector3D(0.2f, 0.6f, 0.2f);
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

DirectLightComponent::DirectLightComponent() {
	rotation = CVector3D(-46.0f, 0.0f, 0.0f);		// 初始方向
	k = CVector3D(1.0f, 1.0f, 1.0f);
	//k = CVector3D(0.2f, 0.6f, 0.2f);
}

bool DirectLightComponent::use(SPTR_Shader shader) {

	if (shader == nullptr) return false;

	// get uniform name in shader 
	std::string t_name = "u_direct_light[" + StringHelper_ins().int_to_string(id) + "]";

	// bind uniform value for t_shader
	shader->use();
	shader->set_vec3(t_name + ".color", color);
	shader->set_float(t_name + ".intensity", intensity);
	shader->set_vec3(t_name + ".k", k);
	shader->set_vec3(t_name + ".dirction", get_dirction());

	return true;

}

// =======================================================

IMPLEMENT_CLASS(PointLightComponent)

PointLightComponent::PointLightComponent() { 
	location = CVector3D(0.0f, 1.0f, 0.0f);

	set_att_radius(50.0f);

	mat_proj_views.resize(6);
}

bool PointLightComponent::use(SPTR_Shader shader) {
	if (shader == nullptr) return false;

	// get uniform name in shader 
	std::string t_name = "u_point_light[" + StringHelper_ins().int_to_string(id) + "]";

	// bind uniform value for t_shader
	shader->use();
	shader->set_vec3(t_name + ".color", color);
	shader->set_float(t_name + ".intensity", intensity);
	shader->set_vec3(t_name + ".k", k);
	shader->set_vec3(t_name + ".position", location);
	shader->set_float(t_name + ".att_ka", att_ka);
	shader->set_float(t_name + ".att_kb", att_kb);
	shader->set_float(t_name + ".att_kc", att_kc);

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

CMatrix4x4 PointLightComponent::get_mat_proj_view(int tid) { assert(tid >= 0 && tid < 6); return mat_proj_views[tid]; }
void PointLightComponent::set_mat_proj_view(int tid, const CMatrix4x4& mat) { assert(tid >= 0 && tid < 6); mat_proj_views[tid] = mat; }

// =======================================================

IMPLEMENT_CLASS(SpotLightComponent)

SpotLightComponent::SpotLightComponent() {
	location = CVector3D(0.0f, 1.0f, 0.0f);			// init location
	rotation = CVector3D(-90.0f, 0.0f, 0.0f);		// init dirction

	set_att_radius(50.0f);							// init raduis

	// cut off angle
	inner = 40.0f;
	outer = 45.0f;
}

bool SpotLightComponent::use(SPTR_Shader shader) {
	if (shader == nullptr) return false;

	// get uniform name in shader 
	std::string t_name = "u_spot_light[" + StringHelper_ins().int_to_string(id) + "]";

	// bind uniform value for t_shader
	shader->use();
	shader->set_vec3(t_name + ".color", color);
	shader->set_float(t_name + ".intensity", intensity);
	shader->set_vec3(t_name + ".k", k);
	shader->set_vec3(t_name + ".position", location);
	shader->set_vec3(t_name + ".dirction", get_dirction());
	shader->set_float(t_name + ".att_ka", att_ka);
	shader->set_float(t_name + ".att_kb", att_kb);
	shader->set_float(t_name + ".att_kc", att_kc);

	shader->set_float(t_name + ".inner", std::cos(CMath_ins().deg_to_rad(inner)));
	shader->set_float(t_name + ".outer", std::cos(CMath_ins().deg_to_rad(outer)));

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


