#pragma once

#include "cmath.h"
#include "component.h"

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)

DECLARE_AUTO_PTR(SceneComponent)
class SceneComponent : public Component, public std::enable_shared_from_this<SceneComponent> {
	DECLARE_CLASS(SceneComponent)
public:
	SceneComponent();
	virtual ~SceneComponent() override;

	virtual void begin_play() {}
	virtual void tick(float time) { time; }
	virtual void draw(SPTR_Shader shader) { shader; }

	void _begin_play();
	void _tick(float time);
	void _draw(SPTR_Shader shader);

	void attach_to(SPTR_SceneComponent parent);
	
	CMatrix4x4 get_transform();

    SET(CVector3D, location)
    SET(CVector3D, rotation)
    SET(CVector3D, scale)

	void set_location(float x, float y, float z);
	void set_rotation(float x, float y, float z);
	void set_scale(float x, float y, float z);

	void add_location(CVector3D l);
	void add_rotation(CVector3D	r);
	void add_scale(CVector3D s);
	void add_location(float x, float y, float z);
	void add_rotation(float x, float y, float z);
	void add_scale(float x, float y, float z);

	CVector3D get_location();
	CVector3D get_rotation();
	CVector3D get_scale();

	CVector3D get_front_axis();
	CVector3D get_right_axis();
	CVector3D get_up_axis();
	CVector3D get_world_up();

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

	// axis
	void update_rotation();			// to update axis and world_up
	CVector3D front_axis;
	CVector3D right_axis;
	CVector3D up_axis;
	CVector3D world_up;

};
