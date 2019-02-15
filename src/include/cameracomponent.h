#pragma once

#include "mesh.h"
#include "cameradata.h"
#include "scenecomponent.h"

class CameraComponent : public SceneComponent {
	DECLARE_CLASS(CameraComponent)
public:
	CameraComponent();
	~CameraComponent();
	
	virtual void draw(const std::string& shader) override;
 
	QMatrix4x4 get_view_mat();

	GET_SET(float, camera_speed);

private:
	std::string camera_mesh;

	SPTR_CameraData camera_data;

	float camera_speed;
};
DECLARE_AUTO_PTR(CameraComponent)


