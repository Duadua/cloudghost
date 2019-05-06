#include "skybox.h"

#include "mesh.h"
#include "assetmanager.h"
#include "meshcomponent.h"
#include "texture3d.h"

IMPLEMENT_CLASS(SkyBox)

SkyBox::SkyBox() : texture(nullptr), texture_hdr(nullptr), b_use_hdr(false) {
	
	auto mc = CREATE_CLASS(MeshComponent);
	set_root_component(mc);
	auto t_m = AssetManager_ins().get_mesh("cube");
	t_m->set_use_default_mt(false);
	mc->set_mesh(t_m);
}

void SkyBox::set_texture_hdr(SPTR_Texture2D tex) {
	texture_hdr = tex;
	if (texture_hdr) { b_use_hdr = true; }
}

void SkyBox::begin_play() {

}


