#pragma once

#include "scenecomponent.h"
#include "cmath.h"

// base light
class LightComponent : public SceneComponent {
	DECLARE_CLASS(LightComponent)
public:
	LightComponent();
	virtual ~LightComponent() {}

	virtual bool use(const std::string& shader) { return true; }

	GET_SET(CVector3D, position);
	GET_SET(CVector3D, dirction);
	GET_SET(float, intensity);

protected:
	CVector3D position;
	CVector3D dirction;
	float intensity;

private:

};
DECLARE_AUTO_PTR(LightComponent)

// direct light
class DirectLightComponent : public LightComponent{
	DECLARE_CLASS(DirectLightComponent)
public:
	DirectLightComponent();
	virtual ~DirectLightComponent() {}

	virtual bool use(const std::string& shader) override;
private:

};
DECLARE_AUTO_PTR(DirectLightComponent)

// point light
class PointLightComponent : public LightComponent{
	DECLARE_CLASS(PointLightComponent)

};
DECLARE_AUTO_PTR(PointLightComponent)

// spot light
class SpotLightComponent : public LightComponent{
	DECLARE_CLASS(SpotLightComponent)

};
DECLARE_AUTO_PTR(SpotLightComponent)

// sky light
class SkyLightComponent : public LightComponent{
	DECLARE_CLASS(SkyLightComponent)

};
DECLARE_AUTO_PTR(SkyLightComponent)


