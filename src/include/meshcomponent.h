#pragma once

#include "scenecomponent.h"

class Mesh;

class MeshComponent : public SceneComponent {
public:
	MeshComponent(Mesh* m = nullptr);
	virtual ~MeshComponent();

	virtual void draw(Shader& shader);
	void set_mesh(Mesh* m);

private:
	Mesh* mesh;
};