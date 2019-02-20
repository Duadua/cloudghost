#include "plane.h"
#include "assetmanager.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(PlaneObject)

PlaneObject::PlaneObject() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root(rt);

	auto mc = CREATE_CLASS(MeshComponent);
	mc->attach_to(rt);
	mc->set_mesh("rect");
	mc->set_location(QVector3D(2.5f, 0.0f, 2.5f));
	mc->set_roataion(QVector3D(90.0f, 0.0f, 0.0f));
	mc->set_scale(QVector3D(5.0f, 5.0f, 5.0f));
	
	auto mc2 = CREATE_CLASS(MeshComponent);
	mc2->attach_to(rt);
	mc2->set_mesh("rect");
	mc2->set_location(QVector3D(-2.5f, 0.0f, 2.5f));
	mc2->set_roataion(QVector3D(90.0f, 0.0f, 0.0f));
	mc2->set_scale(QVector3D(5.0f, 5.0f, 5.0f));

	auto mc3 = CREATE_CLASS(MeshComponent);
	mc3->attach_to(rt);
	mc3->set_mesh("rect");
	mc3->set_location(QVector3D(2.5f, 0.0f, -2.5f));
	mc3->set_roataion(QVector3D(90.0f, 0.0f, 0.0f));
	mc3->set_scale(QVector3D(5.0f, 5.0f, 5.0f));

	auto mc4 = CREATE_CLASS(MeshComponent);
	mc4->attach_to(rt);
	mc4->set_mesh("rect");
	mc4->set_location(QVector3D(-2.5f, 0.0f, -2.5f));
	mc4->set_roataion(QVector3D(90.0f, 0.0f, 0.0f));
	mc4->set_scale(QVector3D(5.0f, 5.0f, 5.0f));

}

void PlaneObject::begin_play() {

}
void PlaneObject::tick() {

}
