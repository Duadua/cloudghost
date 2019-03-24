#pragma once

#include "reflex.h"
#include "delegate.h"
#include "singleton.h"
#include <QSet>
#include <QFlag>
#include <QPoint>
#include <QVector>
#include <QMap>
#include <QTimer>

DECLARE_AUTO_PTR(QTimer)

class QOpenGLWidget;
class QKeyEvent;
class QMosueEvent;

DELEGATE0(InputAction, void);
DELEGATE1(InputAxis, void, float);

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

struct InputData {
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

// ================================================================================

SINGLETON_CLASS(InputManager) {
	SINGLETON(InputManager)
public:
	// mouse
    void exec_mouse_pressed_event(class QMouseEvent* event);
    void exec_mouse_release_event(class QMouseEvent* event);
    void exec_mouse_moveeee_event(class QMouseEvent* event);
    void exec_mouse_wheeeel_event(class QWheelEvent* event);
	void mouse_pressed_over(); // 鼠标单击结束
	
	// cursor
	void cursor_clip();
	void cursor_unclip();
	void cursor_move(const QPoint& pos, int move_ignore_count = 1);
	void cursor_show(const Qt::CursorShape& shape);
	bool cursor_in_edge();

	// key
    void exec_key_pressed_event(QKeyEvent* event);
    void exec_key_release_event(QKeyEvent* event);
	void key_pressed_over();

	// binders
	void map_action(const QString& key, InputState is);
	void bind_action(const QString& key, DELEGATE_ICLASS(InputAction) ia);
	void exec_action();

	void map_axis(const QString& key, InputState is);
	void bind_axis(const QString& key, DELEGATE_ICLASS(InputAxis) ia);
	void exec_axis(float delta);
	void exec_axis_mouse_move(float delta);
	void exec_axis_mouse_wheel(float delta);
	void exec_axis_key_pressing(float delta);

	void init(QOpenGLWidget* gl);
	void exit();

	const InputData& get_input_data();							

	QOpenGLWidget*	gl;

private:
	// mouse
	QTimer timer_mouse_pressed_over;					// 清空鼠标单击 flag 用

	// key
	QTimer timer_key_pressed_over;						// 清空键盘单击 flag 用

	// binders
	QMap<QString, QVector<InputState> > action_maps;
	QMap<QString, QVector<InputState> > axis_maps;

	QMap<QString, DELEGATE_ICLASS(InputAction)>	input_actions;
	QMap<QString, DELEGATE_ICLASS(InputAxis)>	input_axis;

	InputData	cur_input_data;
	InputState	cur_input_state;

};
SINGLETON_X(InputManager)

#define IM_BIND_ACTION(name, type, obj, func)					\
    DELEGATE_BIND(InputAction, type, type##name, obj, func);	\
    InputManager_ins().bind_action(#name, type##name);

#define IM_BIND_AXIS(name, type, obj, func)						\
    DELEGATE_BIND(InputAxis, type, type##name, obj, func);	\
    InputManager_ins().bind_axis(#name, type##name);

// 没有类对象时使用
#define IM_BIND_ACTION_VOID(name, type, func)					\
    DELEGATE_BIND_VOID(InputAction, type##name, func);		\
    InputManager_ins().bind_action(#name, type##name);

#define IM_BIND_AXIS_VOID(name, type, func)						\
    DELEGATE_BIND_VOID(InputAxis, type##name, func);			\
    InputManager_ins().bind_axis(#name, type##name);

// ======================================================================
