#include "lightobject.h"
#include "lightcomponent.h"

bool LightObject::use(std::string& shader) {
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

IMPLEMENT_CLASS(SpotLightObject)

// ======================================================================

IMPLEMENT_CLASS(PointLightObject)

// ======================================================================

IMPLEMENT_CLASS(SkyLightObject)


