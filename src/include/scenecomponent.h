#pragma once

#include "shader.h"
#include "component.h"
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

class SceneComponent : public Component {
public:
	SceneComponent();
	virtual ~SceneComponent();

	virtual void draw(Shader& shader);

	void attach_to(SceneComponent* parent);
	
	// transform
	QVector3D location;
	QVector3D rotation;
	QVector3D scale;

	QMatrix4x4 get_transform();


protected:
	SceneComponent * parent_component;
	QVector<SceneComponent*> child_components;

	void add_child(SceneComponent* child);
};
