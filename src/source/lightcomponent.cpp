#include "lightcomponent.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(LightComponent)

LightComponent::LightComponent() {
	position = CVector3D(0.0f, 1.0f, 0.0f);					// default position at top head
	dirction = CVector3D(0.0, -1.0f, 0.0f);					// default dirction to bottom

	intensity = 1.0f;										// default intensity
}

// =======================================================

IMPLEMENT_CLASS(DirectLightComponent)
DirectLightComponent::DirectLightComponent() {

}
bool DirectLightComponent::use(const std::string& shader) {

	auto t_shader = AssetManager::get_shader(shader);
	if (t_shader == nullptr) return false;
	// bind uniform value for t_shader
	return true;

}

// =======================================================

IMPLEMENT_CLASS(PointLightComponent)

// =======================================================

IMPLEMENT_CLASS(SpotLightComponent)

// =======================================================

IMPLEMENT_CLASS(SkyLightComponent)


