#include <QCursor>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLShader>	// 为了使用 wind32 的 ClipCursor
#include "cglmanager.h"
#include "inputmanager.h"

// mouse
QMap<QString, DELEGATE_ICLASS(MousePress)*> InputManager::mouse_press_handlers;
QMap<QString, DELEGATE_ICLASS(MouseRelease)*> InputManager::mouse_release_handlers;
QMap<QString, DELEGATE_ICLASS(MouseMove)*> InputManager::mouse_move_handlers;
QMap<QString, DELEGATE_ICLASS(MouseWheel)*> InputManager::mouse_wheel_handlers;
QMap<QString, DELEGATE_ICLASS(MouseDoubleClick)*> InputManager::mouse_dclick_handlers;

QVector2D InputManager::mouse_pre_position = QVector2D(0.0f, 0.0f);
QVector2D InputManager::mouse_last_position = QVector2D(0.0f, 0.0f);
float InputManager::mouse_sensitivity = 0.1f;

bool InputManager::mouse_left_pressed = false;
bool InputManager::mouse_right_pressed = false;
bool InputManager::mouse_moved = false;

void InputManager::bind_mouse_press_event(const QString& key, DELEGATE_ICLASS(MousePress)* mp) {
	mouse_press_handlers[key] = mp;
} 
void InputManager::ubnd_mouse_press_event(const QString& key) {
	mouse_press_handlers.remove(key);
}
void InputManager::exec_mouse_press_event(QMouseEvent* event, CGLManager* gl) {
	if (event->button() == Qt::LeftButton) { mouse_left_pressed = true; }
	else if (event->button() == Qt::RightButton) { mouse_right_pressed = true; }

	// set cursor
	mouse_pre_position = QVector2D(event->x(), event->y());
	mouse_last_position = QVector2D(event->x(), event->y());
	gl->setCursor(Qt::BlankCursor);

	// clip cursor
	QRect rect = gl->rect();
	QPoint t_a = gl->mapToGlobal(QPoint(rect.left(), rect.top()));
	QPoint t_b = gl->mapToGlobal(QPoint(rect.right(), rect.bottom()));
	clip_cursor(t_a.x(), t_a.y(), t_b.x(), t_b.y());

	for (auto it = mouse_press_handlers.cbegin(); it != mouse_press_handlers.cend(); ++it) {
		(*it)->invoke(event);
	}
}

void InputManager::bind_mouse_release_event(const QString& key, DELEGATE_ICLASS(MouseRelease)* mp) {
	mouse_release_handlers[key] = mp;
}
void InputManager::ubnd_mouse_release_event(const QString& key) {
	mouse_release_handlers.remove(key);
}
void InputManager::exec_mouse_release_event(QMouseEvent* event, CGLManager* gl) {
	// set mouse state flag
	if (event->button() == Qt::LeftButton) { mouse_left_pressed = false; }
	else if (event->button() == Qt::RightButton) { mouse_right_pressed = false; }

	// judge if has moved
	QVector2D move_differ = QVector2D(event->x(), event->y()) - mouse_pre_position;
	float differ = move_differ.length();
	if (differ <= 1e-3) { mouse_moved = false; }
	else { mouse_moved = true; }

	for (auto it = mouse_release_handlers.cbegin(); it != mouse_release_handlers.cend(); ++it) {
		(*it)->invoke(event);
	}

	// reset cursor if no pressed
	if (!mouse_left_pressed && !mouse_right_pressed) {
		QPoint pos = gl->mapToGlobal(QPoint(mouse_pre_position.x(), mouse_pre_position.y()));
		QCursor cursor(Qt::CrossCursor);
		cursor.setPos(pos);					// 释放时回到原来按下时的位置
		gl->setCursor(cursor);
		unclip_cursor();
	}
}

void InputManager::bind_mouse_move_event(const QString& key, DELEGATE_ICLASS(MouseMove)* mp) {
	mouse_move_handlers[key] = mp;
}
void InputManager::ubnd_mouse_move_event(const QString& key) {
	mouse_move_handlers.remove(key);
}
void InputManager::exec_mouse_move_event(QMouseEvent* event, CGLManager* gl) {
	// event->x() 越界 -- 重置鼠标位置, 且不处理越界的所有事件
	if (event->x() <= 0 || event->x() >= gl->width() - 2 || event->y() <= 0 || event->y() >= gl->height() - 2) {
		QPoint pos = gl->mapToGlobal(QPoint(mouse_pre_position.x(), mouse_pre_position.y()));
		QCursor cursor(Qt::BlankCursor);
		cursor.setPos(pos);
		gl->setCursor(cursor);
		mouse_last_position = mouse_pre_position;
		return;
	}

	for (auto it = mouse_move_handlers.cbegin(); it != mouse_move_handlers.cend(); ++it) {
		(*it)->invoke(event);
	}

	mouse_last_position = QVector2D(event->x(), event->y());
}

void InputManager::bind_mouse_wheel_event(const QString& key, DELEGATE_ICLASS(MouseWheel)* mp) {
	mouse_wheel_handlers[key] = mp;
}
void InputManager::ubnd_mouse_wheel_event(const QString& key) {
	mouse_wheel_handlers.remove(key);
}
void InputManager::exec_mouse_wheel_event(QWheelEvent* event, CGLManager* gl) {
	for (auto it = mouse_wheel_handlers.cbegin(); it != mouse_wheel_handlers.cend(); ++it) {
		(*it)->invoke(event);
	}
}

void InputManager::bind_mouse_dclick_event(const QString& key, DELEGATE_ICLASS(MouseDoubleClick)* mp) {
	mouse_dclick_handlers[key] = mp;
}
void InputManager::ubnd_mouse_dclick_event(const QString& key) {
	mouse_dclick_handlers.remove(key);
}
void InputManager::exec_mouse_dclick_event(QMouseEvent* event, CGLManager* gl) {
	for (auto it = mouse_dclick_handlers.cbegin(); it != mouse_dclick_handlers.cend(); ++it) {
		(*it)->invoke(event);
	}
}

void InputManager::clip_cursor(int left, int top, int right, int bottom) {
	RECT rect;
	rect.left = left; rect.top = top; 
	rect.right = right; rect.bottom = bottom;
	ClipCursor(&rect);
}
void InputManager::unclip_cursor() {
	ClipCursor(nullptr);
}

// key
QMap<QString, DELEGATE_ICLASS(KeyPress)*> InputManager::key_press_handlers;
QMap<QString, DELEGATE_ICLASS(KeyRelease)*> InputManager::key_release_handlers;

void InputManager::bind_key_press_event(const QString& key, DELEGATE_ICLASS(KeyPress)* kp) {
	key_press_handlers[key] = kp;
}
void InputManager::ubnd_key_press_event(const QString& key) {
	key_press_handlers.remove(key);
}
void InputManager::exec_key_press_event(QKeyEvent* event, CGLManager* gl) {
	for (auto it = key_press_handlers.cbegin(); it != key_press_handlers.cend(); ++it) {
		(*it)->invoke(event);
	}
}

void InputManager::bind_key_release_event(const QString& key, DELEGATE_ICLASS(KeyRelease)* kp) {
	key_release_handlers[key] = kp;
}
void InputManager::ubnd_key_release_event(const QString& key) {
	key_release_handlers.remove(key);
}
void InputManager::exec_key_release_event(QKeyEvent* event, CGLManager* gl) {
	for (auto it = key_release_handlers.cbegin(); it != key_release_handlers.cend(); ++it) {
		(*it)->invoke(event);
	}
}

