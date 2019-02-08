#pragma once

#include "shader.h"
#include "component.h"
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

class SceneComponent : public Component {
	DECLARE_CLASS(SceneComponent)
public:
	SceneComponent();
	virtual ~SceneComponent();

	virtual void draw(SPTR_Shader shader);

	void attach_to(SceneComponent* parent);
	
	QMatrix4x4 get_transform();

	void set_location(QVector3D l);
	void set_roataion(QVector3D r);
	void set_scale(QVector3D s);

	QVector3D get_location();
	QVector3D get_rotation();
	QVector3D get_scale();

protected:
	SceneComponent * parent_component;
	QVector<SceneComponent*> child_components;

	void add_child(SceneComponent* child);

protected:
	// transform
	QVector3D location;
	QVector3D rotation;
	QVector3D scale;
};
DECLARE_AUTO_PTR(SceneComponent)