#define GLEW_STATIC
#include <GL/glew.h>

#include "mesh.h"
#include "shader.h"
#include "texture2d.h"
#include "gameobject.h"
#include "freecamera.h"
#include "cameradata.h"
#include "rendertarget.h"
#include "uniformbuffer.h"
#include "cameracomponent.h"

#include "timemanager.h"
#include "gamemanager.h"
#include "assetmanager.h"
#include "inputmanager.h"

#include <chrono>					// get time
#include <assert.h>

#include <GLFW/glfw3.h>

void GameManager::_init() {
	// ui setting init
	{
		background_color = CColor(205, 220, 232);
		border_color = CColor(221, 161, 18);

		front_polygon_mode = GL_FILL;
		back_polygon_mode = GL_FILL;

		b_use_vr = false;
		vr_delta = 0.1f;

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
		//AssetManager_ins().load_shader("default", "resources/shaders/mvp_anim.vert", "resources/shaders/single_texture.frag");
		AssetManager_ins().load_shader("depth", "resources/shaders/mvp_anim.vert", "resources/shaders/depth.frag");
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

		AssetManager_ins().load_shader("shader_toy_img", "resources/shaders/scene_2d.vert", "resources/shaders/shadertoy_img.frag");
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
		//AssetManager_ins().load_texture("resources/textures/txt/texture_default.txt");
		AssetManager_ins().load_texture_x("resources/textures/texture_default.png");
		AssetManager_ins().load_texture_x("resources/textures/brickwall_d.jpg");

		load_asset();						// 加载资源
		
	}

	main_camera = set_main_camera();	// 绑定主相机

	// init shader stack 
	{
		stack_shaders = CREATE_CLASS(ShaderStack);
		stack_shaders->push(AssetManager_ins().get_shader("default"));	// default shader
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
		set_stencil_test(true, GL_NOTEQUAL, 1, 0xff, GL_KEEP, GL_REPLACE, GL_REPLACE);
		set_blend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		set_polygon_mode(front_polygon_mode, back_polygon_mode);
		set_cull_face();

		glEnable(GL_PROGRAM_POINT_SIZE);

		//glEnable(GL_FRAMEBUFFER_SRGB);

	}
	
	// init rt
	{
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
	
	// draw
	{
		// pass 0
		scene_pass();			// 渲染到默认缓冲 -- 必须第一个执行
	
		if(!b_use_shader_toy) {
			// pass 1
			pick_pass();			// 得到拾取贴图 -- 用来判断哪个物体被拾取
			// pass 2
			if (b_use_vr) { vr_pass(); }
			else { base_pass(); }

			// pass 3
			if (pp_type != PostProcessType::NOPE) { post_process_pass(); }

			// pass 4
			if (b_hdr) hdr_pass();

			// pass 5
			if (b_gamma) gamma_pass();

		}
		else {
			shader_toy_pass();		// render_shader_toy

			if (pp_type != PostProcessType::NOPE) { post_process_pass(); }

			if (b_gamma) gamma_pass();
		}
	}
}

// render pass
void GameManager::scene_pass() {

	draw_init();

	stack_shaders->push(AssetManager_ins().get_shader("scene2d")); {
		if (stack_shaders->top()) {
			stack_shaders->use();
			if (scene_texture) { scene_texture->bind(0); }
			stack_shaders->top()->set_int("u_texture", 0);
		}

		draw_scene(stack_shaders->top());

	} stack_shaders->pop();

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
			glStencilMask(0xff);							// 允许写入模板缓冲
			draw_all_objs(stack_shaders->top());
		} if (b_explode) { stack_shaders->pop(); }

		// draw border
		stack_shaders->push(AssetManager_ins().get_shader("solid_color")); {
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

		// 法线可视化
		if (b_normal_visual) { normal_visual_pass(); }

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
						stack_shaders->top()->set_int("u_msaa_num", 4);
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
void GameManager::normal_visual_pass() {
	stack_shaders->push(AssetManager_ins().get_shader("normal_visual")); {
		if (stack_shaders->top()) {
			stack_shaders->top()->use();
			stack_shaders->top()->set_vec4("u_solid_color", CVector4D(0.0f, 1.0f, 1.0f, 1.0f));
		}
		draw_all_objs(stack_shaders->top());
	} stack_shaders->pop();

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
void GameManager::post_process_pass() {
	auto t_pp_rt = pp_rt;
	if (b_hdr) { t_pp_rt = hdr_pp_rt; }

	t_pp_rt->use(); {
		draw_init();

		stack_shaders->push(AssetManager_ins().get_shader("pp")); {
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				if (scene_texture) { scene_texture->bind(0); }
				stack_shaders->top()->set_int("u_texture", 0);
				stack_shaders->top()->set_int("u_pp_type", static_cast<int>(pp_type));
			}

			draw_scene(stack_shaders->top());

		} stack_shaders->pop();

	} t_pp_rt->un_use();

	// update scene texture
	if (t_pp_rt->get_attach_textures().size() > 0) {
		scene_texture = t_pp_rt->get_attach_textures()[0].texture;
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
void GameManager::vr_pass() {
	auto t_vr_rt = vr_rt;
	auto t_msaa_vr_rt = msaa_vr_rt;
	auto t_vr_rt_mix = vr_rt_mix;
	if (b_hdr) { t_vr_rt = hdr_vr_rt; t_msaa_vr_rt = hdr_msaa_vr_rt; t_vr_rt_mix = hdr_vr_rt_mix; }

	// get left and right pic pass
	if (b_msaa) { t_msaa_vr_rt->use(); } else { t_vr_rt->use(); } {

		// left eyes scene
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			draw_init();

			auto t_old_location = main_camera->get_root_component()->get_location();
			auto t_new_location = t_old_location + main_camera->get_camera_component()->get_right_axis() * vr_delta;
			main_camera->get_root_component()->set_location(t_new_location);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), main_camera->get_camera_component()->get_view_mat().data());

			// draw all objects
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			}
			glStencilMask(0xff);							// 允许写入模板缓冲
			draw_all_objs(stack_shaders->top());

			// draw border
			stack_shaders->push(AssetManager_ins().get_shader("solid_color")); {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
				}
				glStencilFunc(GL_NOTEQUAL, 1, 0xff);			// 之前置 1 了的 不通过
				glStencilMask(0x00);							// 不允许写入模板缓冲
																	//glDisable(GL_DEPTH_TEST);
				draw_border(stack_shaders->top());
				//glEnable(GL_DEPTH_TEST);
				glStencilMask(0xff);							// 重新允许写入模板缓冲

			} stack_shaders->pop();

			// normal visual
			if (b_normal_visual) { normal_visual_pass(); }

			main_camera->get_root_component()->set_location(t_old_location);
		}

		// right eyes scene
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
			draw_init();

			auto t_old_location = main_camera->get_root_component()->get_location();
			auto t_new_location = t_old_location + main_camera->get_camera_component()->get_right_axis() * -vr_delta;
			main_camera->get_root_component()->set_location(t_new_location);

			ub_matrices->fill_data(0, CMatrix4x4::data_size(), main_camera->get_camera_component()->get_view_mat().data());

			// draw all objects
			if (stack_shaders->top()) {
				stack_shaders->top()->use();
				stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			}
			glStencilMask(0xff);							// 允许写入模板缓冲
			draw_all_objs(stack_shaders->top());

			// draw border
			stack_shaders->push(AssetManager_ins().get_shader("solid_color")); {
				if (stack_shaders->top()) {
					stack_shaders->top()->use();
					stack_shaders->top()->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
				}
				glStencilFunc(GL_NOTEQUAL, 1, 0xff);			// 之前置 1 了的 不通过
				glStencilMask(0x00);							// 不允许写入模板缓冲
																	//glDisable(GL_DEPTH_TEST);
				draw_border(stack_shaders->top());
				//glEnable(GL_DEPTH_TEST);
				glStencilMask(0xff);							// 重新允许写入模板缓冲

			} stack_shaders->pop();

			// normal visual
			if (b_normal_visual) { normal_visual_pass(); }

			main_camera->get_root_component()->set_location(t_old_location);

		}

	} if (b_msaa) { t_msaa_vr_rt->un_use(); } else { t_vr_rt->un_use(); }

	if(b_msaa) {
		if (!b_msaa_custom) {
			blit(t_msaa_vr_rt, t_vr_rt, viewport_info.width, viewport_info.heigh);
			blit(t_msaa_vr_rt, t_vr_rt, viewport_info.width, viewport_info.heigh, 1, 1);
		} // opengl 自带抗锯齿
		else {
			t_vr_rt->use(); {
				// left eye
				{
					glDrawBuffer(GL_COLOR_ATTACHMENT0);
					draw_init();
					stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
						if (stack_shaders->top() && t_msaa_vr_rt->get_attach_textures().size() > 0) {
							auto t_texture = t_msaa_vr_rt->get_attach_textures()[0].texture;
							if (t_texture) { t_texture->bind(0); }
							stack_shaders->top()->use();
							stack_shaders->top()->set_int("u_texture_msaa", 0);
							stack_shaders->top()->set_int("u_msaa_num", 4);
						}
						draw_scene(stack_shaders->top());
					} stack_shaders->pop();
				}
				// right eye
				{
					glDrawBuffer(GL_COLOR_ATTACHMENT1);
					draw_init();
					stack_shaders->push(AssetManager_ins().get_shader("msaa")); {
						if (stack_shaders->top() && t_msaa_vr_rt->get_attach_textures().size() > 1) {
							auto t_texture = t_msaa_vr_rt->get_attach_textures()[1].texture;
							if (t_texture) { t_texture->bind(0); }
							stack_shaders->top()->use();
							stack_shaders->top()->set_int("u_texture_msaa", 0);
							stack_shaders->top()->set_int("u_msaa_num", 4);
						}
						draw_scene(stack_shaders->top());
					} stack_shaders->pop();
				}
			} t_vr_rt->un_use();

		} // 自定义抗锯齿
	}

	// mix pass -- mix left and right pic
	t_vr_rt_mix->use(); {
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

	} t_vr_rt_mix->un_use();

	if (t_vr_rt_mix->get_attach_textures().size() > 0) {
		scene_texture = t_vr_rt_mix->get_attach_textures()[0].texture;
	}
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
	set_polygon_mode(front_polygon_mode, back_polygon_mode);
	for (auto go : game_objects) {
		if (go.second->get_id() != cur_pick_object_id) { 
			glStencilFunc(GL_ALWAYS, 0, 0xff); 
			go.second->_draw(shader);
		} // 总是通过测试 -- 且通过后置0
	} // 先画没被选中的 -- 保证最后写入的 1 不被 0 覆盖
	for (auto go : game_objects) {
		if (go.second->get_id() == cur_pick_object_id) { 
			glStencilFunc(GL_ALWAYS, 1, 0xff); 
			go.second->_draw(shader);
		} // 总是通过测试 -- 且通过后置1
	}
	set_polygon_mode();
}
void GameManager::draw_border(SPTR_Shader shader) {
	set_polygon_mode(front_polygon_mode, back_polygon_mode);
	for (auto go : game_objects) {
		if (go.second->get_id() == cur_pick_object_id) {
			go.second->get_root_component()->set_all_border(true);
			go.second->_draw(shader);
			go.second->get_root_component()->set_all_border(false);
		}	// 拾取 -- 绘制边框
	}
	set_polygon_mode();
}
void GameManager::draw_init() {
	glClearColor(background_color.r_f(), background_color.g_f(), background_color.b_f(), background_color.a_f());
	glClear(GL_COLOR_BUFFER_BIT);
	if (glIsEnabled(GL_DEPTH_TEST)) { glClear(GL_DEPTH_BUFFER_BIT); }
	if (glIsEnabled(GL_STENCIL_TEST)) { glClear(GL_STENCIL_BUFFER_BIT); }
}

void GameManager::init_rt() {
	if (scene_rt) scene_rt.reset();
	scene_rt = CREATE_CLASS(RenderTarget);
	if (scene_rt) {
		if (!scene_rt->init_normal(viewport_info.width, viewport_info.heigh)) {
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
}
void GameManager::init_pick_rt() {
	if(pick_rt) pick_rt.reset();
	pick_rt = CREATE_CLASS(RenderTarget);
	if (pick_rt) {
		pick_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}
}
void GameManager::init_vr_rt() {
	if (vr_rt) vr_rt.reset();
	vr_rt = CREATE_CLASS(RenderTarget);
	if (vr_rt) {
		vr_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}

	if (msaa_vr_rt) msaa_vr_rt.reset();
	msaa_vr_rt = CREATE_CLASS(RenderTarget);
	if (msaa_vr_rt) {
		msaa_vr_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh, true)
			->init();
	}

	if (vr_rt_mix) vr_rt_mix.reset();
	vr_rt_mix = CREATE_CLASS(RenderTarget);
	if (vr_rt_mix) {
		vr_rt_mix->init_normal(viewport_info.width, viewport_info.heigh);
	}

	// init rt for hdr
	if (hdr_vr_rt) hdr_vr_rt.reset();
	hdr_vr_rt = CREATE_CLASS(RenderTarget);
	if (hdr_vr_rt) {
		hdr_vr_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGBA16F)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D, GL_RGBA16F)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}

	if (hdr_msaa_vr_rt) hdr_msaa_vr_rt.reset();
	hdr_msaa_vr_rt = CREATE_CLASS(RenderTarget);
	if (hdr_msaa_vr_rt) {
		hdr_msaa_vr_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh, true)
			->init();
	}

	if (hdr_vr_rt_mix) hdr_vr_rt_mix.reset();
	hdr_vr_rt_mix = CREATE_CLASS(RenderTarget);
	if (hdr_vr_rt_mix) {
		hdr_vr_rt_mix->init_normal_f(viewport_info.width, viewport_info.heigh);
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
	free_camera->get_root_component()->set_location(0.0f, 1.5f, -10.0f);
	free_camera->get_root_component()->set_rotation(0.0f, 0.0f, 0.0f);

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







