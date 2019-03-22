#pragma once
#include "gameobject.h"

class Riven : public GameObject {
	DECLARE_CLASS(Riven)
public:
	Riven();
	virtual ~Riven() {}

	virtual void begin_play();
	virtual void tick();

};
DECLARE_AUTO_PTR(Riven)

