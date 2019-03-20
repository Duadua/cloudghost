#include "cmath.h"
#include "flowsphere.h"
#include "timemanager.h"

IMPLEMENT_CLASS(FlowSphere)

void FlowSphere::update_origin_location() {
	if(root_component != nullptr) o_location = root_component->get_location();
}

void FlowSphere::begin_play() {
}

void FlowSphere::tick() {

	float sy = 0.1f * std::sin(time_manager().cur_runtime_seconds());

	root_component->set_location(o_location + CVector3D(0.0f, sy, 0.0f));

}
