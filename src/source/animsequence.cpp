#include "animsequence.h"

AnimNode::AnimNode(const AnimNode& b) : id(b.id), name(b.name) {
	position_keys.clear(); position_keys.assign(b.position_keys.begin(), b.position_keys.end());
	rotation_keys.clear(); rotation_keys.assign(b.rotation_keys.begin(), b.rotation_keys.end());
	scale_keys.clear(); scale_keys.assign(b.scale_keys.begin(), b.scale_keys.end());
}

// ===============================================================================================

IMPLEMENT_CLASS(AnimSequence)

void AnimSequence::update(float cur_time) {

}

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



