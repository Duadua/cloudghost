#pragma once

#include "gameobject.h"
#include "lightcomponent.h"

enum LightType {
	DIRECT,
	POINT,
	SPOT,
	SKY
};

class LightObject : public GameObject {
	DECLARE_CLASS(LightObject)
public:
	LightObject();
	virtual ~LightObject() {}

	bool init(LightType type);

	GET_SET(LightType, light_type);

	SPTR_LightComponent get_light_component() { return light_component; }

private:

	SPTR_LightComponent light_component;

	LightType light_type;

};
DECLARE_AUTO_PTR(LightObject)