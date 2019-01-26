#pragma once

#include "cameraobject.h"

class FreeCamera : public CameraObject {
public:
	FreeCamera();
	~FreeCamera();

	virtual void bind_input();

private:
	void click();

};
