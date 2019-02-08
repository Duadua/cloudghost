#pragma once

#include "mesh.h"
#include "scenecomponent.h"

class MeshComponent : public SceneComponent {
	DECLARE_CLASS(MeshComponent)
public:
	MeshComponent(SPTR_Mesh m = nullptr);
	virtual ~MeshComponent();

	virtual void draw(SPTR_Shader shader) override;
	void set_mesh(SPTR_Mesh m);

private:
	SPTR_Mesh mesh;
};
DECLARE_AUTO_PTR(MeshComponent)