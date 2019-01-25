#include "mesh.h"
#include "cameradata.h"
#include "cameracomponent.h"

CameraComponent::CameraComponent() : camera_mesh(nullptr){
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
