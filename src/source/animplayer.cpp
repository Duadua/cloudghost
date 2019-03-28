#include "skeleton.h"
#include "animplayer.h"
#include "animsequence.h"
#include "skeletalmesh.h"
#include <queue>

#include "cdebuger.h"

IMPLEMENT_CLASS(AnimPlayer)

void AnimPlayer::init(SPTR_AnimSequence a, SPTR_SkeletalMesh m) {
	anim = a;
	mesh = m;

	is_played = false;
	is_can_play = true;
	is_looping = true;

	init_position = 0.0f;

	cur_time = 0.0f;
	cur_frame = 0.0f;
	cur_position = init_position;

}

void AnimPlayer::set_init_position(float i) {
	init_position = i; 
}
void AnimPlayer::play(float time) {
	if (!is_can_play) { return; }
	if (!check_anim()) { return; }

	if (!is_played) {
		is_played = true; 

		cur_time = time;
		cur_frame = anim->get_duration() * init_position;			// 当前的动画帧
		cur_position = cur_frame / anim->get_duration();
	}
	if (is_paused) {
		is_paused = false;
		cur_time = time;					// 继续地话要更新下时间 -- 否则把暂停的时间也算进去了 - 但不用更新cur_frame
	} 

	// update cur_frame
	float t_frame = 0.001f * (time - cur_time) * anim->get_ticks_per_seconds() + cur_frame;
	if (!is_looping && CMath_ins().fcmp(t_frame, anim->get_duration()) > 0) {
		is_can_play = false;
		return;
	} // 不能重复播放时 -- 到头即停
	
	cur_time = time;
	cur_frame = std::fmodf(t_frame, anim->get_duration());
	cur_position = cur_frame / anim->get_duration();

	// update bones
	update_bones(cur_frame);

}
void AnimPlayer::start() { is_can_play = true; is_paused = true; }
void AnimPlayer::stop() {
	is_can_play = false;
	is_played = false;

}
void AnimPlayer::pause() { is_can_play = false; }

void AnimPlayer::go_on() { 

}

void AnimPlayer::update_bones(float frame) {
	if (!check_anim()) return;

	auto t_skeleton = mesh->get_skeleton();
	if (!t_skeleton) return;

	std::queue<int> nodes;
	nodes.push(t_skeleton->get_root_node());
	std::queue<CMatrix4x4> mats;
	mats.push(CMatrix4x4());

	while (!nodes.empty()) {
		int t_id = nodes.front(); nodes.pop();
		auto t_node = t_skeleton->get_node(t_id);
		auto t_mat = mats.front(); mats.pop();

		auto cur_mat = t_node.mat_trans;		// 当前的变换矩阵 -- 如果
		// 当前结点有 anim_node 时 -- 重算 cur_mat
		if (anim->has_node(t_node.name)) {
			auto t_anim_node = anim->get_node(t_node.name);
			// 插值操作
			cur_mat.set_to_identity();

			auto v_trans = t_anim_node.get_trans(frame); // translate 插值
			auto v_rotat = t_anim_node.get_rotat(frame); // rotate 插值
			auto v_scale = t_anim_node.get_scale(frame); // scale 插值

			//cur_mat.scale(v_scale).rotate_quaternion(v_rotat).translate(v_trans);
			cur_mat.translate(v_trans).rotate_quaternion(v_rotat).scale(v_scale);
		}
		
		
		auto final_mat = t_mat * cur_mat;		// 最终的mat = 父节点的mat * 当前的mat

		// 修改mesh的骨骼 -- 如果有的话
		if (t_node.bone_id >= 0 && t_node.bone_id < mesh->get_bones().size()) {
			mesh->update_bone_mat(t_node.bone_id, final_mat);
		}

		// add child node
		for (auto i : t_node.children) {
			nodes.push(i);
			mats.push(final_mat);
		}
	}
}

bool AnimPlayer::check_anim() {
	if (!anim || !mesh) return false;
	if (anim->get_skeleton_name().compare(mesh->get_skeleton()->get_name()) != 0) { return false; }
	return true;
}




