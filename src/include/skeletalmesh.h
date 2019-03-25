#pragma once

// skeletal mesh

#include "mesh.h"
#include "skeleton.h"
#include "animsequence.h"


struct Bone {
	CMatrix4x4 mat_offset;
	CMatrix4x4 mat_finall;
	Bone() { mat_offset = CMatrix4x4(); mat_finall = CMatrix4x4(); }
};

DECLARE_AUTO_PTR(SkeletalMesh)
class SkeletalMesh : public Mesh {
	DECLARE_CLASS(SkeletalMesh)

public:
	SkeletalMesh();
	SkeletalMesh(const SkeletalMesh& b);
	void copy_from(const SPTR_SkeletalMesh b);
	void set_bones(const std::vector<Bone>& b);
	virtual ~SkeletalMesh() override {}

	virtual void draw(const std::string& shader) override;

	GET_SET(SPTR_Skeleton, skeleton)
	GET(SPTR_AnimSequence, animation)

	bool set_animation(SPTR_AnimSequence anim);

private:

	SPTR_Skeleton skeleton;
	std::vector<Bone> bones;						// 所有的真正的骨骼节点信息 -- 与层次节点一对一，但是并非所有的层次节点都有骨骼

	SPTR_AnimSequence animation;					// 动画序列 -- 依赖于 skeleton

};

