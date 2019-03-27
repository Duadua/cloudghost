#include "mesh.h"
#include "plane.h"
#include "material.h"
#include "assetmanager.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(PlaneObject)

PlaneObject::PlaneObject() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root_component(rt);

	auto mc = CREATE_CLASS(SceneComponent);
	mc->attach_to(rt);
	mc->set_scale(5.0f);

	auto mc1 = CREATE_CLASS(MeshComponent);
	mc1->attach_to(mc);
	mc1->set_mesh(AssetManager_ins().get_mesh("rect"));
	mc1->set_location(1.0f, 0.0f, 1.0f);
	 mc1->set_rotation(90.0f, 0.0f, 0.0f);

	auto mc2 = CREATE_CLASS(MeshComponent);
	mc2->attach_to(mc);
	mc2->set_mesh(AssetManager_ins().get_mesh("rect"));
	mc2->set_location(-1.0f, 0.0f, 1.0f);
	mc2->set_rotation(90.0f, 0.0f, 0.0f);

	auto mc3 = CREATE_CLASS(MeshComponent);
	mc3->attach_to(mc);
	mc3->set_mesh(AssetManager_ins().get_mesh("rect"));
	mc3->set_location(1.0f, 0.0f, -1.0f);
	mc3->set_rotation(90.0f, 0.0f, 0.0f);

	auto mc4 = CREATE_CLASS(MeshComponent);
	mc4->attach_to(mc);
	mc4->set_mesh(AssetManager_ins().get_mesh("rect"));
	mc4->set_location(-1.0f, 0.0f, -1.0f);
	mc4->set_rotation(90.0f, 0.0f, 0.0f);
	

}

void PlaneObject::begin_play() {

}
void PlaneObject::tick() {

}

void PlaneObject::set_material(SPTR_Material name, uint rid) {
	auto t_mo = get_root_component()->get_child_components()[0];
	if (t_mo != nullptr) {
		for (uint i = 0; i < 4; ++i) {
			auto t_mc = std::dynamic_pointer_cast<MeshComponent>(t_mo->get_child_components()[i]);
			if (t_mc == nullptr) continue;
			auto t_m = t_mc->get_mesh();
			if (t_m != nullptr) {
				t_m->render_data(rid).material = name;
			}
		}
	}
}
