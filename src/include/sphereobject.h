#pragma once

#include "material.h"
#include "gameobject.h"

class SphereObject : public GameObject {
	DECLARE_CLASS(SphereObject)
public:
	SphereObject();
	virtual ~SphereObject() {}

	virtual void begin_play();
	virtual void tick();

	void set_material(SPTR_Material name, uint rid = 0);

};
DECLARE_AUTO_PTR(SphereObject)
