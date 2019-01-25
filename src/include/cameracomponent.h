#pragma once

#include "scenecomponent.h"

class Mesh;
class CameraData;

class CameraComponent : public SceneComponent {

public:
	CameraComponent();
	~CameraComponent();
	
	virtual void draw(Shader& shader) override;
 
	QMatrix4x4 get_view_mat();

private:
	Mesh * camera_mesh;

	CameraData* camera_data;
};


