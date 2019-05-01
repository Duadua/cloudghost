#include "shader.h"
#include "texture2d.h"
#include "cameradata.h"
#include "freecamera.h"
#include "inputmanager.h"
#include "assetmanager.h"
#include "mygamemanager.h"
#include "meshcomponent.h"
#include "lightobject.h"
#include "lightcomponent.h"
#include "cameracomponent.h"
#include "skeletalmeshcomponent.h"

#include "cubeobject.h"
#include "sphereobject.h"
#include "cylinderobject.h"

#include "riven.h"
#include "plane.h"
#include "street.h"
#include "mansion.h"
#include "flowsphere.h"
#include "rotatecylinder.h"

void MyGameManager::load_asset() {
	// shader

	// mesh
	AssetManager_ins().load_mesh_x("elf_mansion", "resources/models/static/Elf_Mansion/Elf_Mansion.fbx");
	AssetManager_ins().load_mesh_x("street", "resources/models/static/Street/Street_environment_V01.obj");
	//skeletal mesh
	AssetManager_ins().load_mesh_skeletal("riven", "resources/models/animation/Riven18/riven18.FBX");
	// animation
	AssetManager_ins().load_anim_sequences("resources/models/animation/Riven18/riven18_idle1.FBX", "riven");
	AssetManager_ins().load_anim_sequences("resources/models/animation/Riven18/riven18_dance.FBX", "riven");

	// material
	AssetManager_ins().load_materials("resources/materials/txt/cube_material.txt"); 

	// texture
	AssetManager_ins().load_texture_x("resources/textures/wood.png");
	AssetManager_ins().load_texture("resources/textures/txt/texture_solide_white.txt");
	AssetManager_ins().load_texture("resources/textures/txt/texture_solide_gray.txt");
	AssetManager_ins().load_texture("resources/textures/txt/texture_gradua_red.txt");

}

void MyGameManager::begin_play() {
	// 生成 plan
	{
		auto plane = CREATE_CLASS(PlaneObject);
		add_game_object("plane", plane);
		plane->get_root_component()->add_scale(10.0f);
		plane->get_root_component()->add_rotation(-90.0f, 0.0f, 0.0f);
		plane->set_material(AssetManager_ins().get_material("brickwall"));
	}
	
	
	// add Riven
	{
		{
			auto riven = CREATE_CLASS(Riven);
			add_game_object("riven", riven);
			riven->get_root_component()->set_location(-3.0f, 0.0f, 2.0f);
			riven->get_root_component()->set_rotation(0.0f, 90.0f, 0.0f);
		}
		{
			auto riven = CREATE_CLASS(Riven);
			add_game_object("rivend", riven);
			riven->get_root_component()->set_location(3.0f, 0.0f, 0.0f);
			riven->get_root_component()->set_rotation(0.0f, -90.0f, 0.0f);
			auto mc = std::dynamic_pointer_cast<SkeletalMeshComponent>(riven->get_root_component()->get_child_components()[0]);
			if (mc) { mc->set_anim(AssetManager_ins().get_anim_sequence("riven18_dance")); }
		}
	}

	// 添加 城堡 mesh
	{
		auto mansion = CREATE_CLASS(Mansion);
		add_game_object("mansion", mansion);
		mansion->get_root_component()->set_location(3.0f, 5.0f, 15.0f);
		mansion->get_root_component()->set_rotation(0.0f, 180.0f, 0.0f);
	}
	{
		auto street = CREATE_CLASS(Street);
		add_game_object("street", street);
		street->get_root_component()->set_location(0.0f, 0.1f, -60.0f);
	}

	// 生成 cylinder -- test rb3d
	{
		for (int j = 0; j < 2; ++j) {
			for (int i = 0; i < 6; ++i) {
				{
					SPTR_GameObject t_mo;
					if (i & 1) { t_mo = CREATE_CLASS(RotateCylinder); }
					else { t_mo = CREATE_CLASS(SphereObject); }
					//auto t_mo = CREATE_CLASS(RotateCylinder);
					std::string t_name = "cylinder" + StringHelper_ins().int_to_string(j) + "_" + StringHelper_ins().int_to_string(i);
					add_game_object(t_name, t_mo);
					t_mo->get_root_component()->set_location(1.5f*(j - 0.5f), 0.5f, 2.0f * (i - 2));
					t_mo->get_root_component()->set_scale(0.5);
					if (i & 1) {
						t_mo->get_root_component()->set_location(1.5f*(j - 0.5f), 0.651f, 2.0f * (i - 2));
						t_mo->get_root_component()->set_scale(0.35f, 1.3f, 0.35f);
						auto t_rc = std::dynamic_pointer_cast<RotateCylinder>(t_mo);
						t_rc->update_origin_rotation();
						if ((i+j)&1) t_rc->set_material(AssetManager_ins().get_material("emerald"));
						else t_rc->set_material(AssetManager_ins().get_material("jade"));
						t_rc->set_material(AssetManager_ins().get_material("default"), 2);
					}
				}
				{
					SPTR_GameObject t_mo;
					if (i & 1) { t_mo = CREATE_CLASS(FlowSphere); }
					else { continue; t_mo = CREATE_CLASS(SphereObject); }
					//auto t_mo = CREATE_CLASS(FlowSphere);
					std::string t_name = "sphere" + StringHelper_ins().int_to_string(j) + "_" + StringHelper_ins().int_to_string(i);
					add_game_object(t_name, t_mo);
					t_mo->get_root_component()->set_location(1.5f*(j - 0.5f), 1.30f, 2.0f * (i - 2));
					t_mo->get_root_component()->set_scale(0.2f);
					if (i & 1) {
						t_mo->get_root_component()->set_location(1.5f*(j - 0.5f), 1.70f, 2.0f * (i - 2));
						t_mo->get_root_component()->set_scale(0.18f);
						auto t_fs = std::dynamic_pointer_cast<FlowSphere>(t_mo);
						t_fs->update_origin_location();
						if ((i+j)&1) t_fs->set_material(AssetManager_ins().get_material("jade"));
						else t_fs->set_material(AssetManager_ins().get_material("emerald"));
					}
				
				}
	
			}
		}
	}
	
	// 生成 sphere -- test pbr
	{
		auto pbr_sphere = CREATE_CLASS(SphereObject);
		pbr_sphere->get_root_component()->set_location(8.0f, 1.0f, 7.0f);
		add_game_object("pbr_sphere", pbr_sphere);
		//add_game_object_pbr("pbr_sphere", pbr_sphere);
	}
	
	

	// use direct light
	{
		auto d_light = CREATE_CLASS(DirectLightObject);
		d_light->get_root_component()->set_rotation(-60.0f, -45.0f, 0.0f);
		//d_light->get_root_component()->set_rotation(-60.0f, -135.0f, 0.0f);
		d_light->get_light_component()->set_intensity(300.0f);
		d_light->get_light_component()->set_k(CVector3D(1.0f, 1.0f, 1.0f));
		//d_light->get_light_component()->set_k(CVector3D(0.2f, 0.7f, 0.1f));
		add_direct_light("d_light", d_light);
		
	}

	// use point light
	{
		auto p_light = CREATE_CLASS(PointLightObject);
		p_light->get_root_component()->set_location(-5.0f, 2.0f, 5.0f);
		p_light->get_light_component()->set_color(CVector3D(0.0f, 0.5f, 1.0f));
		p_light->get_light_component()->set_att_radius(5.0f);
		p_light->get_light_component()->set_intensity(100.0f);
		add_point_light("p_light0", p_light);
	}
	{
		auto p_light = CREATE_CLASS(PointLightObject);
		//p_light->get_root_component()->set_location(0.0f, 3.0f, 3.0f);
		p_light->get_root_component()->set_location(4.0f, 6.0f, 15.0f);
		p_light->get_light_component()->set_att_radius(50.0f);
		p_light->get_light_component()->set_intensity(50.0f);
		p_light->get_light_component()->set_color(CVector3D(0.7f, 0.3f, 0.0f));
		add_point_light("p_light1", p_light);
	}
	
	
	// use spot light
	{
		auto s_light = CREATE_CLASS(SpotLightObject);
		s_light->get_root_component()->set_location(-1.0f, 7.0f, 15.0f);
		s_light->get_root_component()->set_rotation(-90.0f, -0.0f, 0.0f);
		//s_light->get_light_component()->set_color(CVector3D(0.0f, 0.0f, 1.0f));
		s_light->get_light_component()->set_intensity(300.0f);
		s_light->get_light_component()->set_att_radius(10.0f);
		s_light->get_light_component()->set_inner(30.0f);
		s_light->get_light_component()->set_outer(32.0f);

		add_spots_light("s_light", s_light);
	}

}

void MyGameManager::map_input() {
	InputState is;
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager_ins().map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::RightButton;
	InputManager_ins().map_action("turn_over", is);
	is = InputState();
	is.modifiers = Qt::ControlModifier;
	is.mouse_released = Qt::RightButton;
	InputManager_ins().map_action("turn_over", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager_ins().map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::LeftButton;
	InputManager_ins().map_action("turn_over", is);
	is = InputState();
	is.modifiers = Qt::ControlModifier;
	is.mouse_released = Qt::LeftButton;
	InputManager_ins().map_action("turn_over", is);

	// look_up
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("look_up", is);

	// move_forward
	is = InputState();
	is.axis_types = InputAxisType::WHEEL;
	InputManager_ins().map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_forward_plane", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_forward", is);

	// move right
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.axis_types = InputAxisType::MOUSE_X;
	is.axis_scale = 1.0f;
	InputManager_ins().map_axis("move_right", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_right", is);

	// move up
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_up", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	InputManager_ins().map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -1.0f;
	InputManager_ins().map_axis("move_up", is);
}

