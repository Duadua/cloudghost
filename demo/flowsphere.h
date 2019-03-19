#pragma once
#include "sphereobject.h"

class FlowSphere : public SphereObject {
	DECLARE_CLASS(FlowSphere)
public:
	FlowSphere() {}

	virtual void begin_play();
	virtual void tick();

	// DECLARE_ATTR(public, double, width, FlowSphere);

};
DECLARE_AUTO_PTR(FlowSphere)
