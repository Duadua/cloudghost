#include "mesh.h"
#include "shader.h"
#include "meshcomponent.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(MeshComponent)

MeshComponent::MeshComponent() { mesh = nullptr; }
MeshComponent::~MeshComponent() { }

void MeshComponent::draw(const std::string& shader) {
	if (mesh != nullptr) {
		auto t_shader = AssetManager_ins().get_shader(shader);
		if (t_shader != nullptr) {
			if (is_border) {
				t_shader->set_mat4("u_model", get_transform().scale(1.1f));
			}
			else t_shader->set_mat4("u_model", get_transform());
		}
		mesh->draw(shader);
	}

	for (auto cc : child_components) { cc->draw(shader); }
}
