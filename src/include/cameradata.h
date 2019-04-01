#pragma once

#include "cmath.h"
#include "reflex.h"

struct Frustum {
	float fov;				// 视野角度 -- 度
	float near;
	float far;
	float width;			// near's width
	float heigh;
	Frustum(float fo = 45.0f, float n = 0.1f, float f = 1000.0f, float w = 800.0f, float h = 600.0f) 
		: fov(fo), near(n), far(f), width(w), heigh(h) {}

};	// 平截头体

class CameraData : public CObject{
	DECLARE_CLASS(CameraData)
public:
	CameraData();
	~CameraData();

	CMatrix4x4 get_view_mat();
	CMatrix4x4 get_proj_mat();

	void update(CVector3D location, CVector3D rotation);
	
	Frustum& get_frustum() { return frustum; }

	GET(CVector3D, right)

private:

	CMatrix4x4 look_at();			

	CVector3D location;
	CVector3D rotation;
	
	CVector3D front;
	CVector3D right;
	CVector3D up;

	CVector3D world_up;

	Frustum frustum;

};
DECLARE_AUTO_PTR(CameraData)

