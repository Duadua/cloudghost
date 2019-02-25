#pragma once

#include "cmath.h"
#include "reflex.h"
#include <QMatrix4x4>

class CameraData : public CObject{
	DECLARE_CLASS(CameraData)
public:
	CameraData();
	~CameraData();

	QMatrix4x4 get_view_mat();

	void update(CVector3D location, CVector3D rotation);

private:

	QMatrix4x4 look_at();

	CVector3D location;
	CVector3D rotation;
	
	CVector3D front;
	CVector3D right;
	CVector3D up;

	CVector3D world_up;

};
DECLARE_AUTO_PTR(CameraData)

