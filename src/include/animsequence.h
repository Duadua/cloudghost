#pragma once

#include "reflex.h"
#include "cmath.h"
#include <map>

struct AnimPositionKey {
	float time;
	CVector3D value;
	AnimPositionKey() {}
	AnimPositionKey(float t, const CVector3D& v) : time(t), value(v) {}
};
struct AnimRotationKey {
	float time;
	CQuaternion value;
	AnimRotationKey() {}
	AnimRotationKey(float t, const CQuaternion& v) : time(t), value(v) {}
};
struct AnimScaleKey {
	float time;
	CVector3D value;
	AnimScaleKey() {}
	AnimScaleKey(float t, const CVector3D& v) : time(t), value(v) {}
};

struct AnimNode {
	int id;
	std::string name;								// node name 与 骨骼层次里的 node 名称对应
	std::vector<AnimPositionKey> position_keys;
	std::vector<AnimRotationKey> rotation_keys;
	std::vector<AnimScaleKey> scale_keys;

	AnimNode() {}
	AnimNode(const AnimNode& b);

	CVector3D	get_trans(float time) const;	// time = [0, duration]
	CVector3D	get_scale(float time) const;	// time = [0, duration]
	CQuaternion	get_rotat(float time) const;	// time = [0, duration]

};

DECLARE_AUTO_PTR(AnimSequence)
class AnimSequence : public CObject {
	DECLARE_CLASS(AnimSequence)

public:
	AnimSequence() { clear(); }
	void clear() { map_anim_nodes.clear(); anim_nodes.clear(); }
	void init(const std::map<std::string, int>& m_n, const std::vector<AnimNode>& n);
	~AnimSequence() {}

	void add_node(const AnimNode& an);
	const AnimNode& get_node(const std::string& key);
	const AnimNode& get_node(int id);
	bool has_node(const std::string& key) { return map_anim_nodes.count(key); }

	GET_SET(std::string, name)
	GET_SET(float, duration)
	GET_SET(float, ticks_per_seconds)
	GET_SET(std::string, skeleton_name)

private:

	std::map<std::string, int> map_anim_nodes;
	std::vector<AnimNode> anim_nodes;

	std::string name;
	float duration;									// 持续时间 -- 总共的帧数
	float ticks_per_seconds;						// 每秒多少帧 -- 1000.0 / ticks_per_seconds 为每帧多少毫秒

	std::string skeleton_name;						// 动画序列依赖于某个骨骼 -- 又叕 like ue4 hhhhhhh

};
