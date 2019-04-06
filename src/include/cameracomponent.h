#pragma once

#include "scenecomponent.h"

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)
PRE_DECLARE_CLASS(Mesh)
USING_SPTR(Mesh)
PRE_DECLARE_CLASS(CameraData)
USING_SPTR(CameraData)

class CameraComponent : public SceneComponent {
	DECLARE_CLASS(CameraComponent)
public:
	CameraComponent();
    ~CameraComponent() override;
	
    virtual void draw(SPTR_Shader shader) override;
 
	CMatrix4x4 get_view_mat();
	CMatrix4x4 get_proj_mat();

	GET_SET(float, camera_speed)
	GET_SET(SPTR_CameraData, camera_data)

	CVector3D get_right_axis();
	CVector3D get_front_axis();
	CVector3D get_world_up();

private:

	SPTR_Mesh camera_mesh;

	SPTR_CameraData camera_data;

	float camera_speed;
};
DECLARE_AUTO_PTR(CameraComponent)


