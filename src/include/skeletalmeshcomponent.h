#pragma once

#include "scenecomponent.h"
#include <string>

PRE_DECLARE_CLASS(SkeletalMesh)
USING_SPTR(SkeletalMesh)
PRE_DECLARE_CLASS(AnimSequence)
USING_SPTR(AnimSequence)


class SkeletalMeshComponent : public SceneComponent {
	DECLARE_CLASS(SkeletalMeshComponent)
public:
	SkeletalMeshComponent();
	virtual ~SkeletalMeshComponent() override;

	virtual void draw(const std::string& shader) override;

	GET_SET(SPTR_SkeletalMesh, mesh)
	GET_SET(SPTR_AnimSequence, anim)

private:
	SPTR_SkeletalMesh mesh;
	SPTR_AnimSequence anim;

};
DECLARE_AUTO_PTR(SkeletalMeshComponent)
