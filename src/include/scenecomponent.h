#pragma once

#include "component.h"
#include <QVector>
#include <QVector3D>
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

	void set_location(QVector3D l);
	void set_roataion(QVector3D r);
	void set_scale(QVector3D s);

	QVector3D get_location();
	QVector3D get_rotation();
	QVector3D get_scale();

protected:
	WPTR_SceneComponent parent_component;
	QVector<SPTR_SceneComponent> child_components;

	void add_child(SPTR_SceneComponent child);

protected:
	// transform
	QVector3D location;
	QVector3D rotation;
	QVector3D scale;
};