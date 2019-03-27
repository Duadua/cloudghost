#include "mansion.h"
#include "assetmanager.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(Mansion)

Mansion::Mansion() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root_component(rt);

	auto mc = CREATE_CLASS(MeshComponent);
	mc->attach_to(rt);
	mc->set_mesh(AssetManager_ins().get_mesh("elf_mansion"));
	//mc->set_rotation(-90.0f, 0.0f, 0.0f);

}

void Mansion::begin_play() {

}
void Mansion::tick() {

}
