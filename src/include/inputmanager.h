#pragma once

#include "reflex.h"
#include "delegate.h"
#include <QSet>
#include <QFlag>

class QTimer;
DECLARE_AUTO_PTR(QTimer)

class QOpenGLWidget;
class QKeyEvent;
class QMosueEvent;

DELEGATE0(InputAction, void);
DELEGATE1(InputAxis, void, float);

struct InputData;
struct InputState;
class InputThread;

class InputManager {

public:
	// mouse
	static void exec_mouse_pressed_event(QMouseEvent* event, QOpenGLWidget* gl);
	static void exec_mouse_release_event(QMouseEvent* event, QOpenGLWidget* gl);
	static void exec_mouse_moveeee_event(QMouseEvent* event, QOpenGLWidget* gl);
	static void exec_mouse_wheeeel_event(QWheelEvent* event, QOpenGLWidget* gl);
	static void mouse_pressed_over(); // 鼠标单击结束
	
	// cursor
	static void cursor_clip();
	static void cursor_unclip();
	static void cursor_move(const QPoint& pos, int move_ignore_count = 1);
	static void cursor_show(const Qt::CursorShape& shape);
	static bool cursor_in_edge();

	// key
	static void exec_key_pressed_event(QKeyEvent* event, QOpenGLWidget* gl);
	static void exec_key_release_event(QKeyEvent* event, QOpenGLWidget* gl);
	static void key_pressed_over();

	// binders
	static void map_action(const QString& key, InputState is);
	static void bind_action(const QString& key, DELEGATE_ICLASS(InputAction) ia);
	static void exec_action();

	static void map_axis(const QString& key, InputState is);
	static void bind_axis(const QString& key, DELEGATE_ICLASS(InputAxis) ia);
	static void exec_axis();
	static void exec_axis_mouse_move();
	static void exec_axis_mouse_wheel();
	static void exec_axis_key_pressing();

	static void init(QOpenGLWidget* gl);
	static void exit();

	static const InputData& get_input_data();							

private:
	// mouse
	static QTimer timer_mouse_pressed_over;					// 清空鼠标单击 flag 用

	// key
	static QTimer timer_key_pressed_over;						// 清空键盘单击 flag 用

	// binders
	static QMap<QString, QVector<InputState> > action_maps;
	static QMap<QString, QVector<InputState> > axis_maps;

	static QMap<QString, DELEGATE_ICLASS(InputAction)>	input_actions;
	static QMap<QString, DELEGATE_ICLASS(InputAxis)>	input_axis;

	static InputData	cur_input_data;
	static InputState	cur_input_state;

	static QOpenGLWidget*	gl;

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
	MOUSE = MOUSE_X | MOUSE_Y,

	KEY_PRESSING = 0x0016

};
Q_DECLARE_FLAGS(InputAxisTypes, InputAxisType)
Q_DECLARE_OPERATORS_FOR_FLAGS(InputAxisTypes)

struct InputState {

	// mouse
	Qt::MouseButtons mouse_pressed;		// 按下事件
	Qt::MouseButtons mouse_released;	// 弹起事件
	Qt::MouseButtons mouse_pressing;	// 长按事件

	// axis can be used
	InputAxisTypes axis_types;
	float axis_scale;

	// key
	QSet<int> key_pressed;
	QSet<int> key_released;
	QSet<int> key_pressing;

	// modifier
	Qt::KeyboardModifiers modifiers;
	
	InputState();
	bool contain(const InputState& is); 
	bool operator == (const InputState& is) const; 
	bool operator < (const InputState& is) const;

};

struct InputData{
	// mouse
	QPoint mouse_pressed_pos;
	QPoint mouse_last_pos;
	QPoint mouse_cur_pos;

	float mouse_move_delta_x;
	float mouse_move_delta_y;
	float mouse_wheel_delta;
	float mouse_sensitivity;

	int mouse_pressed_count;			// 按下计数 -- 可用于实现双击等效果
	int mouse_move_ignore_count;		// 屏蔽鼠标光标移动产生的 mousemove 事件

	// key
	int key_pressed_count;
	float key_pressing_speed;

	InputData();
};


