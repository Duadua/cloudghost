#pragma once

#include "cmath.h"
#include "component.h"
#include <QMatrix4x4>

DECLARE_AUTO_PTR(SceneComponent)
class SceneComponent : public Component, public std::enable_shared_from_this<SceneComponent> {
	DECLARE_CLASS(SceneComponent)
public:
	SceneComponent();
	virtual ~SceneComponent();

	virtual void draw(const std::string& shader);

	void attach_to(SPTR_SceneComponent parent);
	
	QMatrix4x4 get_transform();

	void set_location(CVector3D l);
	void set_roataion(CVector3D r);
	void set_scale(CVector3D s);

	void set_location(float x, float y, float z);
	void set_roataion(float x, float y, float z);
	void set_scale(float x, float y, float z);

	CVector3D get_location();
	CVector3D get_rotation();
	CVector3D get_scale();

protected:
	WPTR_SceneComponent parent_component;
	std::vector<SPTR_SceneComponent> child_components;

	void add_child(SPTR_SceneComponent child);

protected:
	// transform
	CVector3D location;
	CVector3D rotation;
	CVector3D scale;
};