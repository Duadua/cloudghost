#pragma once

#include "gameobject.h"

class CameraComponent;

class CameraObject : public GameObject {
public:
	CameraObject();
	~CameraObject();

	CameraComponent* get_camera_component();

private:
	CameraComponent * camera_component;
};