#include "skybox.h"

#include "mesh.h"
#include "assetmanager.h"
#include "meshcomponent.h"
#include "texture3d.h"

IMPLEMENT_CLASS(SkyBox)

SkyBox::SkyBox() : texture(nullptr) {
	
	auto mc = CREATE_CLASS(MeshComponent);
	set_root_component(mc);
	mc->set_mesh(AssetManager_ins().get_mesh("cube"));
}

void SkyBox::begin_play() {

}


