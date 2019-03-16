#include "lightobject.h"
#include "lightcomponent.h"

bool LightObject::use(const std::string& shader) {
	auto t_lc = std::dynamic_pointer_cast<LightComponent>(root_component);
	if (t_lc == nullptr) return false;
	return t_lc->use(shader);
}

// ======================================================================

IMPLEMENT_CLASS(DirectLightObject)

DirectLightObject::DirectLightObject() {
	light_type = LightType::DIRECT_LIGHT;
	root_component = CREATE_CLASS(DirectLightComponent);
}

// ======================================================================

IMPLEMENT_CLASS(PointLightObject)

PointLightObject::PointLightObject() {
	light_type = LightType::POINT_LIGHT;
	root_component = CREATE_CLASS(PointLightComponent);
}

// ======================================================================

IMPLEMENT_CLASS(SpotLightObject)

SpotLightObject::SpotLightObject() {
	light_type = LightType::SPOT_LIGHT;
	root_component = CREATE_CLASS(SpotLightComponent);
}

// ======================================================================

IMPLEMENT_CLASS(SkyLightObject)

SkyLightObject::SkyLightObject() {
	light_type = LightType::SKY_LIGHT;
	root_component = CREATE_CLASS(SkyLightComponent);
}

