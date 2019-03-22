#include "cmath.h"
#include "rotatecylinder.h"
#include "timemanager.h"

IMPLEMENT_CLASS(RotateCylinder)

void RotateCylinder::update_origin_rotation() {
	if (root_component != nullptr) o_rotation = root_component->get_rotation();
}

void RotateCylinder::begin_play() {
}

void RotateCylinder::tick() {

	//float sy = 1.0f * (time_manager().cur_runtime_msconds()/10 % 360);
	float sy = - std::fmodf(0.2f * time_manager().cur_runtime_msconds(), 360.0f);

	root_component->set_rotation(o_rotation + CVector3D(0.0f, sy, 0.0f));

}