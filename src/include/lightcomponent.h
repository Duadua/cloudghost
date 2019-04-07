#pragma once

#include "cmath.h"
#include "scenecomponent.h"

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)

const int direct_light_num_max = 1;
const int point_light_num_max = 16;
const int spot_light_num_max = 16;
const int sky_light_num_max = 1;

// base light
class LightComponent : public SceneComponent {
	DECLARE_CLASS(LightComponent)
public:
	LightComponent();
	virtual ~LightComponent() override {}

	virtual bool use(SPTR_Shader shader) { if (shader) { return true; } return false; }

	GET_SET(int , id)
	GET_SET(CVector3D, color)
	GET_SET(float, intensity)
	GET_SET(CVector3D, k)
	GET_SET(CMatrix4x4, mat_proj_view)
	CVector3D get_dirction();					// from rotation  -- 初始方向 (0.0, 0.0, 1.0)

protected:
	int id;
	CVector3D color;
	float intensity;
	CVector3D k;								// 各分量占比 -- ambient / diffuse / specular

	CMatrix4x4 mat_proj_view;					// 此光源的 T 矩阵 -- 阴影用
private:

};
DECLARE_AUTO_PTR(LightComponent)

// =======================================================

// direct light
class DirectLightComponent : public LightComponent{
	DECLARE_CLASS(DirectLightComponent)
public:
	DirectLightComponent();
    virtual ~DirectLightComponent() override {}

	virtual bool use(SPTR_Shader shader) override;

private:

};
DECLARE_AUTO_PTR(DirectLightComponent)

// =======================================================

// point light
class PointLightComponent : public LightComponent{
	DECLARE_CLASS(PointLightComponent)
public:
	PointLightComponent ();
    virtual ~PointLightComponent() override {}

	virtual bool use(SPTR_Shader shader) override;
	
    GET(float, att_raduis)
	void set_att_radius(float t_att_radius);
	void update_att();			// 由衰减半径更新 参数

private:

	float att_raduis;			// 衰减半径

	// attenuation
	float att_ka;
	float att_kb;
	float att_kc;
	

};
DECLARE_AUTO_PTR(PointLightComponent)

// =======================================================

// spot light
class SpotLightComponent : public LightComponent{
	DECLARE_CLASS(SpotLightComponent)
public:
	SpotLightComponent ();
    virtual ~SpotLightComponent() override {}

	virtual bool use(SPTR_Shader shader) override;
	
    GET(float, att_raduis)
	void set_att_radius(float t_att_radius);
	void update_att();			// 由衰减半径更新 参数

    GET_SET(float, inner)
    GET_SET(float, outer)

private:

	float att_raduis;			// 衰减半径

	// attenuation
	float att_ka;
	float att_kb;
	float att_kc;

	// cut off angle -- 角度
	float inner;
	float outer;
	
};
DECLARE_AUTO_PTR(SpotLightComponent)

// =======================================================

// sky light
class SkyLightComponent : public LightComponent{
	DECLARE_CLASS(SkyLightComponent)

};
DECLARE_AUTO_PTR(SkyLightComponent)


