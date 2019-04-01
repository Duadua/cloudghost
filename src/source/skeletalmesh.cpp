#include "shader.h"
#include "skeleton.h"
#include "material.h"
#include "renderdata.h"
#include "skeletalmesh.h"
#include "assetmanager.h"
#include <cassert>

IMPLEMENT_CLASS(SkeletalMesh)

SkeletalMesh::SkeletalMesh() { skeleton = nullptr; bones.clear(); }
SkeletalMesh::SkeletalMesh(const SkeletalMesh& b) : Mesh(b) { skeleton = b.skeleton; bones.assign(b.bones.begin(), b.bones.end()); }
void SkeletalMesh::copy_from(const SPTR_SkeletalMesh b) { Mesh::copy_from(b); skeleton = b->skeleton; bones.assign(b->bones.begin(), b->bones.end()); }
//void SkeletalMesh::set_bones(const std::vector<Bone>& b) { bones.assign(b.begin(), b.end()); }

void SkeletalMesh::draw(SPTR_Shader shader) {
	if (shader) {
		// 更新 骨骼变换矩阵
		for (int i = 0; i < bones.size(); ++i) {
			shader->set_mat4("u_bones[" + StringHelper_ins().int_to_string(i) + "]", bones[i].mat_finall);
		}
	}
	for (auto rd : render_datas) {
		auto t_material = rd.material;
		if (t_material == nullptr && rd.rd->get_material_name().compare("") != 0) { t_material = AssetManager_ins().get_material(rd.rd->get_material_name()); }
		if (t_material == nullptr && use_default_mt) { t_material = AssetManager_ins().get_material(Material::default_material_name); }
		if (t_material != nullptr) { t_material->use(shader); }
		rd.rd->draw();
		Material::un_use(shader);
	}
}
Bone& SkeletalMesh::get_bone(int bid) {
	assert(bid >= 0 && bid < bones.size());
	return bones[bid];
}

void SkeletalMesh::update_bone_mat(int id, const CMatrix4x4& mat) {
	get_bone(id).mat_finall = mat_global * mat * get_bone(id).mat_offset;
	//c_debuger() << get_bone(id).mat_offset.to_string();
}
