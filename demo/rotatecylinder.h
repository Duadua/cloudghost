#pragma once
#include "cylinderobject.h"

class RotateCylinder : public CylinderObject {
	DECLARE_CLASS(RotateCylinder)
public:
	RotateCylinder() : o_rotation(CVector3D(0.0f)) {}

	virtual void begin_play();
	virtual void tick();

	void update_origin_rotation();

private:
	CVector3D o_rotation;

};
DECLARE_AUTO_PTR(RotateCylinder)
