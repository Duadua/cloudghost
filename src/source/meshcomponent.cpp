#include "mesh.h"
#include "shader.h"
#include "meshcomponent.h"
#include "assetmanager.h"

IMPLEMENT_CLASS(MeshComponent)

MeshComponent::MeshComponent() { mesh = nullptr; }
MeshComponent::~MeshComponent() { }

void MeshComponent::tick(float time) {
	if (mesh != nullptr) { mesh->tick(time); }

}
void MeshComponent::draw(SPTR_Shader shader) {
	if (mesh != nullptr) {
		if (shader != nullptr) {
			if (is_border) {
				shader->set_mat4("u_model", get_transform().scale(1.1f));
			}
			else shader->set_mat4("u_model", get_transform());
		}
		mesh->draw(shader);
	}
}
