#pragma once

#include "scenecomponent.h"
#include <string>

PRE_DECLARE_CLASS(SkeletalMesh)
USING_SPTR(SkeletalMesh)

class SkeletalMeshComponent : public SceneComponent {
	DECLARE_CLASS(SkeletalMeshComponent)
public:
	SkeletalMeshComponent();
	virtual ~SkeletalMeshComponent() override;

	virtual void draw(const std::string& shader) override;

	GET_SET(SPTR_SkeletalMesh, mesh)

private:
	SPTR_SkeletalMesh mesh;

};
DECLARE_AUTO_PTR(SkeletalMeshComponent)
