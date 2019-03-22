#include "meshcomponent.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(MeshComponent)

MeshComponent::MeshComponent() { mesh = nullptr; }
MeshComponent::~MeshComponent() { }

void MeshComponent::draw(const std::string& shader) {
	if (mesh != nullptr) {
		auto t_shader = AssetManager::get_shader(shader);
		if (t_shader != nullptr) {
			if (is_border) {
				CVector3D t_s = 1.0f + 0.1f / get_scale();
				t_shader->set_mat4("u_model", get_transform().scale(t_s));
			}
			else t_shader->set_mat4("u_model", get_transform());
		}
		mesh->draw(shader);
	}

	for (auto cc : child_components) { cc->draw(shader); }
}
void MeshComponent::set_mesh(const std::string& name) { mesh = AssetManager::get_mesh(name); }
