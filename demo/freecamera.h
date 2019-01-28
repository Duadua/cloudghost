#pragma once

#include "cameraobject.h"

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

class FreeCamera : public CameraObject {
public:
	FreeCamera();
	~FreeCamera();

	virtual void bind_input();

protected:

	void turn_rate(float offset);

};
