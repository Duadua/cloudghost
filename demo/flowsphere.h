#pragma once
#include "sphereobject.h"

class FlowSphere : public SphereObject {
	DECLARE_CLASS(FlowSphere)
public:
	FlowSphere() : o_location(CVector3D(0.0f)){}

	virtual void begin_play();
	virtual void tick(float time) override;

	void update_origin_location();

private:
	CVector3D o_location;

};
DECLARE_AUTO_PTR(FlowSphere)
