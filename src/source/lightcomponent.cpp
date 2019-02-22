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

// =======================================================

IMPLEMENT_CLASS(SpotLightComponent)

// =======================================================

IMPLEMENT_CLASS(SkyLightComponent)


