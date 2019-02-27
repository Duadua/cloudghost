#include "meshcomponent.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(MeshComponent)

MeshComponent::MeshComponent(const std::string& m) { mesh = m; }
MeshComponent::~MeshComponent() { }

void MeshComponent::draw(const std::string& shader) {
	if (mesh.compare("") != 0) {
		auto t_shader = AssetManager::get_shader(shader);
		if(t_shader != nullptr) t_shader->set_mat4("u_model", CMatrix4x4(get_transform().data()));

		auto t_mesh = AssetManager::get_mesh(mesh);
		if(t_mesh != nullptr) t_mesh->draw(shader);
	}

	for (auto cc : child_components) { cc->draw(shader); }
}
