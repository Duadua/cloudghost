#pragma once

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

	virtual void load_asset() = 0;						// 加载资源
	virtual void begin_play(QOpenGLWidget* gl) = 0;		// 设置模型等
	virtual void tick() = 0;							// 每帧变化的参数
	virtual void map_input() = 0;						// 绑定输入
	virtual SPTR_CameraComponent set_main_camera() = 0; // 设置主相机 

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
	virtual SPTR_CameraComponent set_main_camera() override;

public:
	GameManager();
	virtual ~GameManager();
	static GameManager* get_instance();

	void add_game_object(const QString& key, SPTR_GameObject value);

protected:
	static GameManager* instance;
	
	SPTR_CameraComponent main_camera;
	SPTR_Shader main_shader;

private:
	void main_bind_input();
	void main_begin_play();
	void main_tick();
	void main_draw();
	
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
