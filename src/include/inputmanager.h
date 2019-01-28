#pragma once

#include "delegate.h"
#include <QMap>
#include <QMutex>
#include <QThread>

class QTimer;
class CGLManager;
class QKeyEvent;
class QMosueEvent;

DELEGATE1(KeyPress, void, QKeyEvent*);
DELEGATE1(KeyRelease, void, QKeyEvent*);

DELEGATE0(InputAction, void);
DELEGATE1(InputAxis, void, float);

struct InputData;
struct InputState;
class InputThread;

class InputManager {

public:
	// mouse
	static void exec_mouse_pressed_event(QMouseEvent* event, CGLManager* gl);
	static void exec_mouse_release_event(QMouseEvent* event, CGLManager* gl);
	static void exec_mouse_moveeee_event(QMouseEvent* event, CGLManager* gl);
	static void exec_mouse_dbclick_event(QMouseEvent* event, CGLManager* gl);
	static void exec_mouse_wheeeel_event(QWheelEvent* event, CGLManager* gl);
	// key
	static void exec_key_pressed_event(QKeyEvent* event, CGLManager* gl);
	static void exec_key_release_event(QKeyEvent* event, CGLManager* gl);

	// binders
	static void map_action(const QString& key, InputState is);
	static void bind_action(const QString& key, DELEGATE_ICLASS(InputAction)* ia);
	static void exec_action();

	static void map_axis(const QString& key, InputState is);
	static void bind_axis(const QString& key, DELEGATE_ICLASS(InputAxis)* ia);
	static void exec_axis();

	static void init(CGLManager* gl);
	static void quit();
private:
	// mouse
	static void clip_cursor(int top, int left, int w, int h);
	static void unclip_cursor();
	static QTimer* mouse_sgclick_checker;
	static Qt::MouseButton cur_sgclick_button;

	// key

	// binders
	static QMap<QString, QVector<InputState> > action_maps;
	static QMap<QString, DELEGATE_ICLASS(InputAction)*> input_actions;

	static QMap<QString, QVector<InputState> > axis_maps;
	static QMap<QString, DELEGATE_ICLASS(InputAxis)*> input_axis;

	static InputData cur_input_data;
	static InputState cur_input_state;
	static InputThread* input_thread;

	static CGLManager* gl;

	InputManager(){}

private slots:
	static void mouse_sgclick();
};

#define IM_BIND_ACTION(name, type, obj, func)					\
	DELEGATE_BIND(InputAction, type, type##name##, obj, func);	\
	InputManager::bind_action(#name, type##name##);

#define IM_BIND_AXIS(name, type, obj, func)						\
	DELEGATE_BIND(InputAxis, type, type##name##, obj, func);	\
	InputManager::bind_axis(#name, type##name##);

// ======================================================================

struct InputState {

	// mouse
	QMap<Qt::MouseButton, bool> mouse_pressed;
	QMap<Qt::MouseButton, bool> mouse_sgclick;
	bool mouse_dbclick;
	bool mouse_wheel;
	bool mouse_rigid;							// 判断 press 和 release 之间有没有移动 - sgclick 用
	// axis can be used
	bool axis_mouse_x;

	// key
	QMap<Qt::Key, bool> key_single_click;
	QMap<Qt::Key, bool> key_double_click;
	QMap<Qt::Key, bool> key_longgg_click;

	// modifier
	Qt::KeyboardModifier modifier_single_click;
	Qt::KeyboardModifier modifier_double_click;
	Qt::KeyboardModifier modifier_longgg_click;
	
	InputState();
	void reset();
	bool operator == (const InputState& is); 


};
struct InputData{
	// mouse
	bool mouse_left_pressed;
	bool mouse_right_pressed;

	QVector2D mouse_pressed_position;
	QVector2D mouse_release_position;
	QVector2D mouse_moved_last_position;
	QVector2D mouse_moved_curr_position;

	float wheel_delta;
	float mouse_sensitivity;

	// key

	InputData();
};

// ======================================================================

class InputThread : public QThread {
public:
	virtual void run();
private:
	QMutex mutex;
};