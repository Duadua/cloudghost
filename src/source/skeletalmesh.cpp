#include "skeletalmesh.h"

IMPLEMENT_CLASS(SkeletalMesh)

SkeletalMesh::SkeletalMesh() { skeleton = nullptr; }
SkeletalMesh::SkeletalMesh(const SkeletalMesh& b) : Mesh(b) { skeleton = b.skeleton; }
void SkeletalMesh::copy_from(const SPTR_SkeletalMesh b) { Mesh::copy_from(b); skeleton = b->skeleton; }

void SkeletalMesh::draw(const std::string& shader) {

}