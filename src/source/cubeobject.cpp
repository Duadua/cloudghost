#include "cubeobject.h"
#include "assetmanager.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(CubeObject)

CubeObject::CubeObject() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root_component(rt);

	auto mc = CREATE_CLASS(MeshComponent);
	mc->attach_to(rt);
	mc->set_mesh("cube");
}

void CubeObject::begin_play() {
	
}

void CubeObject::tick() {

}

void CubeObject::set_material(std::string name) {
	auto t_mesh = AssetManager::get_mesh("cube");
	t_mesh->render_data(0).material = name;
}
