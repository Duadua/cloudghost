#include "riven.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(Riven)

Riven::Riven() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root_component(rt);

	auto mc = CREATE_CLASS(MeshComponent);
	mc->attach_to(rt);
	mc->set_mesh("riven");
	mc->set_scale(0.02f);
	//	mc->set_rotation(-90.0f, 0.0f, 0.0f);

}

void Riven::begin_play() {

}
void Riven::tick() {

}
