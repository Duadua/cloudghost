#include "skeleton.h"
#include "animplayer.h"
#include "animsequence.h"
#include "skeletalmesh.h"

#include "cdebuger.h"

IMPLEMENT_CLASS(AnimPlayer)

void AnimPlayer::init() {
	anim = nullptr;
	mesh = nullptr;

	is_played = false;
	is_can_play = true;
	is_looping = true;

	init_position = 0.0f;

}

void AnimPlayer::play(SPTR_AnimSequence a, SPTR_SkeletalMesh m, float time) {
	if (!a || !m) return;
	if (a->get_skeleton_name().compare(m->get_skeleton()->get_name()) != 0) { return; }
	if (!is_played) {
		is_played = true; 
		anim = a; mesh = m; 
	}


}
void AnimPlayer::stop() {

}
void AnimPlayer::pause() {

}
void AnimPlayer::go_on() { 

}
