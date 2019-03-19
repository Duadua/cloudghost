#include "cmath.h"
#include "flowsphere.h"
#include "timemanager.h"

#include "loader.h"

IMPLEMENT_CLASS(FlowSphere)

void FlowSphere::update_origin_location() {
	if(root_component != nullptr) o_location = root_component->get_location();
}

void FlowSphere::begin_play() {
}

void FlowSphere::tick() {

	float sy = 0.1 * std::sin(time_manager().cur_runtime_msconds() / 1000.0f);

	//root_component->set_location(root_component->get_location().x(), root_component->get_location().y() + sy, root_component->get_location().z());
	root_component->set_location(o_location + CVector3D(0.0f, sy, 0.0f));
}
