#pragma once

#include "cmath.h"
#include "reflex.h"

class CameraData : public CObject{
	DECLARE_CLASS(CameraData)
public:
	CameraData();
	~CameraData();

	CMatrix4x4 get_view_mat();

	void update(CVector3D location, CVector3D rotation);
	
	GET(CVector3D, right);

private:

	CMatrix4x4 look_at();			

	CVector3D location;
	CVector3D rotation;
	
	CVector3D front;
	CVector3D right;
	CVector3D up;

	CVector3D world_up;

};
DECLARE_AUTO_PTR(CameraData)

