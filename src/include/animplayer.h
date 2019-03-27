#pragma once

#include "reflex.h"

PRE_DECLARE_CLASS(AnimSequence)
PRE_DECLARE_CLASS(SkeletalMesh)
USING_SPTR(AnimSequence)
USING_SPTR(SkeletalMesh)

DECLARE_AUTO_PTR(AnimPlayer)
class AnimPlayer : public CObject {
	DECLARE_CLASS(AnimPlayer)
public:

	void play(SPTR_AnimSequence anim, SPTR_SkeletalMesh mesh, float time);	// 播放
	void stop();															// 停止 -- 再次播放时从 init_position 开始
	void pause();															// 暂停
	void go_on();															// 继续 -- 从暂停的地方开始

	void init();

private:
	
	SPTR_AnimSequence anim;
	SPTR_SkeletalMesh mesh;

	bool is_played;				// 是否已经播放了
	bool is_can_play;			// 是否可以播放			 
	bool is_looping;			// 是否循环播放

	float init_position;		// 初始位置 -- [0 ,, 1]

};