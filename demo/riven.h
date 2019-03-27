#pragma once
#include "gameobject.h"

class Riven : public GameObject {
	DECLARE_CLASS(Riven)
public:
	Riven();
	virtual ~Riven() {}

	virtual void begin_play();

};
DECLARE_AUTO_PTR(Riven)

