#pragma once

#include "material.h"
#include "gameobject.h"

class PlaneObject : public GameObject {
	DECLARE_CLASS(PlaneObject)
public:
	PlaneObject();
	virtual ~PlaneObject() {}

	virtual void begin_play();
	virtual void tick();

	void set_material(SPTR_Material name, uint rid = 0);

	// DECLARE_ATTR(public, double, width, PlaneObject);

};
DECLARE_AUTO_PTR(PlaneObject)
