#pragma once

#include "scenecomponent.h"
#include "cmath.h"

// base light
class LightComponent : public SceneComponent {
	DECLARE_CLASS(LightComponent)
public:
	LightComponent();
	virtual ~LightComponent() {}

	virtual bool use(const std::string& shader) { return false; }

	GET_SET(CVector3D, color);
	GET_SET(float, intensity);


protected:
	CVector3D color;
	float intensity;

private:

};
DECLARE_AUTO_PTR(LightComponent)

// =======================================================

// direct light
class DirectLightComponent : public LightComponent{
	DECLARE_CLASS(DirectLightComponent)
public:
	DirectLightComponent();
	virtual ~DirectLightComponent() {}

	virtual bool use(const std::string& shader) override;

private:
	
	CVector3D get_dirction();					// from rotation  -- 初始方向 (0.0, 0.0, 1.0)

	static uint direct_light_num;

};
DECLARE_AUTO_PTR(DirectLightComponent)

// =======================================================

// point light
class PointLightComponent : public LightComponent{
	DECLARE_CLASS(PointLightComponent)
public:
	PointLightComponent ();
	virtual ~PointLightComponent() {}

	virtual bool use(const std::string& shader) override;
	
	GET(float, att_raduis);
	void set_att_radius(float t_att_radius);
	void update_att();			// 由衰减半径更新 参数

private:

	float att_raduis;			// 衰减半径

	// attenuation
	float att_ka;
	float att_kb;
	float att_kc;
	
	static uint point_light_num;


};
DECLARE_AUTO_PTR(PointLightComponent)

// =======================================================

// spot light
class SpotLightComponent : public LightComponent{
	DECLARE_CLASS(SpotLightComponent)

};
DECLARE_AUTO_PTR(SpotLightComponent)

// =======================================================

// sky light
class SkyLightComponent : public LightComponent{
	DECLARE_CLASS(SkyLightComponent)

};
DECLARE_AUTO_PTR(SkyLightComponent)


