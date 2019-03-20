#include "cubeobject.h"
#include "assetmanager.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(CubeObject)

CubeObject::CubeObject() {

	auto mc = CREATE_CLASS(MeshComponent);
	set_root_component(mc);
	mc->set_mesh("cube");
}

void CubeObject::begin_play() {
	
}

void CubeObject::tick() {

}

void CubeObject::set_material(std::string name, uint id) {
	auto t_mo = std::dynamic_pointer_cast<MeshComponent>(get_root_component());
	if (t_mo != nullptr) {
		auto t_m = t_mo->get_mesh();
		if (t_m != nullptr) {
			t_m->render_data(id).material = name;
		}
	}
}
