#pragma once

#include "component.h"
#include <QVector>

class SceneComponent : public Component {
public:
	SceneComponent();
	virtual ~SceneComponent();

	virtual void draw();

	void attach_to(SceneComponent* parent);
	
protected:
	QVector<SceneComponent*> child_components;

	void add_child(SceneComponent* child);
};
