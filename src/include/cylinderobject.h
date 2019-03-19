#pragma once
#include "gameobject.h"

class CylinderObject : public GameObject {
	DECLARE_CLASS(CylinderObject)
public:
	CylinderObject();
	virtual ~CylinderObject() {}

	virtual void begin_play();
	virtual void tick();

	void set_material(std::string name);

};
DECLARE_AUTO_PTR(CylinderObject)
