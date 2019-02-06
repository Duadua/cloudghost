#include "mesh.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(MeshComponent)

MeshComponent::MeshComponent(Mesh* m) {
	mesh = m;
}

MeshComponent::~MeshComponent() { }

void MeshComponent::set_mesh(Mesh* m) {
	mesh = m;
}

void MeshComponent::draw(Shader& shader) {
	if (mesh != nullptr) {
		shader.set_mat4("model", get_transform());
		mesh->draw();
	}
	for (auto cc : child_components) {
		cc->draw(shader);
	}

}
