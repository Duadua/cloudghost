#pragma once

#include "cmath.h"
#include "component.h"

DECLARE_AUTO_PTR(SceneComponent)
class SceneComponent : public Component, public std::enable_shared_from_this<SceneComponent> {
	DECLARE_CLASS(SceneComponent)
public:
	SceneComponent();
	virtual ~SceneComponent();

	virtual void draw(const std::string& shader);

	void attach_to(SPTR_SceneComponent parent);
	
	CMatrix4x4 get_transform();

    SET(CVector3D, location)
    SET(CVector3D, rotation)
    SET(CVector3D, scale)

	void set_location(float x, float y, float z);
	void set_rotation(float x, float y, float z);
	void set_scale(float x, float y, float z);

	CVector3D get_location();
	CVector3D get_rotation();
	CVector3D get_scale();

    GET_SET(bool, is_border)
	void set_all_border(bool border);			// 设置 此 MC 和 其子MC 都为 border

    GET(std::vector<SPTR_SceneComponent>, child_components)

protected:
	WPTR_SceneComponent parent_component;
	std::vector<SPTR_SceneComponent> child_components;

	void add_child(SPTR_SceneComponent child);

	bool is_border;					// 是否为边界 -- 描边用

protected:
	// transform
	CVector3D location;
	CVector3D rotation;
	CVector3D scale;
};
