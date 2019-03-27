#include "riven.h"
#include "assetmanager.h"
#include "skeletalmesh.h"
#include "skeletalmeshcomponent.h"

IMPLEMENT_CLASS(Riven)

Riven::Riven() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root_component(rt);

	auto mc = CREATE_CLASS(SkeletalMeshComponent);
	mc->attach_to(rt);
	mc->set_mesh(AssetManager_ins().get_mesh_skeletal("riven"));
	auto ms = mc->get_mesh();
	if (ms) {
		ms->set_animation(AssetManager_ins().get_anim_sequence("riven18_idle1"));
	}
	mc->set_scale(0.02f);
	//	mc->set_rotation(-90.0f, 0.0f, 0.0f);

}

void Riven::begin_play() {

}
void Riven::tick() {

}
