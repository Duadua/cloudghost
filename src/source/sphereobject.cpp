#include "mesh.h"
#include "assetmanager.h"
#include "sphereobject.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(SphereObject)

SphereObject::SphereObject() {

	auto mc = CREATE_CLASS(MeshComponent);
	set_root_component(mc);
	mc->set_mesh(AssetManager_ins().get_mesh("sphere"));
}

void SphereObject::begin_play() {

}

void SphereObject::tick() {

}

void SphereObject::set_material(SPTR_Material name, uint rid) {
	auto t_mo = std::dynamic_pointer_cast<MeshComponent>(get_root_component());
	if (t_mo != nullptr) {
		auto t_m = t_mo->get_mesh();
		if (t_m != nullptr) {
			t_m->render_data(rid).material = name;
		}
	}
}
