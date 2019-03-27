#include "shader.h"
#include "skeletalmesh.h"
#include "assetmanager.h"
#include "skeletalmeshcomponent.h"

IMPLEMENT_CLASS(SkeletalMeshComponent)

SkeletalMeshComponent::SkeletalMeshComponent() { mesh = nullptr; }
SkeletalMeshComponent::~SkeletalMeshComponent() { }

void SkeletalMeshComponent::draw(const std::string& shader) {
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

