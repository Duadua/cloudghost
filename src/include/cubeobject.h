#pragma once

#include "gameobject.h"

PRE_DECLARE_CLASS(Material)
USING_SPTR(Material)

class CubeObject : public GameObject {
	DECLARE_CLASS(CubeObject)
public:
	CubeObject();
	virtual ~CubeObject() {}

	virtual void begin_play();
	virtual void tick();

	void set_material(SPTR_Material name, uint rid = 0);

};
DECLARE_AUTO_PTR(CubeObject)
