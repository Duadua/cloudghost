#pragma once

#include "delegate.h"
#include <QSet>
#include <QFlag>

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
	static void exec_mouse_sgclick();

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
	static QTimer* timer_mouse_sgclick;
	static Qt::MouseButton mouse_sgclick_bt;

	// key

	// binders
	static QMap<QString, QVector<InputState> > action_maps;
	static QMap<QString, DELEGATE_ICLASS(InputAction)*> input_actions;

	static QMap<QString, QVector<InputState> > axis_maps;
	static QMap<QString, DELEGATE_ICLASS(InputAxis)*> input_axis;

	static InputData cur_input_data;
	static InputState cur_input_state;

	static CGLManager* gl;

	InputManager(){}

};

#define IM_BIND_ACTION(name, type, obj, func)					\
	DELEGATE_BIND(InputAction, type, type##name##, obj, func);	\
	InputManager::bind_action(#name, type##name##);

#define IM_BIND_AXIS(name, type, obj, func)						\
	DELEGATE_BIND(InputAxis, type, type##name##, obj, func);	\
	InputManager::bind_axis(#name, type##name##);

// ======================================================================

enum InputAxisType {
	NONE = 0x0001,
	MOUSE_X = 0x0002,
	MOUSE_Y = 0x0004,
	WHEEL = 0x0008,
	MOUSE = MOUSE_X | MOUSE_Y
};
Q_DECLARE_FLAGS(InputAxisTypes, InputAxisType)
Q_DECLARE_OPERATORS_FOR_FLAGS(InputAxisTypes)

struct InputState {

	// mouse
	QSet<Qt::MouseButton> mouse_pressed;
	QSet<Qt::MouseButton> mouse_dbpress;
	QSet<Qt::MouseButton> mouse_sgclick;
	QSet<Qt::MouseButton> mouse_dbclick;

	// axis can be used
	InputAxisTypes axis_types;
	float axis_scale;

	// key
	QSet<Qt::Key> key_single_click;
	QSet<Qt::Key> key_double_click;
	QSet<Qt::Key> key_longgg_click;

	// modifier
	Qt::KeyboardModifier modifier_single_click;
	Qt::KeyboardModifier modifier_double_click;
	Qt::KeyboardModifier modifier_longgg_click;
	
	InputState();
	bool operator == (const InputState& is) const; 
	bool operator < (const InputState& is) const;

};

struct InputData{
	// mouse
	bool mouse_left_pressed;
	bool mouse_right_pressed;
	bool mouse_left_dbpressed;
	bool mouse_right_dbpressed;
	bool mouse_rigid;							// 判断 press 和 release 之间有没有移动 - sgclick 用

	QPoint mouse_pressed_pos;
	QPoint mouse_last_pos;
	float mouse_move_delta_x;
	float mouse_move_delta_y;

	float mouse_wheel_delta;
	float mouse_sensitivity;


	int mouse_move_ignore_count;
	// key

	InputData();
};


