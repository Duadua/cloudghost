#pragma once

#include "scenecomponent.h"
#include <string>

PRE_DECLARE_CLASS(Mesh)
USING_SPTR(Mesh)

class MeshComponent : public SceneComponent {
	DECLARE_CLASS(MeshComponent)
public:
	MeshComponent();
    virtual ~MeshComponent() override;

	virtual void draw(const std::string& shader) override;
	
	GET_SET(SPTR_Mesh, mesh)

private:
	SPTR_Mesh mesh;

};
DECLARE_AUTO_PTR(MeshComponent)
