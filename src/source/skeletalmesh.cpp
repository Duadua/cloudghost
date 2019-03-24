#include "skeletalmesh.h"

IMPLEMENT_CLASS(SkeletalMesh)

SkeletalMesh::SkeletalMesh() { skeleton = nullptr; bones.clear(); }
SkeletalMesh::SkeletalMesh(const SkeletalMesh& b) : Mesh(b) { skeleton = b.skeleton; bones.assign(b.bones.begin(), b.bones.end()); }
void SkeletalMesh::copy_from(const SPTR_SkeletalMesh b) { Mesh::copy_from(b); skeleton = b->skeleton; bones.assign(b->bones.begin(), b->bones.end()); }

void SkeletalMesh::set_bones(const std::vector<Bone>& b) { bones.assign(b.begin(), b.end()); }

void SkeletalMesh::draw(const std::string& shader) {

}