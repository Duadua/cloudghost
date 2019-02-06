#pragma once

#include "gameobject.h"

class CameraComponent;

class CameraObject : public GameObject {
	DECLARE_CLASS(CameraObject)
public:
	CameraObject();
	~CameraObject();

	CameraComponent* get_camera_component();

private:
	CameraComponent * camera_component;
};