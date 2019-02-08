#pragma once

#include "mesh.h"
#include "cameradata.h"
#include "scenecomponent.h"

class CameraComponent : public SceneComponent {
	DECLARE_CLASS(CameraComponent)
public:
	CameraComponent();
	~CameraComponent();
	
	virtual void draw(SPTR_Shader shader) override;
 
	QMatrix4x4 get_view_mat();

	void set_camera_speed(float s);
	float get_camera_speed();

private:
	SPTR_Mesh camera_mesh;

	SPTR_CameraData camera_data;

	float camera_speed;
};
DECLARE_AUTO_PTR(CameraComponent)


