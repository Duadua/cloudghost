#pragma once

// skeletal mesh

#include "mesh.h"
#include "skeleton.h"

DECLARE_AUTO_PTR(SkeletalMesh)
class SkeletalMesh : public Mesh {
	DECLARE_CLASS(SkeletalMesh)

public:
	SkeletalMesh();
	SkeletalMesh(const SkeletalMesh& b);
	void copy_from(const SPTR_SkeletalMesh b);
	virtual ~SkeletalMesh() override {}

	virtual void draw(const std::string& shader) override;

	GET_SET(SPTR_Skeleton, skeleton)

private:
	SPTR_Skeleton skeleton;
};

