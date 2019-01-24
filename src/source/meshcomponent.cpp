#include "mesh.h"
#include "meshcomponent.h"

MeshComponent::MeshComponent(Mesh* m) {
	mesh = m;
}

MeshComponent::~MeshComponent() {
	delete mesh;
}

void MeshComponent::set_mesh(Mesh* m) {
	mesh = m;
}

void MeshComponent::draw() {
	if (mesh != nullptr) mesh->draw();
	for (auto cc : child_components) {
		cc->draw();
	}

}
