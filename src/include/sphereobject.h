#pragma once

#include "gameobject.h"

PRE_DECLARE_CLASS(Material)
USING_SPTR(Material)

class SphereObject : public GameObject {
	DECLARE_CLASS(SphereObject)
public:
	SphereObject();
	virtual ~SphereObject() {}

	virtual void begin_play();

	void set_material(SPTR_Material name, uint rid = 0);

};
DECLARE_AUTO_PTR(SphereObject)
