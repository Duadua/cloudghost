#include "cameraobject.h"
#include "cameracomponent.h"

CameraObject::CameraObject() {
	root_component = new SceneComponent();
	camera_component = new CameraComponent();
	camera_component->attach_to(root_component);
	
}
CameraObject::~CameraObject() {
	delete root_component;
}

CameraComponent* CameraObject::get_camera_component() {
	return camera_component;
}
