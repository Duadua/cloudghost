#pragma once

#include "delegate.h"
#include <QMap>

class CGLManager;
class QKeyEvent;
class QMosueEvent;

DELEGATE1(MousePress, void, QMouseEvent*);
DELEGATE1(MouseRelease, void, QMouseEvent*);
DELEGATE1(MouseMove, void, QMouseEvent*);
DELEGATE1(MouseWheel, void, QWheelEvent*);
DELEGATE1(MouseDoubleClick, void, QMouseEvent*);

DELEGATE1(KeyPress, void, QKeyEvent*);
DELEGATE1(KeyRelease, void, QKeyEvent*);

class InputManager {
// mouse
public:

	static void bind_mouse_press_event(const QString& key, DELEGATE_ICLASS(MousePress)* mp);
	static void ubnd_mouse_press_event(const QString& key);
	static void exec_mouse_press_event(QMouseEvent* event, CGLManager* gl);

	static void bind_mouse_release_event(const QString& key, DELEGATE_ICLASS(MouseRelease)* mp);
	static void ubnd_mouse_release_event(const QString& key);
	static void exec_mouse_release_event(QMouseEvent* event, CGLManager* gl);

	static void bind_mouse_move_event(const QString& key, DELEGATE_ICLASS(MouseMove)* mp);
	static void ubnd_mouse_move_event(const QString& key);
	static void exec_mouse_move_event(QMouseEvent* event, CGLManager* gl);

	static void bind_mouse_wheel_event(const QString& key, DELEGATE_ICLASS(MouseWheel)* mp);
	static void ubnd_mouse_wheel_event(const QString& key);
	static void exec_mouse_wheel_event(QWheelEvent* event, CGLManager* gl);

	static void bind_mouse_dclick_event(const QString& key, DELEGATE_ICLASS(MouseDoubleClick)* mp);
	static void ubnd_mouse_dclick_event(const QString& key);
	static void exec_mouse_dclick_event(QMouseEvent* event, CGLManager* gl);

	static void clip_cursor(int top, int left, int w, int h);
	static void unclip_cursor();

	static QVector2D mouse_pre_position;
	static QVector2D mouse_last_position;
	static float mouse_sensitivity;

	static bool mouse_left_pressed;
	static bool mouse_right_pressed;
	static bool mouse_moved;

// key
public:
	static void bind_key_press_event(const QString& key, DELEGATE_ICLASS(KeyPress)* kp);
	static void ubnd_key_press_event(const QString& key);
	static void exec_key_press_event(QKeyEvent* event, CGLManager* gl);

	static void bind_key_release_event(const QString& key, DELEGATE_ICLASS(KeyRelease)* kp);
	static void ubnd_key_release_event(const QString& key);
	static void exec_key_release_event(QKeyEvent* event, CGLManager* gl);

private:
	InputManager(){}

	// mouse
	static QMap<QString, DELEGATE_ICLASS(MousePress)*> mouse_press_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseRelease)*> mouse_release_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseMove)*> mouse_move_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseWheel)*> mouse_wheel_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseDoubleClick)*> mouse_dclick_handlers;

	// key
	static QMap<QString, DELEGATE_ICLASS(KeyPress)*> key_press_handlers;
	static QMap<QString, DELEGATE_ICLASS(KeyRelease)*> key_release_handlers;

};

#define IM_BIND_MOUSE_PRESS(name, type, obj, func)		\
	DELEGATE_BIND(MousePress, type, name, obj, func);	\
	InputManager::bind_mouse_press_event(#name, name);
	
#define IM_BIND_MOUSE_RELEASE(name, type, obj, func)	\
	DELEGATE_BIND(MouseRelease, type, name, obj, func);	\
	InputManager::bind_mouse_release_event(#name, name);

#define IM_BIND_MOUSE_MOVE(name, type, obj, func)		\
	DELEGATE_BIND(MouseMove, type, name, obj, func);	\
	InputManager::bind_mouse_move_event(#name, name);

#define IM_BIND_MOUSE_WHEEL(name, type, obj, func)		\
	DELEGATE_BIND(MouseWheel, type, name, obj, func);	\
	InputManager::bind_mouse_wheel_event(#name, name);

#define IM_BIND_MOUSE_DCLICK(name, type, obj, func)			\
	DELEGATE_BIND(MouseDoubleClick, type, name, obj, func);	\
	InputManager::bind_mouse_dclick_event(#name, name);

#define IM_BIND_KEY_PRESS(name, type, obj, func)			\
	DELEGATE_BIND(KeyPress, type, name, obj, func);			\
	InputManager::bind_key_press_event(#name, name);



