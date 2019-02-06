#include "mesh.h"
#include "cameradata.h"
#include "cameracomponent.h"

IMPLEMENT_CLASS(CameraComponent)

CameraComponent::CameraComponent() : camera_mesh(nullptr), camera_speed(0.05f){
	camera_data = new CameraData();
}
CameraComponent::~CameraComponent() {
	delete camera_data;
}

void CameraComponent::draw(Shader& shader) {

}

QMatrix4x4 CameraComponent::get_view_mat() {
	camera_data->update(get_location(), get_rotation());
	return camera_data->get_view_mat();
}

void CameraComponent::set_camera_speed(float s) { camera_speed = s; }
float CameraComponent::get_camera_speed() { return camera_speed; }
