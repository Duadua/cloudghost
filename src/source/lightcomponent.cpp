#include "lightcomponent.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(LightComponent)

LightComponent::LightComponent() {

	color = CVector3D(1.0f);								// default color
	intensity = 1.0f;										// default intensity
}

// =======================================================

IMPLEMENT_CLASS(DirectLightComponent)

uint DirectLightComponent::direct_light_num = 0;

DirectLightComponent::DirectLightComponent() {
	rotation = QVector3D(-46.0f, 0.0f, 0.0f);		// 初始方向
}

bool DirectLightComponent::use(const std::string& shader) {

	auto t_shader = AssetManager::get_shader(shader);
	if (t_shader == nullptr) return false;

	// get uniform name in shader 
	std::ostringstream oss;
	oss << "u_direct_light[" << direct_light_num << "]";
	std::string t_name = oss.str();

	// bind uniform value for t_shader
	t_shader->set_vec3(t_name + ".color", color);
	t_shader->set_vec3(t_name + ".intensity", intensity);
	t_shader->set_vec3(t_name + ".dirction", get_dirction());
	
	t_shader->set_int("u_direct_light_num", ++direct_light_num);

	return true;

}

CVector3D DirectLightComponent::get_dirction() {
	CVector3D res;
	auto t_r = get_rotation();

	float yaw = CMath::deg_to_rad(rotation.y());
	float pitch = CMath::deg_to_rad(rotation.x());
	float roll = CMath::deg_to_rad(rotation.z());

	res.set_x(std::sin(yaw) * std::cos(pitch));
	res.set_z(std::cos(yaw) * std::cos(pitch));
	res.set_y(std::sin(pitch));
	res.normalize();

	return res;
}
// =======================================================

IMPLEMENT_CLASS(PointLightComponent)

PointLightComponent::PointLightComponent() { 
	set_att_radius(50.0f);
	location = QVector3D(0.0f, 1.0f, 0.0f); 
}

uint PointLightComponent::point_light_num = 0;

bool PointLightComponent::use(const std::string& shader) {
	auto t_shader = AssetManager::get_shader(shader);
	if (t_shader == nullptr) return false;

	// get uniform name in shader 
	std::ostringstream oss;
	oss << "u_point_light[" << point_light_num << "]";
	std::string t_name = oss.str();

	// bind uniform value for t_shader
	t_shader->set_vec3(t_name + ".color", color);
	t_shader->set_vec3(t_name + ".intensity", intensity);
	t_shader->set_vec3(t_name + ".position", CVector3D(location.x(), location.y(), location.z()));
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

// =======================================================

IMPLEMENT_CLASS(SkyLightComponent)


