#pragma once

#include "scenecomponent.h"

PRE_DECLARE_CLASS(Mesh)
PRE_DECLARE_CLASS(CameraData)
USING_SPTR(Mesh)
USING_SPTR(CameraData)

class CameraComponent : public SceneComponent {
	DECLARE_CLASS(CameraComponent)
public:
	CameraComponent();
    ~CameraComponent() override;
	
    virtual void draw(const std::string& shader) override;
 
	CMatrix4x4 get_view_mat();

    GET_SET(float, camera_speed)

	CVector3D get_right_axis();

private:

	SPTR_Mesh camera_mesh;

	SPTR_CameraData camera_data;

	float camera_speed;
};
DECLARE_AUTO_PTR(CameraComponent)


