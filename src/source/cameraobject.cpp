#include "cameraobject.h"

IMPLEMENT_CLASS(CameraObject)

CameraObject::CameraObject() {
	root_component = CREATE_CLASS(SceneComponent);
	camera_component = CREATE_CLASS(CameraComponent);
	camera_component->attach_to(root_component);
	
}
CameraObject::~CameraObject() {}

SPTR_CameraComponent CameraObject::get_camera_component() { return camera_component; }
