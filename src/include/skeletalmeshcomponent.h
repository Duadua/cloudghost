#pragma once

#include "scenecomponent.h"
#include <string>

PRE_DECLARE_CLASS(Shader)
USING_SPTR(Shader)
PRE_DECLARE_CLASS(SkeletalMesh)
USING_SPTR(SkeletalMesh)
PRE_DECLARE_CLASS(AnimSequence)
USING_SPTR(AnimSequence)
PRE_DECLARE_CLASS(AnimPlayer)
USING_SPTR(AnimPlayer)

class SkeletalMeshComponent : public SceneComponent {
	DECLARE_CLASS(SkeletalMeshComponent)
public:
	SkeletalMeshComponent();
	virtual ~SkeletalMeshComponent() override;

	virtual void begin_play() override;
	virtual void tick(float time) override;
	virtual void draw(SPTR_Shader shader) override;

	GET_SET(SPTR_SkeletalMesh, mesh)
	GET_SET(SPTR_AnimSequence, anim)
	GET_SET(SPTR_AnimPlayer, anim_player)

private:
	SPTR_SkeletalMesh mesh;
	SPTR_AnimSequence anim;

	SPTR_AnimPlayer anim_player;

};
DECLARE_AUTO_PTR(SkeletalMeshComponent)
