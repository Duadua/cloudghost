#pragma once
#include "gameobject.h"

class Street : public GameObject {
	DECLARE_CLASS(Street)
public:
	Street();
	virtual ~Street() {}

	virtual void begin_play();
	virtual void tick();

};
DECLARE_AUTO_PTR(Street)

