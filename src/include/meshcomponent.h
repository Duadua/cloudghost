#pragma once

#include "scenecomponent.h"
#include <string>

PRE_DECLARE_CLASS(Mesh)
USING_SPTR(Mesh)
PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)

class MeshComponent : public SceneComponent {
	DECLARE_CLASS(MeshComponent)
public:
	MeshComponent();
    virtual ~MeshComponent() override;

	virtual void tick(float time) override;
	virtual void draw(SPTR_Shader shader) override;
	
	GET_SET(SPTR_Mesh, mesh)

private:
	SPTR_Mesh mesh;

};
DECLARE_AUTO_PTR(MeshComponent)
