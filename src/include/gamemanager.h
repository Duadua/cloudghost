#pragma once

#include "shader.h"
#include "gameobject.h"
#include "cameracomponent.h"
#include <QMap>
#include <memory>
#include <QOpenGLFunctions_3_3_core>

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QOpenGLWidget;

class GameManagerImpl {
public:
	virtual ~GameManagerImpl() {}

	virtual void load_asset() = 0;							// 加载资源
	virtual void begin_play(QOpenGLWidget* gl) = 0;			// 设置模型等
	virtual void tick() = 0;								// 每帧变化的参数
	virtual void map_input() = 0;							// 绑定输入

	virtual SPTR_CameraComponent	set_main_camera() = 0;	// 设置主相机 
	virtual SPTR_Shader				set_main_shader() = 0;	// 设置主Shader
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

	virtual SPTR_CameraComponent	set_main_camera() override;
	virtual SPTR_Shader				set_main_shader() override;

public:
	GameManager();
	virtual ~GameManager();
	static GameManager* get_instance();

	void add_game_object(const QString& key, SPTR_GameObject value);

	// gl state
	void set_depth_test(bool enable = true, uint depth_func = GL_LESS, uint depth_mask = GL_TRUE);
	void set_stencil_test(bool enable = false, uint func = GL_EQUAL, uint ref = 1, uint mask = 0xff, uint fail = GL_KEEP, uint zfail = GL_KEEP, uint zpass = GL_KEEP);

protected:
	static GameManager* instance;
	
	SPTR_CameraComponent main_camera;
	SPTR_Shader main_shader;

	// gl state
	bool b_depth_test;
	bool b_stencil_test;
	CColor background_color;

private:
	void main_bind_input();
	void main_begin_play();
	void main_tick();
	void main_draw(const std::string& shader);
	
	// render pipe
	void base_pass();
	void post_process_pass();

private:
	QOpenGLFunctions_3_3_Core * core;

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
};
