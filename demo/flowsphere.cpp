#include "cmath.h"
#include "flowsphere.h"
#include "timemanager.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(FlowSphere)

void FlowSphere::update_origin_location() {
	if(root_component != nullptr) o_location = root_component->get_location();
}

void FlowSphere::begin_play() {
	static bool ok = true;
	/*if (ok) {
		ok = false;
		auto t_mc = std::dynamic_pointer_cast<MeshComponent>(root_component);
		if (t_mc) {
			auto t_m = t_mc->get_mesh();
			if (t_m) {
				std::vector<CVertex> t_vb;
				std::vector<CVertex> t_vs;
				RenderDataReprocess_ins().bump(t_vb, t_m->render_data(0).rd->get_vertices(), 5, 1.0f, 0.00f);
				RenderDataReprocess_ins().smooth(t_vs, t_vb, t_m->render_data(0).rd->get_indices());
				t_m->render_data(0).rd->init(t_vs, t_m->render_data(0).rd->get_indices());
				
			}
		}
	}
	*/
	
}

void FlowSphere::tick() {

	float sy = 0.1f * std::sin(TimeManager_ins().cur_runtime_seconds());

	root_component->set_location(o_location + CVector3D(0.0f, sy, 0.0f));

}
