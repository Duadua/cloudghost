#define GLEW_STATIC
#include <GL/glew.h>

#include "mesh.h"
#include "shader.h"
#include "skybox.h"
#include "texture2d.h"
#include "texture3d.h"
#include "gameobject.h"
#include "freecamera.h"
#include "cameradata.h"
#include "lightobject.h"
#include "rendertarget.h"
#include "uniformbuffer.h"
#include "lightcomponent.h"
#include "cameracomponent.h"

#include "timemanager.h"
#include "gamemanager.h"
#include "assetmanager.h"
#include "inputmanager.h"

#include <chrono>					// get time
#include <assert.h>

#include <GLFW/glfw3.h>

void GameManager::_init() {
	
	// inner object init
	{
		sky_box = nullptr;
	}

	// ui setting init
	{
		background_color = CColor(205, 220, 232);
		border_color = CColor(221, 161, 18);

		front_polygon_mode = GL_FILL;
		back_polygon_mode = GL_FILL;

		b_use_vr = false;
		vr_delta = 6.0f;

		pp_type = PostProcessType::NOPE;

		b_gamma = true;
		v_gamma = 2.2f;

		b_use_shader_toy = false;

		b_normal_visual = false;
		b_explode = false;

		b_msaa = true;
		b_msaa_custom = true;

		b_hdr = true;
		hdr_type = HDR_Type::REINHARD;
		hdr_exposure = 1.0f;

		b_skybox = true;
		b_dynamic_env = false;

		b_depth = false;
		b_shadow = true;
		b_normal_map = true;
	}
}

void GameManager::init() {

	// init glfw
	/*{
		glfwInit();
	}*/
	// init glew
	{
		glewExperimental = true;
		uint flag = glewInit();
		if (flag != GLEW_OK) {
			c_debuger() << "[error][glew]init fail";
            c_debuger() << reinterpret_cast<const char*>(glewGetErrorString(flag));
		}
		else { }
	}
		
	b_mouse_clicked = false;

	// load asset
	{
		// shader
		AssetManager_ins().load_shader("default", "resources/shaders/mvp_anim.vert", "resources/shaders/blinn_phong.frag");
		AssetManager_ins().load_shader("default_shadow", "resources/shaders/mvp_anim_shadow.vert", "resources/shaders/blinn_phong_shadow.frag");
		AssetManager_ins().load_shader("default_texture_only", "resources/shaders/mvp_anim.vert", "resources/shaders/texture_only.frag");
		AssetManager_ins().load_shader("default_light_only", "resources/shaders/mvp_anim_shadow.vert", "resources/shaders/blinn_phong_light_only.frag");
		//AssetManager_ins().load_shader("default", "resources/shaders/mvp_anim.vert", "resources/shaders/single_texture.frag");
		AssetManager_ins().load_shader("depth", "resources/shaders/mvp_anim.vert", "resources/shaders/depth.frag");
		AssetManager_ins().load_shader("border", "resources/shaders/scene_2d.vert", "resources/shaders/border.frag");
		AssetManager_ins().load_shader("solid_color", "resources/shaders/mvp_anim.vert", "resources/shaders/solid_color.frag");
		AssetManager_ins().load_shader("single_texture", "resources/shaders/mvp_anim.vert", "resources/shaders/single_texture.frag");
		AssetManager_ins().load_shader("scene2d", "resources/shaders/scene_2d.vert", "resources/shaders/single_texture_2d.frag");
		AssetManager_ins().load_shader("pp", "resources/shaders/scene_2d.vert", "resources/shaders/post_process.frag");
		AssetManager_ins().load_shader("gamma", "resources/shaders/scene_2d.vert", "resources/shaders/gamma.frag");
		AssetManager_ins().load_shader("pick", "resources/shaders/mvp_anim.vert", "resources/shaders/pick.frag");
		AssetManager_ins().load_shader("vr_mix", "resources/shaders/scene_2d.vert", "resources/shaders/vr_mix.frag");
		AssetManager_ins().load_shader("normal_visual", "resources/shaders/mvp_anim.vert", "resources/shaders/solid_color.frag", "resources/shaders/normal_visual.geom");
		AssetManager_ins().load_shader("explode", "resources/shaders/mvp_anim.vert", "resources/shaders/blinn_phong.frag", "resources/shaders/explode.geom");
		AssetManager_ins().load_shader("msaa", "resources/shaders/scene_2d.vert", "resources/shaders/msaa.frag");
		AssetManager_ins().load_shader("hdr", "resources/shaders/scene_2d.vert", "resources/shaders/hdr.frag");
		AssetManager_ins().load_shader("skybox", "resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
		AssetManager_ins().load_shader("hdr_to_skybox", "resources/shaders/skybox.vert", "resources/shaders/equirectangular_to_cubemap.frag");
		AssetManager_ins().load_shader("shadow", "resources/shaders/shadow.vert", "resources/shaders/shadow.frag");
		AssetManager_ins().load_shader("shadow_point", "resources/shaders/shadow_point.vert", "resources/shaders/shadow_point.frag", "resources/shaders/shadow_point.geom");

		AssetManager_ins().load_shader("pbr", "resources/shaders/mvp_anim.vert", "resources/shaders/pbr.frag");
		AssetManager_ins().load_shader("pbr_ibl", "resources/shaders/mvp_anim.vert", "resources/shaders/pbr_ibl.frag");

		AssetManager_ins().load_shader("shader_toy_img", "resources/shaders/scene_2d.vert", "resources/shaders/shadertoy_img_pbr.frag");
		AssetManager_ins().load_shader("shader_toy_buffer_a", "resources/shaders/scene_2d.vert", "resources/shaders/shadertoy_buffer_a.frag");
		AssetManager_ins().load_shader("shader_toy_buffer_b", "resources/shaders/scene_2d.vert", "resources/shaders/shadertoy_buffer_b.frag");
		AssetManager_ins().load_shader("shader_toy_buffer_c", "resources/shaders/scene_2d.vert", "resources/shaders/shadertoy_buffer_c.frag");
		AssetManager_ins().load_shader("shader_toy_buffer_d", "resources/shaders/scene_2d.vert", "resources/shaders/shadertoy_buffer_d.frag");

		// mesh
		AssetManager_ins().load_mesh("triangle_right", "resources/models/txt/triangle_right.txt");
		AssetManager_ins().load_mesh("triangle_regular", "resources/models/txt/triangle_regular.txt");
		AssetManager_ins().load_mesh("rect", "resources/models/txt/rect.txt");
		AssetManager_ins().load_mesh("circle", "resources/models/txt/circle.txt");

		AssetManager_ins().load_mesh("cube", "resources/models/txt/cube.txt");
		AssetManager_ins().load_mesh("cone", "resources/models/txt/cone.txt");
		AssetManager_ins().load_mesh("cylinder", "resources/models/txt/cylinder.txt");
		AssetManager_ins().load_mesh("sphere", "resources/models/txt/sphere.txt");

		// material
		AssetManager_ins().load_materials("resources/materials/txt/default_material.txt");
		AssetManager_ins().load_materials("resources/materials/txt/single_material.txt");
		AssetManager_ins().load_materials("resources/materials/txt/pbr_material.txt");

		// textures
		//AssetManager_ins().load_texture("resources/textures/txt/texture_default.txt");
		AssetManager_ins().load_texture_x("resources/textures/texture_default.png");
		AssetManager_ins().load_texture_x("resources/textures/brickwall_d.jpg");
		AssetManager_ins().load_texture_x("resources/textures/brickwall_n.jpg", false);
		AssetManager_ins().load_texture_x_hdr("resources/textures/skyboxs/basketball_court/basketball_court.hdr");

		// pbr		
		AssetManager_ins().load_texture_x("resources/textures/rusted_iron/rusted_iron_albedo.png");
		AssetManager_ins().load_texture_x("resources/textures/rusted_iron/rusted_iron_metallic.png", false);
		AssetManager_ins().load_texture_x("resources/textures/rusted_iron/rusted_iron_roughness.png", false);
		AssetManager_ins().load_texture_x("resources/textures/rusted_iron/rusted_iron_normal.png", false);
		AssetManager_ins().load_texture_x("resources/textures/rusted_iron/rusted_iron_ao.png");

		AssetManager_ins().load_texture_x("resources/textures/titanium_scuffed/titanium_scuffed_albedo.png");
		AssetManager_ins().load_texture_x("resources/textures/titanium_scuffed/titanium_scuffed_metallic.png", false);
		AssetManager_ins().load_texture_x("resources/textures/titanium_scuffed/titanium_scuffed_roughness.png", false);
		AssetManager_ins().load_texture_x("resources/textures/titanium_scuffed/titanium_scuffed_normal.png", false);
		AssetManager_ins().load_texture_x("resources/textures/titanium_scuffed/titanium_scuffed_ao.png");

		// textures 3d
		AssetManager_ins().load_texture_3d("resources/textures/skyboxs/lake");
		AssetManager_ins().load_texture_3d("resources/textures/skyboxs/nice");
		AssetManager_ins().load_texture_3d("resources/textures/skyboxs/stormyday");

		load_asset();						// 加载资源
		
	}

	main_camera = set_main_camera();	// 绑定主相机

	// init shader stack 
	{
		stack_shaders = CREATE_CLASS(ShaderStack);

		//stack_shaders->set_shader_default(AssetManager_ins().get_shader("default"));
		stack_shaders->set_shader_default(AssetManager_ins().get_shader("default_shadow"));
	}

	// init uniform block 
	{
		ub_matrices = CREATE_CLASS(UniformBuffer);
		ub_matrices->init(2 * CMatrix4x4::data_size(), 0);

		if (main_camera) {
			main_camera->get_camera_component()->get_camera_data()->get_frustum().width = get_viewport_info().heigh;
			main_camera->get_camera_component()->get_camera_data()->get_frustum().heigh = get_viewport_info().width;
			ub_matrices->fill_data(CMatrix4x4::data_size(), CMatrix4x4::data_size(), main_camera->get_camera_component()->get_proj_mat().data());
		}

		// bind shader to uniform block
		for (auto shader : AssetManager_ins().map_shaders) {
			auto t_shader = shader.second;
			if (t_shader) { t_shader->set_unifom_buffer("Matrices", 0); }
		}
	}

	// init sky box
	{
		sky_box = CREATE_CLASS(SkyBox);
		sky_box->set_texture(AssetManager_ins().get_texture3D("stormyday"));
		sky_box->set_texture_hdr(AssetManager_ins().get_texture("basketball_court.hdr"));
	}

	// init input map
	{
		map_input_default();
		bind_input_default();

		map_input();						
		main_bind_input();
	}

	// gl state init
	{
		set_depth_test(true);
		// set_stencil_test(true, GL_NOTEQUAL, 1, 0xff, GL_KEEP, GL_REPLACE, GL_REPLACE);
		set_blend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		set_polygon_mode(front_polygon_mode, back_polygon_mode);
		set_cull_face(true);

		glEnable(GL_PROGRAM_POINT_SIZE);

		//glEnable(GL_FRAMEBUFFER_SRGB);

	}
	
	// init rt
	{
		init_rt_static();
		init_rt();
	}

	// begin play
	{
		begin_play();						// 设置模型等
		main_begin_play();					 
	}

}
void GameManager::draw() {
	// update time
	{
		TimeManager_ins().update_tick();
	}
	// update
	{
		tick(TimeManager_ins().cur_runtime_msconds());
		main_tick(TimeManager_ins().cur_runtime_msconds());
	}

	// fill shader uniform block
	{
		ub_matrices->fill_data(CMatrix4x4::data_size(), CMatrix4x4::data_size(), main_camera->get_camera_component()->get_proj_mat().data());
		ub_matrices->fill_data(0, CMatrix4x4::data_size(), main_camera->get_camera_component()->get_view_mat().data());
	}
	
	// draw
	{
		auto t_final_tex = scene_texture;
		if (b_depth && !b_use_shader_toy) t_final_tex = depth_texture;
		//t_final_tex = direct_light_shadow_rts[0]->get_attach_textures()[0].texture;

		// pass 0 -- 渲染到默认缓冲 -- 必须第一个执行
		scene_pass(t_final_tex); 						// 显示正常渲染图 

		// pre bake -- 预烘焙 -- 只绘制一次
		{
			if (b_pre_bake) {
				b_pre_bake = false;
				pre_bake();								// 预烘焙
			}
		}

		if(!b_use_shader_toy) {
			if (!b_use_vr) {						

				if (b_depth) { depth_pass(); }					// 生成当前相机视图下的深度图 -- 得到 depth_texture

				// pass 1
				if (b_shadow) { shadow_pass(); }				// 生成(各个光源的)阴影贴图  -- 得到 shadow_texture

				if (b_dynamic_env) { dynamic_env_pass(); }		// 生成动态环境贴图 -- 反射用

				// pass 2
				pick_pass();									// 得到拾取贴图 -- 用来判断哪个物体被拾取 -- 得到拾取物体的 id

				// pass 3
				base_pass();									// 得到 scene_texture

				// pass 4
				border_pass();									// 得到 border_texture -- 然后 输入 scene_texture -- 添加边框 -- 最后得到 scene_texture

				// pass 4
				if (pp_type != PostProcessType::NOPE) { post_process_pass(); } // 后处理 -- 输入 scene_texture 得到 scene_texture

				// pass 5
				if (b_hdr) hdr_pass();							// hdr -- 输入 scene_texture 得到 scene_texture

				// pass 6
				if (b_gamma) gamma_pass();						// gamma 校正 -- 输入 scene_texture 得到 scene_texture
			}
			else {
				if (b_depth) { vr_depth_pass(); }				// 得到 depth_texture
				if (b_shadow) { shadow_pass(); }
				pick_pass();
				vr_base_pass();									// 得到 scene_texture
				vr_border_pass();								// 得到 border_texture -- 然后输入 scene_texture -- 得到scene_texture
				if (pp_type != PostProcessType::NOPE) { post_process_pass(); }
				if (b_hdr) hdr_pass();
				if (b_gamma) gamma_pass();
			}
		}
		else {
			shader_toy_pass();		// render_shader_toy -- 也是得到 scene_texture

			if (pp_type != PostProcessType::NOPE) { post_process_pass(); }

			// if (b_gamma) gamma_pass();
		}
	}
}

// render pass
void GameManager::pre_bake() {
	c_debuger() << "begin baking";
	// hdr skybox -> skybox
	if (sky_box->get_b_use_hdr()) {
		hdr_to_cubemap_pass();
		if (hdr_environment_map_rt && hdr_environment_map_rt->get_attach_texture_3ds().size() > 0) {
			auto t_tex = hdr_environment_map_rt->get_attach_texture_3ds()[0].texture;
			if (t_tex) sky_box->set_texture(t_tex);
		}
	}

	c_debuger() << "baking over";

}
void GameManager::scene_pass(SPTR_Texture2D tex) {

	draw_init();

	stack_shaders->push(AssetManager_ins().get_shader("scene2d")); {
		if (stack_shaders->top()) {
			stack_shaders->use();
			if (tex) { tex->bind(0); }
			stack_shaders->top()->set_int("u_texture", 0);
		}

		draw_scene(stack_shaders->top());

	} stack_shaders->pop();

}
void GameManager::depth_pass() {
	auto t_depth_rt = depth_rt;
	if (b_msaa) { t_depth_rt = depth_msaa_rt; }

	t_depth_rt->use(); {
		draw_init();
		stack_shaders->push(AssetManager_ins().get_shader("depth")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_float("u_near", main_camera->get_camera_component()->get_camera_data()->get_frustum().near);
				stack_shaders->top()->set_float("u_far", main_camera->get_camera_component()->get_camera_data()->get_frustum().far);
			}
			draw_all_objs(stack_shaders->top());
		} stack_shaders->pop();
	} t_depth_rt->un_use();

	if (b_msaa) {
		if (!b_msaa_custom) { blit(t_depth_rt, depth_rt, viewport_info.width, viewport_info.heigh); } // opengl 自带抗锯齿
		else {
			depth_rt->use(); {
				draw_init();
				stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
					if (stack_shaders->top() && t_depth_rt->get_attach_textures().size() > 0) {
						auto t_texture = t_depth_rt->get_attach_textures()[0].texture;
						if (t_texture) { t_texture->bind(0); }
						stack_shaders->top()->use();
						stack_shaders->top()->set_int("u_texture_msaa", 0);
						stack_shaders->top()->set_int("u_msaa_num", t_depth_rt->get_msaa_num());
					}
					draw_scene(stack_shaders->top());
				} stack_shaders->pop();
			} depth_rt->un_use();

		} // 自定义抗锯齿

	}

	if (depth_rt->get_attach_textures().size() > 0) {
		depth_texture = depth_rt->get_attach_textures()[0].texture;
	}


}
void GameManager::shadow_pass() {
	// direction lights's shadow map
	{
		int i = 0;
		for (auto t_p : map_direct_lights) {
			auto t_dlight = t_p.second;
			if (t_dlight) {
				glViewport(0, 0, 1024, 1024); /*set_cull_face(true, GL_BACK);*/ {
					direct_light_shadow_rts[i]->use(); {
						glDrawBuffer(GL_NONE);
						glReadBuffer(GL_NONE);
						draw_init();
						stack_shaders->push(AssetManager_ins().get_shader("shadow")); {
							if (stack_shaders->top()) {
								stack_shaders->top()->use();
								// 设置为此光源的 view and proj 矩阵
								CMatrix4x4 t_view, t_proj;
								t_proj.ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 20.0f);
								t_view.lookAt(-10.0f * t_dlight->get_root_component()->get_front_axis(), CVector3D(0.0f), CVector3D(0.0f, 1.0f, 0.0f));
								t_dlight->get_light_component()->set_mat_proj_view(t_proj * t_view);
								stack_shaders->top()->set_mat4("u_light_proj_view", t_dlight->get_light_component()->get_mat_proj_view());
							}
							draw_all_objs(stack_shaders->top());
						} stack_shaders->pop();
					} direct_light_shadow_rts[i]->un_use();
				} /*set_cull_face();*/ glViewport(0, 0, viewport_info.width, viewport_info.heigh);
				++i;
			} // 每一个光源生成一个 shadow map
		}
	}

	// point lights's shadow map
	{
		int i = 0;
		for (auto t_p : map_point_lights) {
			auto t_light = t_p.second;
			if (t_light) {
				glViewport(0, 0, 1024, 1024); {
					point_light_shadow_rts[i]->use(); {
						point_light_shadow_rts[i]->use_texture_3d_all(0);
						glDrawBuffer(GL_NONE);
						glReadBuffer(GL_NONE);
						draw_init();
						stack_shaders->push(AssetManager_ins().get_shader("shadow_point")); {
							if (stack_shaders->top()) {
								stack_shaders->top()->use();
								// 设置为此光源的 view and proj 矩阵
								CMatrix4x4 t_proj;
								t_proj.perspective(90.0f, 1.0f, 0.1f, 100.0f);
								auto t_loc = t_light->get_root_component()->get_location();
								// 6个方向
								for (int j = 0; j < 6; ++j) {
									CMatrix4x4 t_view;
									t_view.lookAt(t_loc, t_loc + point_front[j], point_world_up[j]);
									t_light->get_light_component()->set_mat_proj_view(j, t_proj*t_view);
									stack_shaders->top()->set_mat4("u_light_proj_view["
										+ StringHelper_ins().int_to_string(j) + "]", 
										t_light->get_light_component()->get_mat_proj_view(j));
								}
								stack_shaders->top()->set_float("u_far", 100.0f);
								stack_shaders->top()->set_vec3("u_light_pos", t_loc);
							}
							draw_all_objs(stack_shaders->top());
						} stack_shaders->pop();
					} point_light_shadow_rts[i]->un_use();
				} glViewport(0, 0, viewport_info.width, viewport_info.heigh);
				
				++i;
			}
		} // 每一个点光源生成一个 cube map -- 作为 point shadow map
	}
}
void GameManager::dynamic_env_pass() {
	glViewport(0, 0, 512, 512); {
		dynamic_environment_map_rt->use(); {

			/*stack_shaders->push(AssetManager_ins().get_shader("dynamic_env"));*/ {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					// 设置为此光源的 view and proj 矩阵
					CMatrix4x4 t_proj;
					t_proj.perspective(90.0f, 1.0f, 0.1f, 100.0f);
					ub_matrices->fill_data(CMatrix4x4::data_size(), CMatrix4x4::data_size(), t_proj.data());
					//auto t_loc = t_light->get_root_component()->get_location();
					auto t_loc = CVector3D(0.0f, 1.0f, 0.0f);
					stack_shaders->top()->set_vec3("u_view_pos", t_loc);
					// 6个方向
					for (int j = 0; j < 6; ++j) {
						dynamic_environment_map_rt->use_texture_3d(0, j);
						dynamic_environment_map_rt->use_texture_3d(1, j);
						
						draw_init();

						CMatrix4x4 t_view;
						t_view.lookAt(t_loc, t_loc + point_front[j], point_world_up[j]);
						ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_view.data());

						// draw obj
						draw_lights(stack_shaders->top());			// set light uniform for shader
						draw_all_objs(stack_shaders->top());

						// draw sky box
						if (b_skybox && sky_box) {
							stack_shaders->push(AssetManager_ins().get_shader("skybox")); {
								if (stack_shaders->top()) {
									stack_shaders->top()->use();
									// 要移除矩阵的移动部分
									auto t_view_s = CMatrix4x4(CMatrix3x3(t_view.data(), 4, 4).data(), 3, 3);
									stack_shaders->top()->set_mat4("u_view_sky_box", t_view_s);
									stack_shaders->top()->set_int("u_texture", 0);
								}
								auto t_texture = sky_box->get_texture();
								if (t_texture) t_texture->bind(0);
								draw_skybox(stack_shaders->top());
							} stack_shaders->pop();
						}
					}
				}

				ub_matrices->fill_data(CMatrix4x4::data_size(), CMatrix4x4::data_size(), main_camera->get_camera_component()->get_proj_mat().data());
				ub_matrices->fill_data(0, CMatrix4x4::data_size(), main_camera->get_camera_component()->get_view_mat().data());

			} /*stack_shaders->pop();*/
		} dynamic_environment_map_rt->un_use();
	} glViewport(0, 0, viewport_info.width, viewport_info.heigh);
}
void GameManager::pick_pass() {

	pick_rt->use(); {
		uint t_fpm = front_polygon_mode;
		front_polygon_mode = GL_FILL;
		draw_init();

		ub_matrices->fill_data(0, CMatrix4x4::data_size(), main_camera->get_camera_component()->get_view_mat().data());

		stack_shaders->push(AssetManager_ins().get_shader("pick")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
				stack_shaders->top()->set_uint("u_object_id", 0);
				stack_shaders->top()->set_uint("u_component_id", 0);
			}

			glStencilMask(0x00);
			draw_all_objs(stack_shaders->top());
			glStencilMask(0xff);

		} stack_shaders->pop();

		if (b_mouse_clicked) {
			b_mouse_clicked = false;
			uint data[3] = { 0 };
			glReadPixels(static_cast<int>(mouse_clicked_x), static_cast<int>(mouse_clicked_y), 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &data);
			cur_pick_object_id = data[0];
			cur_pick_component_id = data[1];
		}

		front_polygon_mode = t_fpm;

	} pick_rt->un_use();

}
void GameManager::base_pass() {
	
	// switch hdr rt
	auto t_scene_rt = scene_rt;
	auto t_msaa_rt = msaa_rt;
	if (b_hdr) { t_scene_rt = hdr_scene_rt; t_msaa_rt = hdr_msaa_rt; }

	if (b_msaa) { t_msaa_rt->use(); } else { t_scene_rt->use(); } {
		draw_init();

		// draw all objects
		if (b_explode) { stack_shaders->push(AssetManager_ins().get_shader("explode")); } {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
				if (b_explode) { stack_shaders->top()->set_float("u_time", TimeManager_ins().cur_runtime_seconds()); }
			}
			draw_lights(stack_shaders->top());			// set light uniform for shader
			draw_all_objs(stack_shaders->top());
		} if (b_explode) { stack_shaders->pop(); }

		stack_shaders->push(AssetManager_ins().get_shader("pbr_ibl")); {
			stack_shaders->top()->use();
			
			stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			stack_shaders->top()->set_int("u_irradiance_diffuse_map", 9);
			if (sky_box) sky_box->get_texture()->bind(9);
			if (b_dynamic_env && dynamic_environment_map_rt && dynamic_environment_map_rt->get_attach_texture_3ds().size() > 0) {
				if (dynamic_environment_map_rt->get_attach_texture_3ds()[0].texture) {
					dynamic_environment_map_rt->get_attach_texture_3ds()[0].texture->bind(9);
				}
			}	// 使用动态环境贴图

			// set uniform for pbr
			draw_lights(stack_shaders->top());			// set light uniform for shader
			draw_pbr_objs(stack_shaders->top());
		} stack_shaders->pop();

		// draw border
		/*stack_shaders->push(AssetManager_ins().get_shader("solid_color")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
				stack_shaders->top()->set_vec4("u_solid_color", border_color.rgba_f());
			}
			glStencilFunc(GL_NOTEQUAL, 1, 0xff);			// 之前置 1 了的 不通过
			glStencilMask(0x00);							// 不允许写入模板缓冲
			//glDisable(GL_DEPTH_TEST);
			draw_border(stack_shaders->top());
			//glEnable(GL_DEPTH_TEST);
			glStencilMask(0xff);							// 重新允许写入模板缓冲

		} stack_shaders->pop();
		*/

		// 法线可视化
		if (b_normal_visual) { normal_visual_pass(); }

		// draw sky box -- if have -- 最后画
		if (b_skybox && sky_box) {
			//set_cull_face(true, GL_FRONT);
			stack_shaders->push(AssetManager_ins().get_shader("skybox")); {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					// 要移除矩阵的移动部分
					auto t_view = CMatrix4x4(CMatrix3x3(main_camera->get_camera_component()->get_view_mat().data(), 4, 4).data(), 3, 3);
					stack_shaders->top()->set_mat4("u_view_sky_box", t_view);
					stack_shaders->top()->set_int("u_texture", 0);
				}
				auto t_texture = sky_box->get_texture();
				if (t_texture) t_texture->bind(0);
				//point_light_shadow_rts[0]->get_attach_texture_3ds()[0].texture->bind(0);
				//dynamic_environment_map_rt->get_attach_texture_3ds()[0].texture->bind(0);
				hdr_environment_map_rt->get_attach_texture_3ds()[0].texture->bind(0);
				draw_skybox(stack_shaders->top());
				Texture3D::un_bind(0);
			} stack_shaders->pop();
		}

	} if (b_msaa) { t_msaa_rt->un_use(); } else { t_scene_rt->un_use(); }

	// blit msaa_rt to scene_rt -- color buffer is stored in scene_texture -- use inner msaa alogrithm
	if (b_msaa) { 
		if (!b_msaa_custom) { blit(t_msaa_rt, t_scene_rt, viewport_info.width, viewport_info.heigh); } // opengl 自带抗锯齿
		else {
			t_scene_rt->use(); {
				draw_init();
				stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
					if (stack_shaders->top() && t_msaa_rt->get_attach_textures().size() > 0) {
						auto t_texture = t_msaa_rt->get_attach_textures()[0].texture;
						if (t_texture) { t_texture->bind(0); }
						stack_shaders->top()->use();
						stack_shaders->top()->set_int("u_texture_msaa", 0);
						stack_shaders->top()->set_int("u_msaa_num", t_msaa_rt->get_msaa_num());
					}
					draw_scene(stack_shaders->top());
				} stack_shaders->pop();
			} t_scene_rt->un_use();

		} // 自定义抗锯齿

	}
	
	// update scene texture
	if (t_scene_rt->get_attach_textures().size() > 0) {
		scene_texture = t_scene_rt->get_attach_textures()[0].texture;
	}
}
void GameManager::border_pass() {
	// 先得到需要加 border 物体的深度图
	border_depth_rt->use(); {
		draw_init();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		// 此时背景色不应受 bg color 影响
		glClear(GL_COLOR_BUFFER_BIT);
		stack_shaders->push(AssetManager_ins().get_shader("depth")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_float("u_near", main_camera->get_camera_component()->get_camera_data()->get_frustum().near);
				stack_shaders->top()->set_float("u_far", main_camera->get_camera_component()->get_camera_data()->get_frustum().far);
			}
			draw_border(stack_shaders->top());
		} stack_shaders->pop();
	} border_depth_rt->un_use();

	// 再将上面得到的图随 scene_texture 一起传入, 后处理得最终的 scene_texture
	border_rt->use(); {
		draw_init();
		stack_shaders->push(AssetManager_ins().get_shader("border")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_int("u_texture", 0);
				stack_shaders->top()->set_int("u_border_texture", 1);
				stack_shaders->top()->set_vec4("u_border_color", border_color.rgba_f());
			}
			if (scene_texture) { scene_texture->bind(0); }
			if (b_depth) { if (depth_texture) { depth_texture->bind(0); } }
			if (border_depth_rt->get_attach_textures().size() > 0) { 
				border_depth_rt->get_attach_textures()[0].texture->bind(1); 
			}
			draw_scene(stack_shaders->top());
		} stack_shaders->pop();
	} border_rt->un_use();

	if (border_rt->get_attach_textures().size() > 0) {
		scene_texture = border_rt->get_attach_textures()[0].texture;
		if (b_depth) { depth_texture = border_rt->get_attach_textures()[0].texture; }
	}
	
}
void GameManager::post_process_pass() {
	auto t_pp_rt = pp_rt;
	if (b_hdr) { t_pp_rt = hdr_pp_rt; }

	t_pp_rt->use(); {
		draw_init();

		stack_shaders->push(AssetManager_ins().get_shader("pp")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				if (scene_texture) { scene_texture->bind(0); }
				if (b_depth) { if (depth_texture) { depth_texture->bind(0); } } 
				stack_shaders->top()->set_int("u_texture", 0);
				stack_shaders->top()->set_int("u_pp_type", static_cast<int>(pp_type));
			}

			draw_scene(stack_shaders->top());

		} stack_shaders->pop();

	} t_pp_rt->un_use();

	// update scene texture
	if (t_pp_rt->get_attach_textures().size() > 0) {
		scene_texture = t_pp_rt->get_attach_textures()[0].texture;
		if (b_depth) { depth_texture = t_pp_rt->get_attach_textures()[0].texture; }
	}
}
void GameManager::hdr_pass() {

	hdr_rt->use(); {
		draw_init();

		stack_shaders->push(AssetManager_ins().get_shader("hdr")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				if (scene_texture) { scene_texture->bind(0); }
				stack_shaders->top()->set_int("u_texture", 0);
				stack_shaders->top()->set_int("u_type", static_cast<int>(hdr_type));
				stack_shaders->top()->set_float("u_exposure", hdr_exposure);
			}

			draw_scene(stack_shaders->top());

		} stack_shaders->pop();

	} hdr_rt->un_use();

	// update scene texture
	if (hdr_rt->get_attach_textures().size() > 0) {
		scene_texture = hdr_rt->get_attach_textures()[0].texture;
	}
}
void GameManager::gamma_pass() {

	gamma_rt->use(); {
		draw_init();

		stack_shaders->push(AssetManager_ins().get_shader("gamma")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				if (scene_texture) { scene_texture->bind(0); }
				stack_shaders->top()->set_int("u_texture", 0);
				stack_shaders->top()->set_float("u_gamma", v_gamma);
			}

			draw_scene(stack_shaders->top());

		} stack_shaders->pop();

	} gamma_rt->un_use();

	// update scene texture
	if (gamma_rt->get_attach_textures().size() > 0) {
		scene_texture = gamma_rt->get_attach_textures()[0].texture;
	}

}

void GameManager::normal_visual_pass() {
	stack_shaders->push(AssetManager_ins().get_shader("normal_visual")); {
		if (stack_shaders->top()) {
			stack_shaders->top()->use();
			stack_shaders->top()->set_vec4("u_solid_color", CVector4D(0.0f, 1.0f, 1.0f, 1.0f));
		}
		draw_all_objs(stack_shaders->top());
	} stack_shaders->pop();

}
void GameManager::shader_toy_pass() {

	for (uint i = 0; i < 4; ++i) {
		auto stbr = shader_toy_buffer_rts[i];

		stbr->use(); {
			draw_init();

			std::string t_name = "shader_toy_buffer_" + StringHelper_ins().char_to_string(static_cast<char>(i + 'a'));

			stack_shaders->push(AssetManager_ins().get_shader(t_name)); {
				if (stack_shaders->top()) {
					stack_shaders->top()->set_vec3("iResolution", CVector3D(viewport_info.width, viewport_info.heigh, 0.0f));

				}
				draw_scene(stack_shaders->top());

			} stack_shaders->pop();

		} stbr->un_use();
	}

	shader_toy_rt->use(); {
		draw_init();
	
		stack_shaders->push(AssetManager_ins().get_shader("shader_toy_img")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("iResolution", CVector3D(viewport_info.width, viewport_info.heigh, 0.0f));
				stack_shaders->top()->set_float("iTime", TimeManager_ins().cur_runtime_seconds());
			}
			draw_scene(stack_shaders->top());

		} stack_shaders->pop();
	
	} shader_toy_rt->un_use();

	if (shader_toy_rt->get_attach_textures().size() > 0) {
		scene_texture = shader_toy_rt->get_attach_textures()[0].texture;
	}

}

void GameManager::vr_depth_pass() {
	auto t_depth_rt = vr_depth_rt;
	if (b_msaa) { t_depth_rt = vr_depth_msaa_rt; }

	t_depth_rt->use(); {
		// left
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			draw_init();

			// cac left view mat
			CMatrix4x4 t_mat_view;
			auto t_eye = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_right_axis() * vr_delta*0.01f;
			auto t_center = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_front_axis() * vr_delta*0.65f;
			if (CMath_ins().fcmp(vr_delta, 0.0f) == 0) { t_center = t_eye + main_camera->get_camera_component()->get_front_axis(); }
			auto t_world_up = main_camera->get_camera_component()->get_world_up();
			t_mat_view.lookAt(t_eye, t_center, t_world_up);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_mat_view.data());

			stack_shaders->push(AssetManager_ins().get_shader("depth")); {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					stack_shaders->top()->set_float("u_near", main_camera->get_camera_component()->get_camera_data()->get_frustum().near);
					stack_shaders->top()->set_float("u_far", main_camera->get_camera_component()->get_camera_data()->get_frustum().far);
				}
				draw_all_objs(stack_shaders->top());
			} stack_shaders->pop();
		}
		// right
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
			draw_init();

			// cac left view mat
			CMatrix4x4 t_mat_view;
			auto t_eye = main_camera->get_root_component()->get_location()
				- main_camera->get_camera_component()->get_right_axis() * vr_delta*0.01f;
			auto t_center = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_front_axis() * vr_delta*0.65f;
			if (CMath_ins().fcmp(vr_delta, 0.0f) == 0) { t_center = t_eye + main_camera->get_camera_component()->get_front_axis(); }
			auto t_world_up = main_camera->get_camera_component()->get_world_up();
			t_mat_view.lookAt(t_eye, t_center, t_world_up);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_mat_view.data());

			stack_shaders->push(AssetManager_ins().get_shader("depth")); {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					stack_shaders->top()->set_float("u_near", main_camera->get_camera_component()->get_camera_data()->get_frustum().near);
					stack_shaders->top()->set_float("u_far", main_camera->get_camera_component()->get_camera_data()->get_frustum().far);
				}
				draw_all_objs(stack_shaders->top());
			} stack_shaders->pop();
		}
	} t_depth_rt->un_use();

	if (b_msaa) {
		if (!b_msaa_custom) {
			blit(t_depth_rt, vr_depth_rt, viewport_info.width, viewport_info.heigh);
			blit(t_depth_rt, vr_depth_rt, viewport_info.width, viewport_info.heigh, 1, 1);
		} // opengl 自带抗锯齿
		else {
			vr_depth_rt->use(); {
				// left eye
				{
					glDrawBuffer(GL_COLOR_ATTACHMENT0);
					draw_init();
					stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
						if (stack_shaders->top() && vr_depth_msaa_rt->get_attach_textures().size() > 0) {
							auto t_texture = vr_depth_msaa_rt->get_attach_textures()[0].texture;
							if (t_texture) { t_texture->bind(0); }
							stack_shaders->top()->use();
							stack_shaders->top()->set_int("u_texture_msaa", 0);
							stack_shaders->top()->set_int("u_msaa_num", vr_depth_msaa_rt->get_msaa_num());
						}
						draw_scene(stack_shaders->top());
					} stack_shaders->pop();
				}
				// right eye
				{
					glDrawBuffer(GL_COLOR_ATTACHMENT1);
					draw_init();
					stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
						if (stack_shaders->top() && vr_depth_msaa_rt->get_attach_textures().size() > 1) {
							auto t_texture = vr_depth_msaa_rt->get_attach_textures()[1].texture;
							if (t_texture) { t_texture->bind(0); }
							stack_shaders->top()->use();
							stack_shaders->top()->set_int("u_texture_msaa", 0);
							stack_shaders->top()->set_int("u_msaa_num", vr_depth_msaa_rt->get_msaa_num());
						}
						draw_scene(stack_shaders->top());
					} stack_shaders->pop();
				}
			} vr_depth_rt->un_use();

		} // 自定义抗锯齿
	}

	// mix vr depth rt
	vr_depth_mix_rt->use(); {
		draw_init();

		stack_shaders->push(AssetManager_ins().get_shader("vr_mix")); {
			if (stack_shaders->top() && vr_depth_rt->get_attach_textures().size() >= 2) {
				auto t_left = vr_depth_rt->get_attach_textures()[0].texture;
				auto t_right = vr_depth_rt->get_attach_textures()[1].texture;
				if (t_left) { t_left->bind(0); }
				if (t_right) { t_right->bind(1); }
				stack_shaders->top()->use();
				stack_shaders->top()->set_int("u_texture_left", 0);
				stack_shaders->top()->set_int("u_texture_right", 1);
			}
			draw_scene(stack_shaders->top());

		} stack_shaders->pop();

	} vr_depth_mix_rt->un_use();

	if (vr_depth_mix_rt->get_attach_textures().size() > 0) {
		depth_texture = vr_depth_mix_rt->get_attach_textures()[0].texture;
	}

}
void GameManager::vr_base_pass() {
	auto t_vr_rt = vr_rt;
	auto t_vr_msaa_rt = vr_msaa_rt;
	auto t_vr_mix_rt = vr_mix_rt;
	if (b_hdr) { t_vr_rt = vr_hdr_rt; t_vr_msaa_rt = vr_hdr_msaa_rt; t_vr_mix_rt = vr_hdr_mix_rt; }

	// get left and right pic pass
	if (b_msaa) { t_vr_msaa_rt->use(); } else { t_vr_rt->use(); } {

		// left eyes scene
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			draw_init();

			// cac left view mat
			CMatrix4x4 t_mat_view;
			auto t_eye = main_camera->get_root_component()->get_location() 
				+ main_camera->get_camera_component()->get_right_axis() * vr_delta*0.01f;
			auto t_center = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_front_axis() * vr_delta*0.65f;
			if (CMath_ins().fcmp(vr_delta, 0.0f) == 0) { t_center = t_eye + main_camera->get_camera_component()->get_front_axis(); }
			auto t_world_up = main_camera->get_camera_component()->get_world_up();
			t_mat_view.lookAt(t_eye, t_center, t_world_up);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_mat_view.data());

			// draw all objects
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			}
			draw_lights(stack_shaders->top());			// set light uniform for shader
			draw_all_objs(stack_shaders->top());

			// normal visual
			if (b_normal_visual) { normal_visual_pass(); }

			// draw sky box -- if have -- 最后画
			if (b_skybox && sky_box) {
				//set_cull_face(true, GL_FRONT);
				stack_shaders->push(AssetManager_ins().get_shader("skybox")); {
					if (stack_shaders->top()) {
						stack_shaders->top()->use();
						// 要移除矩阵的移动部分
						auto t_view = CMatrix4x4(CMatrix3x3(main_camera->get_camera_component()->get_view_mat().data(), 4, 4).data(), 3, 3);
						t_view.translate(main_camera->get_camera_component()->get_right_axis() * 0.01f);
						stack_shaders->top()->set_mat4("u_view_sky_box", t_view);
						stack_shaders->top()->set_int("u_texture", 0);
					}
					auto t_texture = sky_box->get_texture();
					if (t_texture) t_texture->bind(0);
					draw_skybox(stack_shaders->top());
				} stack_shaders->pop();
			}

		}

		// right eyes scene
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
			draw_init();

			CMatrix4x4 t_mat_view;
			auto t_eye = main_camera->get_root_component()->get_location()
				- main_camera->get_camera_component()->get_right_axis() * vr_delta * 0.01f;
			auto t_center = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_front_axis() * vr_delta*0.65f;
			if (CMath_ins().fcmp(vr_delta, 0.0f) == 0) { t_center = t_eye + main_camera->get_camera_component()->get_front_axis(); }
			auto t_world_up = main_camera->get_camera_component()->get_world_up();
			t_mat_view.lookAt(t_eye, t_center, t_world_up);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_mat_view.data());

			// draw all objects
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
				
			}
			draw_lights(stack_shaders->top());			// set light uniform for shader
			draw_all_objs(stack_shaders->top());

			// normal visual
			if (b_normal_visual) { normal_visual_pass(); }

			// draw sky box -- if have -- 最后画
			if (b_skybox && sky_box) {
				//set_cull_face(true, GL_FRONT);
				stack_shaders->push(AssetManager_ins().get_shader("skybox")); {
					if (stack_shaders->top()) {
						stack_shaders->top()->use();
						// 要移除矩阵的移动部分
						auto t_view = CMatrix4x4(CMatrix3x3(main_camera->get_camera_component()->get_view_mat().data(), 4, 4).data(), 3, 3);
						t_view.translate(main_camera->get_camera_component()->get_right_axis() * -0.01f);
						stack_shaders->top()->set_mat4("u_view_sky_box", t_view);
						stack_shaders->top()->set_int("u_texture", 0);
					}
					auto t_texture = sky_box->get_texture();
					if (t_texture) t_texture->bind(0);
					draw_skybox(stack_shaders->top());
				} stack_shaders->pop();
				//set_cull_face(true);
			}


		}

	} if (b_msaa) { t_vr_msaa_rt->un_use(); } else { t_vr_rt->un_use(); }

	if(b_msaa) {
		if (!b_msaa_custom) {
			blit(t_vr_msaa_rt, t_vr_rt, viewport_info.width, viewport_info.heigh);
			blit(t_vr_msaa_rt, t_vr_rt, viewport_info.width, viewport_info.heigh, 1, 1);
		} // opengl 自带抗锯齿
		else {
			t_vr_rt->use(); {
				// left eye
				{
					glDrawBuffer(GL_COLOR_ATTACHMENT0);
					draw_init();
					stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
						if (stack_shaders->top() && t_vr_msaa_rt->get_attach_textures().size() > 0) {
							auto t_texture = t_vr_msaa_rt->get_attach_textures()[0].texture;
							if (t_texture) { t_texture->bind(0); }
							stack_shaders->top()->use();
							stack_shaders->top()->set_int("u_texture_msaa", 0);
							stack_shaders->top()->set_int("u_msaa_num", t_vr_msaa_rt->get_msaa_num());
						}
						draw_scene(stack_shaders->top());
					} stack_shaders->pop();
				}
				// right eye
				{
					glDrawBuffer(GL_COLOR_ATTACHMENT1);
					draw_init();
					stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
						if (stack_shaders->top() && t_vr_msaa_rt->get_attach_textures().size() > 1) {
							auto t_texture = t_vr_msaa_rt->get_attach_textures()[1].texture;
							if (t_texture) { t_texture->bind(0); }
							stack_shaders->top()->use();
							stack_shaders->top()->set_int("u_texture_msaa", 0);
							stack_shaders->top()->set_int("u_msaa_num", t_vr_msaa_rt->get_msaa_num());
						}
						draw_scene(stack_shaders->top());
					} stack_shaders->pop();
				}
			} t_vr_rt->un_use();

		} // 自定义抗锯齿
	}

	// mix pass -- mix left and right pic
	t_vr_mix_rt->use(); {
		draw_init();

		stack_shaders->push(AssetManager_ins().get_shader("vr_mix")); {
			if (stack_shaders->top() && t_vr_rt->get_attach_textures().size() >= 2) {
				auto t_left = t_vr_rt->get_attach_textures()[0].texture;
				auto t_right = t_vr_rt->get_attach_textures()[1].texture;
				if (t_left) { t_left->bind(0); }
				if (t_right) { t_right->bind(1); }
				stack_shaders->top()->use();
				stack_shaders->top()->set_int("u_texture_left", 0);
				stack_shaders->top()->set_int("u_texture_right", 1);
			}
			draw_scene(stack_shaders->top());

		} stack_shaders->pop();

	} t_vr_mix_rt->un_use();

	if (t_vr_mix_rt->get_attach_textures().size() > 0) {
		scene_texture = t_vr_mix_rt->get_attach_textures()[0].texture;
	}
}
void GameManager::vr_border_pass() {
	
	// 先得到需要加 border 物体的深度图
	vr_border_depth_rt->use(); {
		// left
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			draw_init();

			// cac left view mat
			CMatrix4x4 t_mat_view;
			auto t_eye = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_right_axis() * vr_delta * 0.01f;
			auto t_center = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_front_axis() * vr_delta*0.65f;
			if (CMath_ins().fcmp(vr_delta, 0.0f) == 0) { t_center = t_eye + main_camera->get_camera_component()->get_front_axis(); }
			auto t_world_up = main_camera->get_camera_component()->get_world_up();
			t_mat_view.lookAt(t_eye, t_center, t_world_up);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_mat_view.data());

			stack_shaders->push(AssetManager_ins().get_shader("depth")); {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					stack_shaders->top()->set_float("u_near", main_camera->get_camera_component()->get_camera_data()->get_frustum().near);
					stack_shaders->top()->set_float("u_far", main_camera->get_camera_component()->get_camera_data()->get_frustum().far);
				}
				draw_border(stack_shaders->top());
			} stack_shaders->pop();
		}

		// right
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
			draw_init();

			// cac left view mat
			CMatrix4x4 t_mat_view;
			auto t_eye = main_camera->get_root_component()->get_location()
				- main_camera->get_camera_component()->get_right_axis() * vr_delta*0.01f;
			auto t_center = main_camera->get_root_component()->get_location()
				+ main_camera->get_camera_component()->get_front_axis() * vr_delta*0.65f;
			if (CMath_ins().fcmp(vr_delta, 0.0f) == 0) { t_center = t_eye + main_camera->get_camera_component()->get_front_axis(); }
			auto t_world_up = main_camera->get_camera_component()->get_world_up();
			t_mat_view.lookAt(t_eye, t_center, t_world_up);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_mat_view.data());

			stack_shaders->push(AssetManager_ins().get_shader("depth")); {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					stack_shaders->top()->set_float("u_near", main_camera->get_camera_component()->get_camera_data()->get_frustum().near);
					stack_shaders->top()->set_float("u_far", main_camera->get_camera_component()->get_camera_data()->get_frustum().far);
				}
				draw_border(stack_shaders->top());
			} stack_shaders->pop();
		}
		
	} vr_border_depth_rt->un_use();

	// mix vr border depth
	vr_border_mix_rt->use(); {
		draw_init();

		stack_shaders->push(AssetManager_ins().get_shader("vr_mix")); {
			if (stack_shaders->top() && vr_border_depth_rt->get_attach_textures().size() >= 2) {
				auto t_left = vr_border_depth_rt->get_attach_textures()[0].texture;
				auto t_right = vr_border_depth_rt->get_attach_textures()[1].texture;
				if (t_left) { t_left->bind(0); }
				if (t_right) { t_right->bind(1); }
				stack_shaders->top()->use();
				stack_shaders->top()->set_int("u_texture_left", 0);
				stack_shaders->top()->set_int("u_texture_right", 1);
			}
			draw_scene(stack_shaders->top());

		} stack_shaders->pop();

	} vr_border_mix_rt->un_use();

	// 再将上面得到的图随 scene_texture 一起传入, 后处理得最终的 scene_texture
	vr_border_rt->use(); {
		stack_shaders->push(AssetManager_ins().get_shader("border")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_int("u_texture", 0);
				stack_shaders->top()->set_int("u_border_texture", 1);
				stack_shaders->top()->set_vec4("u_border_color", border_color.rgba_f());
			}
			if (scene_texture) { scene_texture->bind(0); }
			if (b_depth) { if (depth_texture) { depth_texture->bind(0); } }
			if (vr_border_mix_rt->get_attach_textures().size() > 0) {
				vr_border_mix_rt->get_attach_textures()[0].texture->bind(1);
			}
			draw_scene(stack_shaders->top());
		} stack_shaders->pop();
	} vr_border_rt->un_use();

	if (vr_border_rt->get_attach_textures().size() > 0) {
		scene_texture = vr_border_rt->get_attach_textures()[0].texture;
		if (b_depth) { depth_texture = vr_border_rt->get_attach_textures()[0].texture; }
	}
	
}

void GameManager::hdr_to_cubemap_pass() {
	glViewport(0, 0, 1024, 1024); {
		hdr_environment_map_rt->use(); {

			/*stack_shaders->push(AssetManager_ins().get_shader("dynamic_env"));*/ {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					// 设置为此光源的 view and proj 矩阵
					CMatrix4x4 t_proj;
					t_proj.perspective(90.0f, 1.0f, 0.1f, 100.0f);
					ub_matrices->fill_data(CMatrix4x4::data_size(), CMatrix4x4::data_size(), t_proj.data());
					//auto t_loc = t_light->get_root_component()->get_location();
					auto t_loc = CVector3D(0.0f, 1.0f, 0.0f);
					// 6个方向
					for (int j = 0; j < 6; ++j) {
						hdr_environment_map_rt->use_texture_3d(0, j);
						hdr_environment_map_rt->use_texture_3d(1, j);

						draw_init();

						CMatrix4x4 t_view;
						t_view.lookAt(t_loc, t_loc + point_front[j], point_world_up[j]);
						ub_matrices->fill_data(0, CMatrix4x4::data_size(), t_view.data());

						// draw sky box
						if (b_skybox && sky_box) {
							stack_shaders->push(AssetManager_ins().get_shader("hdr_to_skybox")); {
								if (stack_shaders->top()) {
									stack_shaders->top()->use();
									// 要移除矩阵的移动部分
									auto t_view_s = CMatrix4x4(CMatrix3x3(t_view.data(), 4, 4).data(), 3, 3);
									stack_shaders->top()->set_mat4("u_view_sky_box", t_view_s);
									stack_shaders->top()->set_int("u_texture_hdr", 0);
								}
								auto t_texture = sky_box->get_texture_hdr();
								if (t_texture) t_texture->bind(0);
								draw_skybox(stack_shaders->top());
							} stack_shaders->pop();
						}
					}
				}

				ub_matrices->fill_data(CMatrix4x4::data_size(), CMatrix4x4::data_size(), main_camera->get_camera_component()->get_proj_mat().data());
				ub_matrices->fill_data(0, CMatrix4x4::data_size(), main_camera->get_camera_component()->get_view_mat().data());

			} /*stack_shaders->pop();*/
		} hdr_environment_map_rt->un_use();

	} glViewport(0, 0, viewport_info.width, viewport_info.heigh);
}

void GameManager::draw_scene(SPTR_Shader shader) {
	glDisable(GL_DEPTH_TEST);
	static auto s_mesh = AssetManager_ins().get_mesh("rect");
	if (s_mesh) {
		s_mesh->set_use_default_mt(false);
		s_mesh->draw(shader);
		s_mesh->set_use_default_mt(true);
	}
	glEnable(GL_DEPTH_TEST);
}
void GameManager::draw_all_objs(SPTR_Shader shader) {

	set_depth_test();
	set_polygon_mode(front_polygon_mode, back_polygon_mode); {
		for (auto go : game_objects) { go.second->_draw(shader); }

		/*for (auto go : game_objects) {
			if (go.second->get_id() != cur_pick_object_id) {
				//glStencilFunc(GL_ALWAYS, 0, 0xff);
				go.second->_draw(shader);
			} // 总是通过测试 -- 且通过后置0
		} // 先画没被选中的 -- 保证最后写入的 1 不被 0 覆盖
		for (auto go : game_objects) {
			if (go.second->get_id() == cur_pick_object_id) {
				//glStencilFunc(GL_ALWAYS, 1, 0xff);
				go.second->_draw(shader);
			} // 总是通过测试 -- 且通过后置1
		}
		*/
	} set_polygon_mode();
}
void GameManager::draw_pbr_objs(SPTR_Shader shader) {
	set_depth_test();
	set_polygon_mode(front_polygon_mode, back_polygon_mode); {
		for (auto go : game_objects_pbr) { go.second->_draw(shader); }
	} set_polygon_mode();
}
void GameManager::draw_border(SPTR_Shader shader) {
	set_polygon_mode(front_polygon_mode, back_polygon_mode);
	for (auto go : game_objects) {
		if (go.second->get_id() == cur_pick_object_id) {
			//go.second->get_root_component()->set_all_border(true);
			go.second->_draw(shader);
			//go.second->get_root_component()->set_all_border(false);
		}	// 拾取 -- 绘制边框
	}
	set_polygon_mode();
}
void GameManager::draw_lights(SPTR_Shader shader) {
	if (!shader) { return; }
	shader->set_bool("u_shadow_b_use", b_shadow);
	shader->set_bool("u_normal_map_b_use", b_normal_map);

	// direct light
	{
		int t_id = 0;
		for (auto t_p : map_direct_lights) {
			auto t_light = t_p.second;
			if (t_light) {
				t_light->get_light_component()->set_id(t_id);
				t_light->use(shader);

				// set shadow
				if (b_shadow) {
					std::string t_name = "u_light_direct_proj_view["
						+ StringHelper_ins().int_to_string(t_light->get_light_component()->get_id()) + "]";
					shader->set_mat4(t_name, t_light->get_light_component()->get_mat_proj_view());

					if (direct_light_shadow_rts.size() > t_id) {
						auto t_v = direct_light_shadow_rts[t_id];
						if (t_v && t_v->get_attach_textures().size() > 0) {
							t_v->get_attach_textures()[0].texture->bind(shadow_direct_0_id + t_id);
							std::string t_rt_name = "u_shadow_direct_map_"
								+ StringHelper_ins().int_to_string(t_id);
							shader->use();
							shader->set_int(t_rt_name, shadow_direct_0_id + t_id);
						}
					}
				}
				else { Texture2D::un_bind(shadow_direct_0_id + t_id); }
				++t_id;
			}
			
		} shader->set_int("u_light_direct_num", t_id);
	}
	// point light
	{
		{
			auto t_t3d = CREATE_CLASS(Texture3D);
			std::vector<TextureData> datas(6);
			std::vector<SPTR_uchar> ds(6);
			t_t3d->init(datas, ds);
			t_t3d->bind(4);
			std::string t_rt_name = "u_shadow_point_map_0";
			shader->use();
			shader->set_int(t_rt_name, shadow_point_0_id);
		}

		int t_id = 0;
		for (auto t_p : map_point_lights) {
			auto t_light = t_p.second;
			if (t_light) {
				t_light->get_light_component()->set_id(t_id);
				t_light->use(shader);

				// set shadow
				if (b_shadow) {
					if (point_light_shadow_rts.size() > t_id) {
						auto t_v = point_light_shadow_rts[t_id];
						if (t_v && t_v->get_attach_texture_3ds().size() > 0) {
							t_v->get_attach_texture_3ds()[0].texture->bind(shadow_point_0_id + t_id);
							std::string t_rt_name = "u_shadow_point_map_"
								+ StringHelper_ins().int_to_string(t_id);
							shader->use();
							shader->set_int(t_rt_name, shadow_point_0_id + t_id);
							shader->set_float("u_far", 100.0f);
						}
					}
				}
				else {
					Texture3D::un_bind(shadow_point_0_id + t_id);
				}
				++t_id;
			}
		} shader->set_int("u_light_point_num", t_id);
	}

	// spots light
	{
		int t_id = 0;
		for (auto t_p : map_spots_lights) {
			auto t_light = t_p.second;
			if (t_light) {
				t_light->get_light_component()->set_id(t_id++);
				t_light->use(shader);
			}
		} shader->set_int("u_light_spot_num", t_id);
	}

}
void GameManager::draw_init() {
	glClearColor(background_color.r_f(), background_color.g_f(), background_color.b_f(), background_color.a_f());
	glClear(GL_COLOR_BUFFER_BIT);
	if (glIsEnabled(GL_DEPTH_TEST)) { glClear(GL_DEPTH_BUFFER_BIT); }
	if (glIsEnabled(GL_STENCIL_TEST)) { glClear(GL_STENCIL_BUFFER_BIT); }
}

void GameManager::draw_skybox(SPTR_Shader shader) {
	// <= 缓冲区的值时通过 -- 提前深度测试时写入了 1.0 -- 所以要有等于 -- 最后画以提高性能
	set_depth_test(true, GL_LEQUAL); {
		sky_box->_draw(shader);
	} set_depth_test(true);
}

void GameManager::init_rt_static() {
	b_pre_bake = true;

	// hdr_environment_map_rt 
	
	if (hdr_environment_map_rt) hdr_environment_map_rt.reset();
	hdr_environment_map_rt = CREATE_CLASS(RenderTarget);
	if (hdr_environment_map_rt) {
		hdr_environment_map_rt
			->add_attach_texture_3d(GL_COLOR_ATTACHMENT0, 1024, 1024,
				GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, GL_FLOAT)
			->add_attach_texture_3d(GL_DEPTH_ATTACHMENT, 1024, 1024,
				GL_TEXTURE_CUBE_MAP, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT)
			->init();
	}

}
void GameManager::init_rt() {
	if (scene_rt) scene_rt.reset();
	scene_rt = CREATE_CLASS(RenderTarget);
	if (scene_rt) {
		if (!scene_rt->init_normal(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	if (depth_rt) depth_rt.reset();
	depth_rt = CREATE_CLASS(RenderTarget);
	if (depth_rt) {
		if (!depth_rt->init_normal(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}
	if (depth_msaa_rt) depth_msaa_rt.reset();
	depth_msaa_rt = CREATE_CLASS(RenderTarget);
	if (depth_msaa_rt) {
		if (!depth_msaa_rt->init_normal_multisample(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	if (pp_rt) pp_rt.reset();
	pp_rt = CREATE_CLASS(RenderTarget);
	if (pp_rt) {
		if (!pp_rt->init_normal(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	if (gamma_rt) gamma_rt.reset();
	gamma_rt = CREATE_CLASS(RenderTarget);
	if (gamma_rt) {
		if (!gamma_rt->init_normal(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	init_pick_rt();
	init_vr_rt();
	init_shader_toy_rt();
	init_msaa_rt();
	init_hdr_rt();
	init_shadow_rt();
	init_skybox_rt();
}
void GameManager::init_shadow_rt() {
	// direct light
	direct_light_shadow_rts.resize(light_direct_num_max);
	for (auto& shadow_rt : direct_light_shadow_rts) {
		if (shadow_rt) shadow_rt.reset();
		shadow_rt = CREATE_CLASS(RenderTarget);
		if (shadow_rt) {
			shadow_rt->add_attach_texture(GL_DEPTH_ATTACHMENT, 1024, 1024,
				GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT)
				->init();
			// set border
			auto t_tex = shadow_rt->get_attach_textures()[0].texture;
			if (t_tex) {
				t_tex->use(); {
					glTexParameteri(t_tex->get_type(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(t_tex->get_type(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					GLfloat t_border_color[] = { 1.0, 1.0, 1.0, 1.0 };
					glTexParameterfv(t_tex->get_type(), GL_TEXTURE_BORDER_COLOR, t_border_color);
				} t_tex->un_use();
			}
		}
	}

	// point light
	point_light_shadow_rts.resize(light_point_num_max);
	for (auto& shadow_rt : point_light_shadow_rts) {
		if (shadow_rt) shadow_rt.reset();
		shadow_rt = CREATE_CLASS(RenderTarget);
		if (shadow_rt) {
			shadow_rt->add_attach_texture_3d(GL_DEPTH_ATTACHMENT, 1024, 1024,
				GL_TEXTURE_CUBE_MAP, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT)
				->init();
			// set border -- cube map needn't
			/*auto t_tex = shadow_rt->get_attach_texture_3ds()[0].texture;
			if (t_tex) {
				t_tex->use(); {
					glTexParameteri(t_tex->get_type(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(t_tex->get_type(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
					glTexParameteri(t_tex->get_type(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
					GLfloat t_border_color[] = { 1.0, 1.0, 1.0, 1.0 };
					glTexParameterfv(t_tex->get_type(), GL_TEXTURE_BORDER_COLOR, t_border_color);
				} t_tex->un_use();
			}*/
		}
	}

}
void GameManager::init_pick_rt() {
	if(pick_rt) pick_rt.reset();
	pick_rt = CREATE_CLASS(RenderTarget);
	if (pick_rt) {
		pick_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}

	if (border_depth_rt) border_depth_rt.reset();
	border_depth_rt = CREATE_CLASS(RenderTarget);
	if (border_depth_rt) {
		if (!border_depth_rt->init_normal_f(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	if (border_rt) border_rt.reset();
	border_rt = CREATE_CLASS(RenderTarget);
	if (border_rt) {
		if (!border_rt->init_normal_f(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}
}
void GameManager::init_msaa_rt() {
	if (msaa_rt) msaa_rt.reset();
	msaa_rt = CREATE_CLASS(RenderTarget);
	if (msaa_rt) { msaa_rt->init_normal_multisample(viewport_info.width, viewport_info.heigh); }
}
void GameManager::init_hdr_rt() {
	if (hdr_rt) hdr_rt.reset();
	hdr_rt = CREATE_CLASS(RenderTarget);
	if (hdr_rt) {
		if (!hdr_rt->init_normal_f(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	if (hdr_scene_rt) hdr_scene_rt.reset();
	hdr_scene_rt = CREATE_CLASS(RenderTarget);
	if (hdr_scene_rt) {
		if (!hdr_scene_rt->init_normal_f(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	if (hdr_pp_rt) hdr_pp_rt.reset();
	hdr_pp_rt = CREATE_CLASS(RenderTarget);
	if (hdr_pp_rt) {
		if (!hdr_pp_rt->init_normal_f(viewport_info.width, viewport_info.heigh)) {
			c_debuger() << "create rt fail";
		}
	}

	if (hdr_msaa_rt) hdr_msaa_rt.reset();
	hdr_msaa_rt = CREATE_CLASS(RenderTarget);
	if (hdr_msaa_rt) { hdr_msaa_rt->init_normal_multisample_f(viewport_info.width, viewport_info.heigh); }

}

void GameManager::init_skybox_rt() {
	if (dynamic_environment_map_rt) dynamic_environment_map_rt.reset();
	dynamic_environment_map_rt = CREATE_CLASS(RenderTarget);
	if (dynamic_environment_map_rt) {
		dynamic_environment_map_rt
			->add_attach_texture_3d(GL_COLOR_ATTACHMENT0, 512, 512,
				GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, GL_FLOAT)
			->add_attach_texture_3d(GL_DEPTH_ATTACHMENT, 512, 512,
				GL_TEXTURE_CUBE_MAP, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT)
			->init();
	}
}

void GameManager::init_vr_rt() {
	// init vr rt
	if (vr_rt) vr_rt.reset();
	vr_rt = CREATE_CLASS(RenderTarget);
	if (vr_rt) {
		vr_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}

	if (vr_msaa_rt) vr_msaa_rt.reset();
	vr_msaa_rt = CREATE_CLASS(RenderTarget);
	if (vr_msaa_rt) {
		vr_msaa_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh, true)
			->init();
	}

	if (vr_hdr_rt) vr_hdr_rt.reset();
	vr_hdr_rt = CREATE_CLASS(RenderTarget);
	if (vr_hdr_rt) {
		vr_hdr_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGBA16F)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGBA16F)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}

	if (vr_hdr_msaa_rt) vr_hdr_msaa_rt.reset();
	vr_hdr_msaa_rt = CREATE_CLASS(RenderTarget);
	if (vr_hdr_msaa_rt) {
		vr_hdr_msaa_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh, true)
			->init();
	}

	// init vr depth rt
	if (vr_depth_rt) vr_depth_rt.reset();
	vr_depth_rt = CREATE_CLASS(RenderTarget);
	if (vr_depth_rt) {
		vr_depth_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}
	if (vr_depth_msaa_rt) vr_depth_msaa_rt.reset();
	vr_depth_msaa_rt = CREATE_CLASS(RenderTarget);
	if (vr_depth_msaa_rt) {
		vr_depth_msaa_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh, true)
			->init();
	}
	if (vr_depth_mix_rt) vr_depth_mix_rt.reset();
	vr_depth_mix_rt = CREATE_CLASS(RenderTarget);
	if (vr_depth_mix_rt) { vr_depth_mix_rt->init_normal(viewport_info.width, viewport_info.heigh); }

	// init vr border rt
	if (vr_border_depth_rt) vr_border_depth_rt.reset();
	vr_border_depth_rt = CREATE_CLASS(RenderTarget);
	if (vr_border_depth_rt) {
		vr_border_depth_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGBA16F)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGBA16F)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}
	if (vr_border_mix_rt) vr_border_mix_rt.reset();
	vr_border_mix_rt = CREATE_CLASS(RenderTarget);
	if (vr_border_mix_rt) {
		vr_border_mix_rt->init_normal_f(viewport_info.width, viewport_info.heigh);
	}
	if (vr_border_rt) vr_border_rt.reset();
	vr_border_rt = CREATE_CLASS(RenderTarget);
	if (vr_border_rt) {
		vr_border_rt->init_normal_f(viewport_info.width, viewport_info.heigh);
	}

	// init vr mix rt
	if (vr_mix_rt) vr_mix_rt.reset();
	vr_mix_rt = CREATE_CLASS(RenderTarget);
	if (vr_mix_rt) {
		vr_mix_rt->init_normal(viewport_info.width, viewport_info.heigh);
	}

	if (vr_hdr_mix_rt) vr_hdr_mix_rt.reset();
	vr_hdr_mix_rt = CREATE_CLASS(RenderTarget);
	if (vr_hdr_mix_rt) {
		vr_hdr_mix_rt->init_normal_f(viewport_info.width, viewport_info.heigh);
	}

}

void GameManager::init_shader_toy_rt() {
	if (shader_toy_rt) shader_toy_rt.reset();
	shader_toy_rt = CREATE_CLASS(RenderTarget);
	if (shader_toy_rt) { shader_toy_rt->init_simple(viewport_info.width, viewport_info.heigh); }

	for (auto& stbr : shader_toy_buffer_rts) {
		if (stbr) stbr.reset();
		stbr = CREATE_CLASS(RenderTarget);
		if (stbr) { stbr->init_simple(viewport_info.width, viewport_info.heigh); }
	}
}

// ===========================================================================

void GameManager::pre_init(uint w, uint h) {
	viewport_info.width = w;
	viewport_info.heigh = h;
}
void GameManager::resize(uint w, uint h) {
	viewport_info.width = w;
	viewport_info.heigh = h;
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

	//projection.frustum(-0.1f, 0.1f, -0.1f / t, 0.1f / t, 0.1f, 100.0f);
	//projection.ortho(-10.0f, 10.0f, -10.0f / t, 10.0f / t, 0.01f, 1000.0f);
	//projection.ortho(20.0f, t, 0.01f, 100.0f);
	//projection.ortho_2d(0.0f, 20.0f, 0.0f / t, 20.0f /t);
	
	if (main_camera) {
		main_camera->get_camera_component()->get_camera_data()->get_frustum().width = w;
		main_camera->get_camera_component()->get_camera_data()->get_frustum().heigh = h;
		ub_matrices->fill_data(CMatrix4x4::data_size(), CMatrix4x4::data_size(), main_camera->get_camera_component()->get_proj_mat().data());
	}

	init_rt();
	
}
void GameManager::exit() {
	InputManager_ins().exit();
}

SPTR_CameraObject GameManager::set_main_camera() {
	auto free_camera = CREATE_CLASS(FreeCamera);
	add_game_object("free_camera", free_camera);
	free_camera->get_root_component()->set_location(0.0f, 1.5f, 10.0f);
	free_camera->get_root_component()->set_rotation(0.0f, 180.0f, 0.0f);

    return std::dynamic_pointer_cast<CameraObject>(free_camera);
}

void GameManager::set_depth_test(bool enable, uint depth_func, uint depth_mask) {
	if (enable) { glEnable(GL_DEPTH_TEST); }
	else { glDisable(GL_DEPTH_TEST); }

    glDepthMask(static_cast<GLboolean>(depth_mask));
	glDepthFunc(depth_func);
}
void GameManager::set_stencil_test(bool enable, uint func, uint ref, uint mask, uint fail, uint zfail, uint zpass) {
	if (enable) { glEnable(GL_STENCIL_TEST); }
	else { glDisable(GL_STENCIL_TEST); }
	
    glStencilFunc(func, static_cast<GLint>(ref), mask);
	glStencilOp(fail, zfail, zpass);
	
}
void GameManager::set_blend(bool enable, uint sfactor, uint dfactor) {
	if (enable) { glEnable(GL_BLEND); }
	else { glDisable(GL_BLEND); }

	glBlendFunc(sfactor, dfactor);
}
void GameManager::set_polygon_mode(uint front_mode, uint back_mode) {
	glPolygonMode(GL_FRONT, front_mode);
	glPolygonMode(GL_BACK, back_mode);
	glPolygonMode(GL_FRONT_AND_BACK, front_mode);			// GL_FRONT and GL_BACK is not work at 330
}
void GameManager::set_cull_face(bool enable, uint mode, uint front_face) {
	if (enable) { glEnable(GL_CULL_FACE); }
	else { glDisable(GL_CULL_FACE); }

	glCullFace(mode);
	glFrontFace(front_face);			// 默认逆时针 -- CCW -- 此状态同时也影响 polygon_mode
}

// ===========================================================================

GameManager* GameManager::instance = nullptr;
GameManager::GameManager() {
	assert(instance == nullptr);
	instance = this;
	_init();
	game_objects.clear();
}
GameManager::~GameManager() {
	assert(instance == this);
	instance = nullptr;
}
GameManager* GameManager::get_instance() {
	assert(instance);
	return instance;
}

void GameManager::add_game_object(const std::string& key, SPTR_GameObject value) {
	if (game_objects.count(key)) return;
	game_objects.insert(std::make_pair(key, value));
}
void GameManager::add_game_object_pbr(const std::string& key, SPTR_GameObject value) {
	if (game_objects_pbr.count(key)) return;
	game_objects_pbr.insert(std::make_pair(key, value));
}

void GameManager::add_direct_light(const std::string& key, SPTR_DirectLightObject value) {
	if (map_direct_lights.count(key)) return;
	if (map_direct_lights.size() >= light_direct_num_max) return;
	map_direct_lights.insert(std::make_pair(key, value));
}
void GameManager::add_point_light(const std::string& key, SPTR_PointLightObject value) {
	if (map_point_lights.count(key)) return;
	if (map_point_lights.size() >= light_point_num_max) return;
	map_point_lights.insert(std::make_pair(key, value));
}
void GameManager::add_spots_light(const std::string& key, SPTR_SpotLightObject value) {
	if (map_spots_lights.count(key)) return;
	if (map_spots_lights.size() >= light_spot_num_max) return;
	map_spots_lights.insert(std::make_pair(key, value));
}

void GameManager::main_bind_input() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it).second->bind_input();
	}
}
void GameManager::main_begin_play() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it).second->_begin_play();
	}
}
void GameManager::main_tick(float time) {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it).second->_tick(time);
	}
}
void GameManager::main_draw(SPTR_Shader shader) {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it).second->_draw(shader);
	}
}

void GameManager::map_input_default() {
	{
		InputState is;
		is.mouse_released = Qt::LeftButton;
		InputManager_ins().map_action("gm_m_r", is);
	}
	
}
void GameManager::bind_input_default() {
	IM_BIND_ACTION(gm_m_r, GameManager, this, &GameManager::mouse_released);
}

void GameManager::mouse_released() {
	// 造成一次单击事件
	// read pix
	float delta = (InputManager_ins().get_input_data().mouse_pressed_pos - InputManager_ins().get_input_data().mouse_cur_pos).manhattanLength();
	if (delta < CMath_ins().eps) {
		b_mouse_clicked = true;
        mouse_clicked_x = static_cast<uint>(InputManager_ins().get_input_data().mouse_cur_pos.x());
        mouse_clicked_y = viewport_info.heigh - static_cast<uint>(InputManager_ins().get_input_data().mouse_cur_pos.y() - 1);
	}
}

SPTR_Texture3D GameManager::get_skybox() { return sky_box->get_texture(); }
void GameManager::set_skybox(SPTR_Texture3D tex) { if (sky_box && tex) { sky_box->set_texture(tex); } }

void GameManager::set_default_shader(SPTR_Shader shader) { stack_shaders->set_shader_default(shader); }
SPTR_Shader GameManager::get_default_shader() { return stack_shaders->get_shader_default(); }

