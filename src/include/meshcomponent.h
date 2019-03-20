#pragma once

#include "mesh.h"
#include "scenecomponent.h"
#include <string>

class MeshComponent : public SceneComponent {
	DECLARE_CLASS(MeshComponent)
public:
	MeshComponent();
    virtual ~MeshComponent() override;

	virtual void draw(const std::string& shader) override;
	
	void set_mesh(const std::string& name);
	SPTR_Mesh get_mesh() { return mesh; }

private:
	SPTR_Mesh mesh;

};
DECLARE_AUTO_PTR(MeshComponent)
