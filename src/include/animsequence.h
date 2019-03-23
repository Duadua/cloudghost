#pragma once

#include "reflex.h"
#include "skeleton.h"
#include <map>

struct AnimNode {

};

DECLARE_AUTO_PTR(AnimSequence)
class AnimSequence : public CObject {
	DECLARE_CLASS(AnimSequence)

public:
	AnimSequence() {}
	~AnimSequence() {}

	void update(float cur_time);						// 更新动画序列

private:

	std::map<std::string, int> map_anim_nodes;
	std::vector<AnimNode> anim_nodes;

	SPTR_Skeleton skeleton;								// 动画序列依赖于某个骨骼 -- 又叕 like ue4 hhhhhhh
};
