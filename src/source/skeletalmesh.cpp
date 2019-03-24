#include "skeletalmesh.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(SkeletalMesh)

SkeletalMesh::SkeletalMesh() { skeleton = nullptr; bones.clear(); }
SkeletalMesh::SkeletalMesh(const SkeletalMesh& b) : Mesh(b) { skeleton = b.skeleton; bones.assign(b.bones.begin(), b.bones.end()); }
void SkeletalMesh::copy_from(const SPTR_SkeletalMesh b) { Mesh::copy_from(b); skeleton = b->skeleton; bones.assign(b->bones.begin(), b->bones.end()); }

void SkeletalMesh::set_bones(const std::vector<Bone>& b) { bones.assign(b.begin(), b.end()); }

void SkeletalMesh::draw(const std::string& shader) {
	auto t_shader = AssetManager_ins().get_shader(shader);
	if (t_shader) {
		for (int i = 0; i < bones.size(); ++i) {
			
			//t_shader->set_mat4()
		}
	}
	for (auto rd : render_datas) {
		std::string t_name = rd.material;
		if (t_name.compare("") == 0) { t_name = rd.rd->get_material_name(); }
		auto t_material = AssetManager_ins().get_material(t_name);
		if (t_material == nullptr && use_default_mt) { t_material = AssetManager_ins().get_material(Material::default_material_name); }
		if (t_material != nullptr) { t_material->use(shader); }
		rd.rd->draw();
		Material::un_use(shader);
	}
}