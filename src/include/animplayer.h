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

	void play(float time);													// 播放中
	void start();															// 开始播放
	void stop();															// 停止 -- 再次播放时从 init_position 开始
	void pause();															// 暂停
	void go_on();															// 继续 -- 从暂停的地方开始

	void init(SPTR_AnimSequence a, SPTR_SkeletalMesh m);

	void update_bones(float frame);											// 更新当前播放位置的骨骼数据

	GET_SET(bool, is_looping)
	GET(float, init_position)
	void AnimPlayer::set_init_position(float i);

private:
	
	SPTR_AnimSequence anim;
	SPTR_SkeletalMesh mesh;

	bool is_played;				// 是否已经播放过了
	bool is_paused;				// 是否曾经暂停过 -- 用于继续播放时更新 cur_time
	bool is_can_play;			// 是否可以播放			 
	bool is_looping;			// 是否循环播放

	float init_position;		// 初始位置 -- [0 ,, 1]
	float cur_position;			// 当前播放的位置 -- [0 ,, 1]

	float cur_time;				// 当前时间
	float cur_frame;			// = duration * cur_position

	bool check_anim();


};