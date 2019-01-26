#pragma once

#include "delegate.h"
#include <QMap>

class QMosueEvent;

DELEGATE1(MousePress, void, QMouseEvent*);
DELEGATE1(MouseRelease, void, QMouseEvent*);
DELEGATE1(MouseMove, void, QMouseEvent*);
DELEGATE1(MouseWheel, void, QWheelEvent*);
DELEGATE1(MouseDoubleClick, void, QMouseEvent*);

class InputManager {
// mouse
public:

	static void bind_mouse_press_event(const QString& key, DELEGATE_ICLASS(MousePress)* mp);
	static void ubnd_mouse_press_event(const QString& key);
	static void exec_mouse_press_event(QMouseEvent* event);

	static void bind_mouse_release_event(const QString& key, DELEGATE_ICLASS(MouseRelease)* mp);
	static void ubnd_mouse_release_event(const QString& key);
	static void exec_mouse_release_event(QMouseEvent* event);

	static void bind_mouse_move_event(const QString& key, DELEGATE_ICLASS(MouseMove)* mp);
	static void ubnd_mouse_move_event(const QString& key);
	static void exec_mouse_move_event(QMouseEvent* event);

	static void bind_mouse_wheel_event(const QString& key, DELEGATE_ICLASS(MouseWheel)* mp);
	static void ubnd_mouse_wheel_event(const QString& key);
	static void exec_mouse_wheel_event(QWheelEvent* event);

	static void bind_mouse_dclick_event(const QString& key, DELEGATE_ICLASS(MouseDoubleClick)* mp);
	static void ubnd_mouse_dclick_event(const QString& key);
	static void exec_mouse_dclick_event(QMouseEvent* event);

// key
public:

private:
	InputManager(){}

	// mouse
	static QMap<QString, DELEGATE_ICLASS(MousePress)*> mouse_press_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseRelease)*> mouse_release_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseMove)*> mouse_move_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseWheel)*> mouse_wheel_handlers;
	static QMap<QString, DELEGATE_ICLASS(MouseDoubleClick)*> mouse_dclick_handlers;

	// key
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

