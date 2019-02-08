#pragma once

#include "gameobject.h"
#include "cameracomponent.h"

class CameraObject : public GameObject {
	DECLARE_CLASS(CameraObject)
public:
	CameraObject();
	~CameraObject();

	SPTR_CameraComponent get_camera_component();

private:
	SPTR_CameraComponent camera_component;
};
DECLARE_AUTO_PTR(CameraObject)