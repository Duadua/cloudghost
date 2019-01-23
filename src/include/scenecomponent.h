#pragma once

#include "component.h"
#include <QVector>

class SceneComponent : public Component {
public:
	SceneComponent();
	~SceneComponent();
protected:
	QVector<SceneComponent*> child_components;
};
