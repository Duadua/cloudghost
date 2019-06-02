#include "mesh.h"
#include "pbrtest.h"
#include "material.h"
#include "assetmanager.h"
#include "meshcomponent.h"

IMPLEMENT_CLASS(PBRTest)

PBRTest::PBRTest() {
	auto rt = CREATE_CLASS(SceneComponent);
	set_root_component(rt);
}

void PBRTest::begin_play() {

}

void PBRTest::init(int r, int c, float s, float sz) {
	row = r; col = c;
	space = s; size = sz;

	float t_wid = 2.0f * size * row + space * (row - 1); t_wid /= 2.0f;
	float t_hei = 2.0f * size * col + space * (col - 1); t_hei /= 2.0f;

	for (int i = 1; i <= row; ++i) {
		for (int j = 1; j <= col; ++j) {
			auto mc = CREATE_CLASS(MeshComponent);
			mc->attach_to(root_component);
			auto t_m = AssetManager_ins().get_mesh("sphere");
			t_m->set_b_sphere_tex_coord(true);
			auto t_mt = AssetManager_ins().get_material("pbr_blue");
			t_mt->set_roughness(static_cast<float>(i-1)/row);
			t_mt->set_metallic(static_cast<float>(j)/col);

			t_m->render_data(0).material = t_mt;
			mc->set_mesh(t_m);
			float t_x = -t_wid + size + (i - 1)*(2 * size + space);
			float t_y = -t_hei + size + (j - 1)*(2 * size + space);
			mc->set_location(t_x, size, t_y);
			mc->set_scale(size);
			//mc1->set_rotation(-180.0f, 0.0f, 0.0f);
		}
	}
}

void PBRTest::set_material(SPTR_Material name, uint rid) {
	auto t_mo = get_root_component()->get_child_components()[0];
	if (t_mo != nullptr) {
		for (uint i = 0; i < 4; ++i) {
			auto t_mc = std::dynamic_pointer_cast<MeshComponent>(t_mo->get_child_components()[i]);
			if (t_mc == nullptr) continue;
			auto t_m = t_mc->get_mesh();
			if (t_m != nullptr) {
				t_m->render_data(rid).material = name;
			}
		}
	}
}
