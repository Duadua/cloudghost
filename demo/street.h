#pragma once
#include "gameobject.h"

class Street : public GameObject {
	DECLARE_CLASS(Street)
public:
	Street();
	virtual ~Street() {}

	virtual void begin_play();

};
DECLARE_AUTO_PTR(Street)

