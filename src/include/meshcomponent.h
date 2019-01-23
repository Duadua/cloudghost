#pragma once

#include "scenecomponent.h"

class Mesh;

class MeshComponent : public SceneComponent {
public:
	MeshComponent();
	~MeshComponent();

private:
	Mesh* mesh;
};