#define GLEW_STATIC
#include <GL/glew.h>

#include "gameobject.h"
#include "freecamera.h"
#include "gamemanager.h"
#include "assetmanager.h"
#include "inputmanager.h"
#include <chrono>					// get time
#include <assert.h>

#include <GLFW/glfw3.h>

const auto t_time_begin_s = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

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
			c_debug() << "【error】【glew】init fail";
			c_debug() << (char*)glewGetErrorString(flag);
		}
		else { }
	}
		
	b_mouse_clicked = false;

	// load asset
	{
		// shader
		AssetManager::load_shader("default", "resources/shaders/mvp.vert", "resources/shaders/blinn_phong.frag");
		AssetManager::load_shader("depth", "resources/shaders/mvp.vert", "resources/shaders/depth.frag");
		AssetManager::load_shader("solid_color", "resources/shaders/mvp.vert", "resources/shaders/solid_color.frag");
		AssetManager::load_shader("single_texture", "resources/shaders/mvp.vert", "resources/shaders/single_texture.frag");
		AssetManager::load_shader("scene2d", "resources/shaders/scene2d.vert", "resources/shaders/single_texture.frag");
		AssetManager::load_shader("pp", "resources/shaders/scene2d.vert", "resources/shaders/post_process.frag");
		AssetManager::load_shader("pick", "resources/shaders/mvp.vert", "resources/shaders/pick.frag");
		AssetManager::load_shader("vr_mix", "resources/shaders/scene2d.vert", "resources/shaders/vr_mix.frag");

		AssetManager::load_shader("shader_toy_img", "resources/shaders/scene2d.vert", "resources/shaders/shadertoy_img.frag");
		AssetManager::load_shader("shader_toy_buffer_a", "resources/shaders/scene2d.vert", "resources/shaders/shadertoy_buffer_a.frag");
		AssetManager::load_shader("shader_toy_buffer_b", "resources/shaders/scene2d.vert", "resources/shaders/shadertoy_buffer_b.frag");
		AssetManager::load_shader("shader_toy_buffer_c", "resources/shaders/scene2d.vert", "resources/shaders/shadertoy_buffer_c.frag");
		AssetManager::load_shader("shader_toy_buffer_d", "resources/shaders/scene2d.vert", "resources/shaders/shadertoy_buffer_d.frag");

		// mesh
		AssetManager::load_mesh("triangle_right", "resources/models/txt/triangle_right.txt");
		AssetManager::load_mesh("triangle_regular", "resources/models/txt/triangle_regular.txt");
		AssetManager::load_mesh("rect", "resources/models/txt/rect.txt");
		AssetManager::load_mesh("circle", "resources/models/txt/circle.txt");

		AssetManager::load_mesh("cube", "resources/models/txt/cube.txt");
		AssetManager::load_mesh("cone", "resources/models/txt/cone.txt");
		AssetManager::load_mesh("cylinder", "resources/models/txt/cylinder.txt");
		AssetManager::load_mesh("sphere", "resources/models/txt/sphere.txt");

		// material
		AssetManager::load_materials("resources/materials/txt/default_material.txt");
		AssetManager::load_materials("resources/materials/txt/single_material.txt");
		AssetManager::load_texture("resources/textures/txt/texture_default.txt");
		AssetManager::load_texture("resources/textures/txt/texture_default.png");

		load_asset();						// 加载资源
		
	}

	main_camera = set_main_camera();	// 绑定主相机
	main_shader = set_main_shader();	// 绑定主shader

	// init input map
	{
		map_input_default();
		bind_input_default();

		map_input();						
		main_bind_input();
	}

	// ui setting init
	{
		background_color = CColor(205, 220, 232);
		border_color = CColor(221, 161, 18);
		front_polygon_mode = GL_FILL;
		back_polygon_mode = GL_LINE;
		b_use_vr = false;
		vr_delta = 0.1f;
		pp_type = PostProcessType::NOPE;
		b_use_shader_toy = false;
	}

	// gl state init
	{
		set_depth_test(true);
		set_stencil_test(true, GL_NOTEQUAL, 1, 0xff, GL_KEEP, GL_REPLACE, GL_REPLACE);
		set_blend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		set_polygon_mode(front_polygon_mode, back_polygon_mode);
		set_cull_face();
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
	// update
	{
		tick();
		main_tick();
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
			//base_pass();
		}
		else {
			shader_toy_pass();		// render_shader_toy
		}
		
		// pass 3
		if (pp_type != PostProcessType::NOPE && !(pp_type == PostProcessType::GRAY && b_use_vr)) {
			post_process_pass();
		}

	}
}

// render pass
void GameManager::scene_pass() {

	draw_init();

	auto s_shader = AssetManager::get_shader("scene2d");
	if (s_shader != nullptr) {
		s_shader->use();
		if (scene_texture != nullptr) { scene_texture->bind(0); }
		s_shader->set_int("u_texture", 0);
	}

	draw_scene(s_shader->get_name());

}
void GameManager::pick_pass() {
	pick_rt->use();
	uint t_fpm = front_polygon_mode;
	front_polygon_mode = GL_FILL;
	draw_init();

	auto p_shader = AssetManager::get_shader("pick");
	if (p_shader != nullptr) {
		p_shader->use();
		p_shader->set_mat4("u_view", main_camera->get_camera_component()->get_view_mat());
		p_shader->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
		p_shader->set_uint("u_object_id", 0);
		p_shader->set_uint("u_component_id", 0);
	}

	glStencilMask(0x00);
	draw_all_objs(p_shader->get_name());
	glStencilMask(0xff);

	if (b_mouse_clicked) {
		b_mouse_clicked = false;
		uint data[3] = { 0 };
		glReadPixels(mouse_clicked_x, mouse_clicked_y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &data);
		cur_pick_object_id = data[0];
		cur_pick_component_id = data[1];
	}
	front_polygon_mode = t_fpm;
	pick_rt->un_use();

}
void GameManager::base_pass() {

	if (main_shader != nullptr) {
		main_shader->use();
		main_shader->set_mat4("u_view", main_camera->get_camera_component()->get_view_mat());
		main_shader->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
	}
	
	auto t_shader = AssetManager::get_shader("solid_color");
	if (t_shader != nullptr) {
		t_shader->use();
		t_shader->set_mat4("u_view", main_camera->get_camera_component()->get_view_mat());
		t_shader->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
		t_shader->set_vec4("u_solid_color", border_color.rgba_f());
	}

	scene_rt->use();
	draw_init();

	// draw all objects
	{
		glStencilMask(0xff);							// 允许写入模板缓冲
		draw_all_objs(main_shader->get_name());
	}

	// draw border
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);			// 之前置 1 了的 不通过
		glStencilMask(0x00);							// 不允许写入模板缓冲
		//glDisable(GL_DEPTH_TEST);
		draw_border(t_shader->get_name());
		//glEnable(GL_DEPTH_TEST);
		glStencilMask(0xff);							// 重新允许写入模板缓冲
	}
	scene_rt->un_use();
	
	// update scene texture
	if (scene_rt->get_attach_textures().size() > 0) {
		scene_texture = scene_rt->get_attach_textures()[0].texture;
	}
}
void GameManager::post_process_pass() {
	pp_rt->use();
	draw_init();

	auto p_shader = AssetManager::get_shader("pp");
	if (p_shader != nullptr) {
		p_shader->use();
		if (scene_texture != nullptr) { scene_texture->bind(0); }
		p_shader->set_int("u_texture", 0);
		p_shader->set_uint("u_pp_type", pp_type);
	}

	draw_scene(p_shader->get_name());

	pp_rt->un_use();

	// update scene texture
	if (pp_rt->get_attach_textures().size() > 0) {
		scene_texture = pp_rt->get_attach_textures()[0].texture;
	}
}
void GameManager::vr_pass() {

	// get left and right pic pass
	{
		vr_rt->use();

		// left eyes scene
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			draw_init();

			auto t_old_location = main_camera->get_root_component()->get_location();
			auto t_new_location = t_old_location + main_camera->get_camera_component()->get_right_axis() * vr_delta;
			main_camera->get_root_component()->set_location(t_new_location);
			if (main_shader != nullptr) {
				main_shader->use();
				main_shader->set_mat4("u_view", main_camera->get_camera_component()->get_view_mat());
				main_shader->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			}
			auto t_shader = AssetManager::get_shader("solid_color");
			if (t_shader != nullptr) {
				t_shader->use();
				t_shader->set_mat4("u_view", main_camera->get_camera_component()->get_view_mat());
				t_shader->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			}

			// draw all objects
			glStencilMask(0xff);							// 允许写入模板缓冲
			draw_all_objs(main_shader->get_name());

			// draw border
			glStencilFunc(GL_NOTEQUAL, 1, 0xff);			// 之前置 1 了的 不通过
			glStencilMask(0x00);							// 不允许写入模板缓冲
																//glDisable(GL_DEPTH_TEST);
			draw_border(t_shader->get_name());
			//glEnable(GL_DEPTH_TEST);
			glStencilMask(0xff);							// 重新允许写入模板缓冲

			main_camera->get_root_component()->set_location(t_old_location);
		}

		// right eyes scene
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
			draw_init();

			auto t_old_location = main_camera->get_root_component()->get_location();
			auto t_new_location = t_old_location + main_camera->get_camera_component()->get_right_axis() * -vr_delta;
			main_camera->get_root_component()->set_location(t_new_location);

			if (main_shader != nullptr) {
				main_shader->use();
				main_shader->set_mat4("u_view", main_camera->get_camera_component()->get_view_mat());
				main_shader->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			}
			auto t_shader = AssetManager::get_shader("solid_color");
			if (t_shader != nullptr) {
				t_shader->use();
				t_shader->set_mat4("u_view", main_camera->get_camera_component()->get_view_mat());
				t_shader->set_vec3("u_view_pos", main_camera->get_root_component()->get_location());
			}

			// draw all objects
			glStencilMask(0xff);							// 允许写入模板缓冲
			draw_all_objs(main_shader->get_name());

			// draw border
			glStencilFunc(GL_NOTEQUAL, 1, 0xff);			// 之前置 1 了的 不通过
			glStencilMask(0x00);							// 不允许写入模板缓冲
																//glDisable(GL_DEPTH_TEST);
			draw_border(t_shader->get_name());
			//glEnable(GL_DEPTH_TEST);
			glStencilMask(0xff);							// 重新允许写入模板缓冲

			main_camera->get_root_component()->set_location(t_old_location);

		}

		vr_rt->un_use();
	}

	// mix pass -- mix left and right pic
	{
		vr_rt_mix->use();
		draw_init();

		auto m_shader = AssetManager::get_shader("vr_mix");
		if (m_shader != nullptr && vr_rt->get_attach_textures().size() >= 2) {
			auto t_left = vr_rt->get_attach_textures()[0].texture;
			auto t_right = vr_rt->get_attach_textures()[1].texture;
			if (t_left != nullptr) { t_left->bind(0); }
			if (t_right != nullptr) { t_right->bind(1); }
			m_shader->use();
			m_shader->set_int("u_texture_left", 0);
			m_shader->set_int("u_texture_right", 1);
		}
		draw_scene(m_shader->get_name());

		vr_rt_mix->un_use();
	}

	if (vr_rt_mix->get_attach_textures().size() > 0) {
		scene_texture = vr_rt_mix->get_attach_textures()[0].texture;
	}
}
void GameManager::shader_toy_pass() {

	// get time
	auto t_time_cur = std::chrono::system_clock::now();
	auto t_time_cur_s = std::chrono::duration_cast<std::chrono::milliseconds>(t_time_cur.time_since_epoch());
	float t_time = (t_time_cur_s - t_time_begin_s).count() / 1000.0f;

	for (uint i = 0; i < 4; ++i) {
		auto stbr = shader_toy_buffer_rts[i];
		stbr->use();
		draw_init();
		std::string t_name = "shader_toy_buffer_" + char_to_string(i + 'a');
		auto t_shader = AssetManager::get_shader(t_name);
		if (t_shader != nullptr) {
			t_shader->set_vec3("iResolution", CVector3D(viewport_info.width, viewport_info.heigh, 0.0f));

		}
		draw_scene(t_shader->get_name());

		stbr->un_use();
	}

	shader_toy_rt->use();
	draw_init();
	auto t_shader = AssetManager::get_shader("shader_toy_img");
	if (t_shader != nullptr) {
		t_shader->use();
		t_shader->set_vec3("iResolution", CVector3D(viewport_info.width, viewport_info.heigh, 0.0f));
		t_shader->set_float("iTime", t_time);
	}
	draw_scene(t_shader->get_name());
	shader_toy_rt->un_use();

	if (shader_toy_rt->get_attach_textures().size() > 0) {
		scene_texture = shader_toy_rt->get_attach_textures()[0].texture;
	}

}

void GameManager::draw_scene(const std::string& shader) {
	glDisable(GL_DEPTH_TEST);
	auto s_mesh = AssetManager::get_mesh("rect");
	if (s_mesh != nullptr) {
		s_mesh->set_use_default_mt(false);
		s_mesh->draw(shader);
		s_mesh->set_use_default_mt(true);
	}
	glEnable(GL_DEPTH_TEST);
}
void GameManager::draw_all_objs(const std::string& shader) {
	set_depth_test();
	set_polygon_mode(front_polygon_mode, back_polygon_mode);
	for (auto go : game_objects) {
		if (go.second->get_id() != cur_pick_object_id) { 
			glStencilFunc(GL_ALWAYS, 0, 0xff); 
			go.second->draw(shader);
		} // 总是通过测试 -- 且通过后置0
	} // 先画没被选中的 -- 保证最后写入的 1 不被 0 覆盖
	for (auto go : game_objects) {
		if (go.second->get_id() == cur_pick_object_id) { 
			glStencilFunc(GL_ALWAYS, 1, 0xff); 
			go.second->draw(shader);
		} // 总是通过测试 -- 且通过后置1
	}
	set_polygon_mode();
}
void GameManager::draw_border(const std::string& shader) {
	set_polygon_mode(front_polygon_mode, back_polygon_mode);
	for (auto go : game_objects) {
		if (go.second->get_id() == cur_pick_object_id) {
			go.second->get_root_component()->set_all_border(true);
			go.second->draw(shader);
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
	if (scene_rt != nullptr) scene_rt.reset();
	scene_rt = CREATE_CLASS(RenderTarget);
	if (scene_rt != nullptr) {
		if (!scene_rt->init_normal(viewport_info.width, viewport_info.heigh)) {
			c_debug() << "create rt fail";
		}
	}

	if (pp_rt != nullptr) pp_rt.reset();
	pp_rt = CREATE_CLASS(RenderTarget);
	if (pp_rt != nullptr) {
		if (!pp_rt->init_normal(viewport_info.width, viewport_info.heigh)) {
			c_debug() << "create rt fail";
		}
	}

	init_pick_rt();
	init_vr_rt();
	init_shader_toy_rt();
}
void GameManager::init_pick_rt() {
	if(pick_rt != nullptr) pick_rt.reset();
	pick_rt = CREATE_CLASS(RenderTarget);
	if (pick_rt != nullptr) {
		pick_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh, GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}
}
void GameManager::init_vr_rt() {
	if (vr_rt != nullptr) vr_rt.reset();
	vr_rt = CREATE_CLASS(RenderTarget);
	if (vr_rt != nullptr) {
		vr_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, viewport_info.width, viewport_info.heigh)
			->add_attach_texture(GL_COLOR_ATTACHMENT1, viewport_info.width, viewport_info.heigh)
			->add_attach_renderbuffer(viewport_info.width, viewport_info.heigh)
			->init();
	}

	if (vr_rt_mix != nullptr) vr_rt_mix.reset();
	vr_rt_mix = CREATE_CLASS(RenderTarget);
	if (vr_rt_mix != nullptr) {
		vr_rt_mix->init_normal(viewport_info.width, viewport_info.heigh);
	}
}
void GameManager::init_shader_toy_rt() {
	if (shader_toy_rt != nullptr) shader_toy_rt.reset();
	shader_toy_rt = CREATE_CLASS(RenderTarget);
	if (shader_toy_rt != nullptr) { shader_toy_rt->init_simple(viewport_info.width, viewport_info.heigh); }

	for (auto& stbr : shader_toy_buffer_rts) {
		if (stbr != nullptr) stbr.reset();
		stbr = CREATE_CLASS(RenderTarget);
		if (stbr != nullptr) { stbr->init_simple(viewport_info.width, viewport_info.heigh); }
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
	glViewport(0, 0, w, h);

	CMatrix4x4 projection;
	float t_ratio = (float)w / (h == 0 ? 1 : h);
	projection.perspective(45.0f, t_ratio, 0.1f, 100.0f);
	//projection.frustum(-0.1f, 0.1f, -0.1f / t, 0.1f / t, 0.1f, 100.0f);
	//projection.ortho(-10.0f, 10.0f, -10.0f / t, 10.0f / t, 0.01f, 1000.0f);
	//projection.ortho(20.0f, t, 0.01f, 100.0f);
	//projection.ortho_2d(0.0f, 20.0f, 0.0f / t, 20.0f /t);

	if (main_shader != nullptr) {
		main_shader->use();
		main_shader->set_mat4("u_projection", projection);
	}
	auto t_s = AssetManager::get_shader("solid_color");
	if (t_s != nullptr) {
		t_s->use();
		t_s->set_mat4("u_projection", projection);
	}
	auto p_s = AssetManager::get_shader("pick");
	if (p_s != nullptr) {
		p_s->use();
		p_s->set_mat4("u_projection", projection);
	}

	init_rt();
	
}
void GameManager::exit() {
	InputManager::exit();
}

SPTR_CameraObject GameManager::set_main_camera() {
	auto free_camera = CREATE_CLASS(FreeCamera);
	add_game_object("free_camera", free_camera);
	free_camera->get_root_component()->set_location(0.0f, 1.5f, -10.0f);
	free_camera->get_root_component()->set_rotation(0.0f, 0.0f, 0.0f);

	return free_camera;
}
SPTR_Shader	GameManager::set_main_shader() { return AssetManager::get_shader("default"); }

void GameManager::set_depth_test(bool enable, uint depth_func, uint depth_mask) {
	if (enable) { glEnable(GL_DEPTH_TEST); }
	else { glDisable(GL_DEPTH_TEST); }

	glDepthMask(depth_mask);
	glDepthFunc(depth_func);
}
void GameManager::set_stencil_test(bool enable, uint func, uint ref, uint mask, uint fail, uint zfail, uint zpass) {
	if (enable) { glEnable(GL_STENCIL_TEST); }
	else { glDisable(GL_STENCIL_TEST); }
	
	glStencilFunc(func, ref, mask);
	glStencilOp(fail, zfail, zpass);
	
}
void GameManager::set_blend(bool enable, uint sfactor, uint dfactor) {
	if (enable) { glEnable(GL_BLEND); }
	else { glDisable(GL_BLEND); }

	glBlendFunc(sfactor, dfactor);
}
void GameManager::set_polygon_mode(uint front_mode, uint back_mode) {
	//glPolygonMode(GL_FRONT, front_mode);
	//glPolygonMode(GL_BACK, back_mode);
	glPolygonMode(GL_FRONT_AND_BACK, front_mode);
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
		(*it).second->begin_play();
	}
}
void GameManager::main_tick() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it).second->tick();
	}
}
void GameManager::main_draw(const std::string& shader) {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it).second->draw(shader);
	}
}

void GameManager::map_input_default() {
	{
		InputState is;
		is.mouse_released = Qt::LeftButton;
		InputManager::map_action("gm_m_r", is);
	}
	
}
void GameManager::bind_input_default() {
	IM_BIND_ACTION(gm_m_r, GameManager, this, &GameManager::mouse_released);
}

void GameManager::mouse_released() {
	// 造成一次单击事件
	// read pix
	float delta = (InputManager::get_input_data().mouse_pressed_pos - InputManager::get_input_data().mouse_cur_pos).manhattanLength();
	if (delta < CMath::eps) {
		b_mouse_clicked = true;
		mouse_clicked_x = InputManager::get_input_data().mouse_cur_pos.x();
		mouse_clicked_y = viewport_info.heigh - InputManager::get_input_data().mouse_cur_pos.y() - 1;
	}
}

// ==========================================================================================

std::string GameManager::uint_to_string(uint ui) { return std::to_string(ui); }

std::string GameManager::char_to_string(char c) {
	std::string res("");
	res.push_back(c);
	return res;
}





