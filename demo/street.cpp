#include "street.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(Street)

Street::Street() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root_component(rt);

	auto mc = CREATE_CLASS(MeshComponent);
	mc->attach_to(rt);
	mc->set_mesh("street");
}

void Street::begin_play() {

}
void Street::tick() {

}
