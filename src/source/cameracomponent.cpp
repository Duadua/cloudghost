#include "mesh.h"
#include "cameradata.h"
#include "cameracomponent.h"

IMPLEMENT_CLASS(CameraComponent)

CameraComponent::CameraComponent() : camera_mesh(nullptr), camera_speed(0.05f){
	camera_data = CREATE_CLASS(CameraData);
}
CameraComponent::~CameraComponent() {}

void CameraComponent::draw(const std::string& shader) { shader; }

CMatrix4x4 CameraComponent::get_view_mat() {
	camera_data->update(get_location(), get_rotation());
	return camera_data->get_view_mat();
}

CVector3D CameraComponent::get_right_axis() {
	camera_data->update(get_location(), get_rotation());
	return camera_data->get_right();
}
