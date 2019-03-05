#pragma once
#include "gameobject.h"

class PlaneObject : public GameObject {
	DECLARE_CLASS(PlaneObject)
public:
	PlaneObject();
	virtual ~PlaneObject() {}

	virtual void begin_play();
	virtual void tick();

	// DECLARE_ATTR(public, double, width, PlaneObject);

};
DECLARE_AUTO_PTR(PlaneObject)
