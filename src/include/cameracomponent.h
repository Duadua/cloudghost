#pragma once

#include "scenecomponent.h"

class Mesh;
class CameraData;

class CameraComponent : public SceneComponent {
	DECLARE_CLASS(CameraComponent)
public:
	CameraComponent();
	~CameraComponent();
	
	virtual void draw(Shader& shader) override;
 
	QMatrix4x4 get_view_mat();

	void set_camera_speed(float s);
	float get_camera_speed();

private:
	Mesh * camera_mesh;

	CameraData* camera_data;

	float camera_speed;
};
DECLARE_AUTO_PTR(CameraComponent)


