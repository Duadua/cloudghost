#pragma once
#include "gameobject.h"

class Mansion : public GameObject {
	DECLARE_CLASS(Mansion)
public:
	Mansion();
	virtual ~Mansion() {}

	virtual void begin_play();
	virtual void tick();

	// DECLARE_ATTR(public, double, width, PlaneObject);

};
DECLARE_AUTO_PTR(Mansion)

