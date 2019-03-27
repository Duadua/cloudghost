#pragma once

#include "reflex.h"
#include "singleton.h"

PRE_DECLARE_CLASS(AnimSequence)
PRE_DECLARE_CLASS(SkeletalMesh)
USING_SPTR(AnimSequence)
USING_SPTR(SkeletalMesh)

SINGLETON_CLASS(AnimPlayer) {
	SINGLETON(AnimPlayer)
public:
	void play_anim_sequence(SPTR_AnimSequence, SPTR_SkeletalMesh, float time);

private:
};
SINGLETON_X(AnimPlayer)