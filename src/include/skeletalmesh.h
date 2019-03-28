#pragma once

// skeletal mesh

#include "mesh.h"

PRE_DECLARE_CLASS(Skeleton)
USING_SPTR(Skeleton)

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
	//void set_bones(const std::vector<Bone>& b);
	virtual ~SkeletalMesh() override {}

	virtual void draw(const std::string& shader) override;

	Bone& get_bone(int id);
	void update_bone_mat(int id, const CMatrix4x4& mat);

	GET_SET(SPTR_Skeleton, skeleton)
	GET_SET(std::vector<Bone>, bones)
	GET_SET(CMatrix4x4, mat_global)


private:

	// 骨骼相关
	SPTR_Skeleton skeleton;
	std::vector<Bone> bones;						// 所有的真正的骨骼节点信息 -- 与层次节点一对一，但是并非所有的层次节点都有骨骼
	CMatrix4x4 mat_global;							// 根骨骼的逆变换矩阵

};

