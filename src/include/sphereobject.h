#pragma once
#include "gameobject.h"

class SphereObject : public GameObject {
	DECLARE_CLASS(SphereObject)
public:
	SphereObject();
	virtual ~SphereObject() {}

	virtual void begin_play();
	virtual void tick();

	void set_material(std::string name, uint id = 0);

};
DECLARE_AUTO_PTR(SphereObject)
