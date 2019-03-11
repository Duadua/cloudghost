#pragma once

#include "shader.h"
#include "gameobject.h"
#include "rendertarget.h"
#include "cameraobject.h"
#include <QMap>
#include <memory>
#include <QOpenGLFunctions_3_3_core>

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QOpenGLWidget;

enum PostProcessType {
	NOPE,					// 原色
	GRAY,					// 灰度
	INVERS,					// 反相
	BLUR,					// 模糊
	SHARPEN,				// 锐化
	EDGE_DET,				// 边缘检测
	
};

class GameManagerImpl {
public:
	virtual ~GameManagerImpl() {}

	virtual void load_asset() = 0;							// 加载资源
	virtual void begin_play(QOpenGLWidget* gl) = 0;			// 设置模型等
	virtual void tick() = 0;								// 每帧变化的参数
	virtual void map_input() = 0;							// 绑定输入

	virtual SPTR_CameraObject	set_main_camera() = 0;		// 设置主相机 
	virtual SPTR_Shader			set_main_shader() = 0;		// 设置主Shader
};

class GameManager : public GameManagerImpl {

public:
	void init(QOpenGLWidget* gl);
	void draw(QOpenGLWidget* gl);

	void pre_init(QOpenGLWidget* gl);
	void resize(QOpenGLWidget* gl, int w, int h);
	void exit(QOpenGLWidget* gl);

public:
	virtual void load_asset() override {}
	virtual void begin_play(QOpenGLWidget* gl) override {}
	virtual void tick() override {}
	virtual void map_input() override {}

	virtual SPTR_CameraObject	set_main_camera() override;
	virtual SPTR_Shader			set_main_shader() override;

public:
	GameManager();
	virtual ~GameManager();
	static GameManager* get_instance();

	void add_game_object(const QString& key, SPTR_GameObject value);

	// gl state
	void set_depth_test(bool enable = true, uint depth_func = GL_LESS, uint depth_mask = GL_TRUE);
	void set_stencil_test(bool enable = false, uint func = GL_EQUAL, uint ref = 1, uint mask = 0xff, uint fail = GL_KEEP, uint zfail = GL_KEEP, uint zpass = GL_KEEP);
	void set_blend(bool enable, uint sfactor = GL_SRC_ALPHA, uint dfactor = GL_ONE_MINUS_SRC_ALPHA);
	void set_polygon_mode(uint front_mode = GL_FILL, uint back_mode = GL_LINE);
	void set_cull_face(bool enable = false, uint mode = GL_BACK, uint front_face = GL_CCW);

public:									// used for qt ui
	GET_SET(CColor, background_color);
	GET_SET(CColor, border_color);
	GET_SET(bool, b_use_vr);
	GET_SET(float, vr_delta);
	GET_SET(PostProcessType, pp_type);
	GET_SET(bool, b_use_shader_toy);

protected:
	static GameManager* instance;
	
	SPTR_CameraObject main_camera;
	SPTR_Shader main_shader;

	// ui shading setting
	CColor background_color;
	CColor border_color;			// 物体边框颜色

	bool b_use_vr;
	float vr_delta;

	PostProcessType pp_type;		// 当前后处理效果

	bool b_use_shader_toy;

private:
	void main_bind_input();
	void main_begin_play();
	void main_tick();
	void main_draw(const std::string& shader);

	// default init
	void map_input_default();
	void bind_input_default();
	
	// render pipe
	SPTR_RenderTarget scene_rt;
	SPTR_RenderTarget pp_rt;
	SPTR_RenderTarget pick_rt;
	SPTR_RenderTarget vr_rt;
	SPTR_RenderTarget vr_rt_mix;
	SPTR_RenderTarget shader_toy_rt;
	SPTR_RenderTarget shader_toy_buffer_rts[4];
	SPTR_Texture2D scene_texture;
	void scene_pass();
	void pick_pass();					// 拾取阶段
	void base_pass();
	void post_process_pass();
	void vr_pass();
	void shader_toy_pass();

	void init_rt();
	void init_pick_rt();
	void init_vr_rt();
	void init_shader_toy_rt();

	void draw_scene(const std::string& shader);
	void draw_all_objs(const std::string& shader);
	void draw_border(const std::string& shader);

	void draw_init();

private:
	QOpenGLFunctions_3_3_Core * core;
	QOpenGLWidget* gl;

	QMap<QString, SPTR_GameObject> game_objects;

public:
	void exec_mouse_wheeeel_event(QWheelEvent* event, QOpenGLWidget* gl);
	void exec_mouse_moveeee_event(QMouseEvent* event, QOpenGLWidget* gl);
	void exec_mouse_pressed_event(QMouseEvent* event, QOpenGLWidget* gl);
	void exec_mouse_release_event(QMouseEvent* event, QOpenGLWidget* gl);
	void mouse_pressed_over();

	void exec_key_pressed_event(QKeyEvent* event, QOpenGLWidget* gl);
	void exec_key_release_event(QKeyEvent* event, QOpenGLWidget* gl);
	void key_pressed_over();

private:
	// 鼠标事件 --拾取用
	bool b_mouse_clicked;
	uint mouse_clicked_x;
	uint mouse_clicked_y;
	uint cur_pick_object_id;
	uint cur_pick_component_id;
	void mouse_released();

// time
private:
		

};
