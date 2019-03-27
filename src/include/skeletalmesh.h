#pragma once

// skeletal mesh

#include "mesh.h"

PRE_DECLARE_CLASS(Skeleton)
USING_SPTR(Skeleton)
PRE_DECLARE_CLASS(AnimSequence)
USING_SPTR(AnimSequence)

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
	GET_SET(CMatrix4x4, mat_global)
	GET(SPTR_AnimSequence, animation)

	bool set_animation(SPTR_AnimSequence anim);

private:

	// 骨骼动画相关
	SPTR_Skeleton skeleton;
	std::vector<Bone> bones;						// 所有的真正的骨骼节点信息 -- 与层次节点一对一，但是并非所有的层次节点都有骨骼
	CMatrix4x4 mat_global;							// 根骨骼的逆变换矩阵

	SPTR_AnimSequence animation;					// 动画序列 -- 依赖于 skeleton

};

