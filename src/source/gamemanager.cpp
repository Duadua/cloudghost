#include "gameobject.h"
#include "freecamera.h"
#include "gamemanager.h"
#include "assetmanager.h"
#include "inputmanager.h"
#include <assert.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>

void GameManager::init(QOpenGLWidget* gl) {

	// init gl_core
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	this->gl = gl;
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

	// gl 状态初始化
	{
		background_color = CColor(205, 220, 232);
		set_depth_test(true);
		set_stencil_test(true, GL_NOTEQUAL, 1, 0xff, GL_KEEP, GL_REPLACE, GL_REPLACE);
		set_blend(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		set_polygon_mode();
		set_cull_face();
	}
	
	// init rt
	{
		init_rt();
	}
	
	// begin play
	{
		begin_play(gl);						// 设置模型等
		main_begin_play();		
	}

}
void GameManager::draw(QOpenGLWidget* gl) {

	// exec input
	{
		InputManager::exec_action();
		InputManager::exec_axis_key_pressing();
	}

	// update
	{
		tick();
		main_tick();
	}
	
	// draw
	{
		// pass 0
		scene_pass();			// 渲染到默认缓冲 -- 必须第一个执行

		// pass 1
		pick_pass();			// 得到拾取贴图 -- 用来判断哪个物体被拾取

		// pass 2
		base_pass();

		// pass 3
		post_process_pass();

	}
}

// render pass
void GameManager::scene_pass() {

	core->glClearColor(background_color.r_f(), background_color.g_f(), background_color.b_f(), background_color.a_f());
	core->glClear(GL_COLOR_BUFFER_BIT);
	if (core->glIsEnabled(GL_DEPTH_TEST)) { core->glClear(GL_DEPTH_BUFFER_BIT); }
	if (core->glIsEnabled(GL_STENCIL_TEST)) { core->glClear(GL_STENCIL_BUFFER_BIT); }

	core->glDisable(GL_DEPTH_TEST);
	auto s_shader = AssetManager::get_shader("scene2d");
	if (s_shader != nullptr) {
		auto s_mesh = AssetManager::get_mesh("rect");
		if (s_mesh != nullptr) {
			if (scene_texture != nullptr) { scene_texture->bind(0); }
			s_shader->set_int("u_texture", 0);
			s_shader->use();
			s_mesh->set_use_default_mt(false);
			s_mesh->draw(s_shader->get_name());
			s_mesh->set_use_default_mt(true);
		}
	}
	core->glEnable(GL_DEPTH_TEST);
}
void GameManager::pick_pass() {
	pick_rt->use();
	core->glClearColor(background_color.r_f(), background_color.g_f(), background_color.b_f(), background_color.a_f());
	core->glClear(GL_COLOR_BUFFER_BIT);
	if (core->glIsEnabled(GL_DEPTH_TEST)) { core->glClear(GL_DEPTH_BUFFER_BIT); }
	if (core->glIsEnabled(GL_STENCIL_TEST)) { core->glClear(GL_STENCIL_BUFFER_BIT); }

	auto p_shader = AssetManager::get_shader("pick");
	if (p_shader != nullptr) {
		p_shader->use();
		p_shader->set_mat4("u_view", main_camera->get_view_mat());
		p_shader->set_vec3("u_view_pos", main_camera->get_location());
		p_shader->set_uint("u_object_id", 0);
		p_shader->set_uint("u_component_id", 0);
	}
	main_draw(p_shader->get_name());

	if (b_mouse_clicked) {
		b_mouse_clicked = false;
		uint data[3] = { 0 };
		core->glReadPixels(mouse_clicked_x, mouse_clicked_y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &data);
		cur_pick_object_id = data[0];
		cur_pick_component_id = data[1];
	}
	pick_rt->un_use();

}
void GameManager::base_pass() {

	if (main_shader != nullptr) {
		main_shader->use();
		main_shader->set_mat4("u_view", main_camera->get_view_mat());
		main_shader->set_vec3("u_view_pos", main_camera->get_location());
	}
	auto t_shader = AssetManager::get_shader("solid_color");
	if (t_shader != nullptr) {
		t_shader->use();
		t_shader->set_vec4("u_solid_color", CColor(221, 161, 18).rgba_f());
		t_shader->set_mat4("u_view", main_camera->get_view_mat());
		t_shader->set_vec3("u_view_pos", main_camera->get_location());
	}

	scene_rt->use();
	core->glClearColor(background_color.r_f(), background_color.g_f(), background_color.b_f(), background_color.a_f());
	core->glClear(GL_COLOR_BUFFER_BIT);
	if (core->glIsEnabled(GL_DEPTH_TEST)) { core->glClear(GL_DEPTH_BUFFER_BIT); }
	if (core->glIsEnabled(GL_STENCIL_TEST)) { core->glClear(GL_STENCIL_BUFFER_BIT); }

	core->glStencilMask(0xff);							// 允许写入模板缓冲
	
	for (auto go : game_objects) {
		if (go->get_id() != cur_pick_object_id) { 
			core->glStencilFunc(GL_ALWAYS, 0, 0xff); 
			go->draw(main_shader->get_name());
		} // 总是通过测试 -- 且通过后置0
	} // 先画没被选中的 -- 保证最后写入的 1 不被 0 覆盖
	for (auto go : game_objects) {
		if (go->get_id() == cur_pick_object_id) { 
			core->glStencilFunc(GL_ALWAYS, 1, 0xff); 
			go->draw(main_shader->get_name());
		} // 总是通过测试 -- 且通过后置1
	}

	core->glStencilFunc(GL_NOTEQUAL, 1, 0xff);			// 之前置 1 了的 不通过
	core->glStencilMask(0x00);							// 不允许写入模板缓冲

	//core->glDisable(GL_DEPTH_TEST);
	for (auto go : game_objects) {
		if (go->get_id() == cur_pick_object_id) {
			go->get_root_component()->set_all_border(true);
			go->draw(t_shader->get_name());
			go->get_root_component()->set_all_border(false);
		}	// 拾取 -- 绘制边框
	}
	//core->glEnable(GL_DEPTH_TEST);

	core->glStencilMask(0xff);							// 重新允许写入模板缓冲
	scene_rt->un_use();
	
	// update scene texture
	if (scene_rt->get_attach_textures().size() > 0) {
		scene_texture = scene_rt->get_attach_textures()[0].texture;
	}
}
void GameManager::post_process_pass() {
	pp_rt->use();
	core->glClearColor(background_color.r_f(), background_color.g_f(), background_color.b_f(), background_color.a_f());
	core->glClear(GL_COLOR_BUFFER_BIT);
	if (core->glIsEnabled(GL_DEPTH_TEST)) { core->glClear(GL_DEPTH_BUFFER_BIT); }
	if (core->glIsEnabled(GL_STENCIL_TEST)) { core->glClear(GL_STENCIL_BUFFER_BIT); }

	core->glDisable(GL_DEPTH_TEST);
	auto s_shader = AssetManager::get_shader("pp");
	if (s_shader != nullptr) {
		auto s_mesh = AssetManager::get_mesh("rect");
		if (s_mesh != nullptr) {
			if (scene_texture != nullptr) { scene_texture->bind(0); }
			s_shader->set_int("u_texture", 0);
			s_shader->use();
			s_mesh->set_use_default_mt(false);
			s_mesh->draw(s_shader->get_name());
			s_mesh->set_use_default_mt(true);
		}
	}
	core->glEnable(GL_DEPTH_TEST);
	pp_rt->un_use();

	// update scene texture
	if (pp_rt->get_attach_textures().size() > 0) {
		scene_texture = pp_rt->get_attach_textures()[0].texture;
	}
}

void GameManager::init_rt() {
	if (scene_rt != nullptr) scene_rt.reset();
	scene_rt = CREATE_CLASS(RenderTarget);
	if (scene_rt != nullptr) {
		if (!scene_rt->init_normal(gl->width(), gl->height())) {
			qDebug() << "create rt fail";
		}
	}

	if (pp_rt != nullptr) pp_rt.reset();
	pp_rt = CREATE_CLASS(RenderTarget);
	if (pp_rt != nullptr) {
		if (!pp_rt->init_normal(gl->width(), gl->height())) {
			qDebug() << "create rt fail";
		}
	}

	init_pick_rt();
}
void GameManager::init_pick_rt() {
	if(pick_rt != nullptr) pick_rt.reset();
	pick_rt = CREATE_CLASS(RenderTarget);
	if (pick_rt != nullptr) {
		pick_rt->add_attach_texture(GL_COLOR_ATTACHMENT0, gl->width(), gl->height(), GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT)
			->add_attach_renderbuffer(gl->width(), gl->height())
			->init();
	}
}

// ===========================================================================

void GameManager::pre_init(QOpenGLWidget* gl) {

	// init gl mouse cursor
	gl->setCursor(Qt::CrossCursor);
	gl->setFocusPolicy(Qt::StrongFocus);

	// init input manager
	InputManager::init(gl);
}
void GameManager::resize(QOpenGLWidget* gl, int w, int h) {
	core->glViewport(0, 0, w, h);

	CMatrix4x4 projection;
	float t_ratio = (float) gl->width() / gl->height();
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
void GameManager::exit(QOpenGLWidget* gl) {
	InputManager::exit();
}

SPTR_CameraComponent GameManager::set_main_camera() {
	auto free_camera = CREATE_CLASS(FreeCamera);
	add_game_object("free_camera", free_camera);
	free_camera->get_root_component()->set_location(0.0f, 1.5f, -10.0f);
	free_camera->get_root_component()->set_rotation(0.0f, 0.0f, 0.0f);

	return free_camera->get_camera_component();
}
SPTR_Shader	GameManager::set_main_shader() { return AssetManager::get_shader("default"); }

void GameManager::set_depth_test(bool enable, uint depth_func, uint depth_mask) {
	if (enable) { core->glEnable(GL_DEPTH_TEST); }
	else { core->glDisable(GL_DEPTH_TEST); }

	core->glDepthMask(depth_mask);
	core->glDepthFunc(depth_func);
}
void GameManager::set_stencil_test(bool enable, uint func, uint ref, uint mask, uint fail, uint zfail, uint zpass) {
	if (enable) { core->glEnable(GL_STENCIL_TEST); }
	else { core->glDisable(GL_STENCIL_TEST); }
	
	core->glStencilFunc(func, ref, mask);
	core->glStencilOp(fail, zfail, zpass);
	
}
void GameManager::set_blend(bool enable, uint sfactor, uint dfactor) {
	if (enable) { core->glEnable(GL_BLEND); }
	else { core->glDisable(GL_BLEND); }

	core->glBlendFunc(sfactor, dfactor);
}
void GameManager::set_polygon_mode(uint front_mode, uint back_mode) {
	core->glPolygonMode(GL_FRONT, front_mode);
	core->glPolygonMode(GL_BACK, back_mode);
}
void GameManager::set_cull_face(bool enable, uint mode, uint front_face) {
	if (enable) { core->glEnable(GL_CULL_FACE); }
	else { core->glDisable(GL_CULL_FACE); }

	core->glCullFace(mode);
	core->glFrontFace(front_face);			// 默认逆时针 -- CCW -- 此状态同时也影响 polygon_mode
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

void GameManager::add_game_object(const QString& key, SPTR_GameObject value) {
	if (game_objects.count(key)) return;
	game_objects.insert(key, value);
}

void GameManager::main_bind_input() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->bind_input();
	}
}
void GameManager::main_begin_play() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->begin_play();
	}
}
void GameManager::main_tick() {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->tick();
	}
}
void GameManager::main_draw(const std::string& shader) {
	for (auto it = game_objects.begin(); it != game_objects.end(); ++it) {
		(*it)->draw(shader);
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
		mouse_clicked_y = gl->height() - InputManager::get_input_data().mouse_cur_pos.y() - 1;
	}
}

void GameManager::exec_mouse_wheeeel_event(QWheelEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_wheeeel_event(event, gl); }
void GameManager::exec_mouse_moveeee_event(QMouseEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_moveeee_event(event, gl); }
void GameManager::exec_mouse_pressed_event(QMouseEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_pressed_event(event, gl); }
void GameManager::exec_mouse_release_event(QMouseEvent* event, QOpenGLWidget* gl) { InputManager::exec_mouse_release_event(event, gl); }
void GameManager::mouse_pressed_over() { InputManager::mouse_pressed_over(); }

void GameManager::exec_key_pressed_event(QKeyEvent* event, QOpenGLWidget* gl)	{ InputManager::exec_key_pressed_event(event, gl); }
void GameManager::exec_key_release_event(QKeyEvent* event, QOpenGLWidget* gl)	{ InputManager::exec_key_release_event(event, gl); }
void GameManager::key_pressed_over()											{ InputManager::key_pressed_over(); }

