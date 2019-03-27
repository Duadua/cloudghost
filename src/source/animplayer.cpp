#include "skeleton.h"
#include "animplayer.h"
#include "animsequence.h"
#include "skeletalmesh.h"

#include "cdebuger.h"

void AnimPlayer::play_anim_sequence(SPTR_AnimSequence anim, SPTR_SkeletalMesh mesh, float time) {
	if (!anim || !mesh) return;
	if (anim->get_name().compare(mesh->get_skeleton()->get_name()) != 0) { return; }

	c_debuger() << "play anim yep";
}
