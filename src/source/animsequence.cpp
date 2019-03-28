#include "animsequence.h"
#include <cassert>
#include "cdebuger.h"

AnimNode::AnimNode(const AnimNode& b) : id(b.id), name(b.name) {
	position_keys.clear(); position_keys.assign(b.position_keys.begin(), b.position_keys.end());
	rotation_keys.clear(); rotation_keys.assign(b.rotation_keys.begin(), b.rotation_keys.end());
	scale_keys.clear(); scale_keys.assign(b.scale_keys.begin(), b.scale_keys.end());
}

CVector3D AnimNode::get_trans(float time) const {
	assert(position_keys.size() > 0);
	if (position_keys.size() < 2) { return position_keys[0].value; }

	int key_len = static_cast<int>(position_keys.size());

	int t_id = -1;
	for (int i = 0; i < key_len; ++i) {
		if (CMath_ins().fcmp(time, position_keys[i].time) < 0) { t_id = i; break; }
	} // 查找区间 -- 可改成二分
	if (t_id == -1 && CMath_ins().fcmp(time, position_keys.back().time) == 0) { t_id = key_len - 1; }
	assert(t_id >= 1 && t_id < key_len);

	// 计算 ratio
	float time_l = position_keys[t_id - 1].time;
	float time_r = position_keys[t_id].time;
	float ratio = (time - time_l) / (time_r - time_l);
	assert(CMath_ins().fcmp(ratio, 0.0f) >= 0 && CMath_ins().fcmp(ratio, 1.0f) <= 0);

	// 插值
	auto value_l = position_keys[t_id - 1].value;
	auto value_r = position_keys[t_id].value;
	auto res = CMath_ins().linear_lerp(value_l, value_r, ratio);
	
	return res;
}
CVector3D AnimNode::get_scale(float time) const {
	assert(scale_keys.size() > 0);
	if (scale_keys.size() < 2) { return scale_keys[0].value; }
	
	int key_len = static_cast<int>(scale_keys.size());

	int t_id = -1;
	for (int i = 0; i < key_len; ++i) {
		if (CMath_ins().fcmp(time, scale_keys[i].time) < 0) { t_id = i; break; }
	} // 查找区间 -- 可改成二分
	if (t_id == -1 && CMath_ins().fcmp(time, scale_keys.back().time) == 0) { t_id = key_len - 1; }
	assert(t_id >= 1 && t_id < key_len);

	// 计算 ratio
	float time_l = scale_keys[t_id - 1].time;
	float time_r = scale_keys[t_id].time;
	float ratio = (time - time_l) / (time_r - time_l);
	assert(CMath_ins().fcmp(ratio, 0.0f) >= 0 && CMath_ins().fcmp(ratio, 1.0f) <= 0);

	// 插值
	auto value_l = scale_keys[t_id - 1].value;
	auto value_r = scale_keys[t_id].value;
	auto res = CMath_ins().linear_lerp(value_l, value_r, ratio);

	return res;
}
CQuaternion AnimNode::get_rotat(float time) const {
	assert(rotation_keys.size() > 0);
	if (rotation_keys.size() < 2) { return rotation_keys[0].value; }

	int key_len = static_cast<int>(rotation_keys.size());

	int t_id = -1;
	for (int i = 0; i < key_len; ++i) {
		if (CMath_ins().fcmp(time, rotation_keys[i].time) < 0) { t_id = i; break; }
	} // 查找区间 -- 可改成二分
	if (t_id == -1 && CMath_ins().fcmp(time, rotation_keys.back().time) == 0) { t_id = key_len - 1; }
	assert(t_id >= 1 && t_id < key_len);

	// 计算 ratio
	float time_l = rotation_keys[t_id - 1].time;
	float time_r = rotation_keys[t_id].time;
	float ratio = (time - time_l) / (time_r - time_l);
	assert(CMath_ins().fcmp(ratio, 0.0f) >= 0 && CMath_ins().fcmp(ratio, 1.0f) <= 0);

	// 插值
	auto value_l = rotation_keys[t_id - 1].value;
	auto value_r = rotation_keys[t_id].value;
	auto res = slerp(value_l, value_r, ratio);

	return res;
}

// ===============================================================================================

IMPLEMENT_CLASS(AnimSequence)

void AnimSequence::init(const std::map<std::string, int>& m_n, const std::vector<AnimNode>& n) {
	clear();
	map_anim_nodes.insert(m_n.begin(), m_n.end());
	anim_nodes.assign(n.begin(), n.end());
}

void AnimSequence::add_node(const AnimNode& an) {
	if (map_anim_nodes.count(an.name)) { return; }
	int t_id = static_cast<int>(anim_nodes.size());

	map_anim_nodes[an.name] = t_id;
	anim_nodes.push_back(an);
	anim_nodes.back().id = t_id;
}
const AnimNode& AnimSequence::get_node(const std::string& key) {
	assert(map_anim_nodes.count(key));
	return get_node(map_anim_nodes[key]);
}
const AnimNode& AnimSequence::get_node(int id) {
	assert(id >= 0 && id < anim_nodes.size());
	return anim_nodes[id];
}

