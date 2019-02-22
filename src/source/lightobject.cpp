#include "lightobject.h"
#include "lightcomponent.h"

IMPLEMENT_CLASS(LightObject)

LightObject::LightObject() { root_component = CREATE_CLASS(SceneComponent); }

bool LightObject::init(LightType type) {
	bool res = true;
	light_type = type;
	switch (light_type) {
	case DIRECT	:	light_component = CREATE_CLASS(DirectLightComponent);	break;
	case POINT	:	light_component = CREATE_CLASS(PointLightComponent);	break;
	case SPOT	:	light_component = CREATE_CLASS(SkyLightComponent);		break;
	case SKY	:	light_component = CREATE_CLASS(SkyLightComponent);		break;
	default		:	res = false;											break;
	}

	if (light_component != nullptr) {
		if (root_component == nullptr) root_component = CREATE_CLASS(SceneComponent);
		if(root_component != nullptr) light_component->attach_to(root_component);
	}
	return res;


}

