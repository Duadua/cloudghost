#include <QTimer>
#include <QCursor>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLShader>	// 为了使用 wind32 的 ClipCursor
#include "cglmanager.h"
#include "inputmanager.h"

QMap<QString, QVector<InputState> > InputManager::action_maps;
QMap<QString, DELEGATE_ICLASS(InputAction)*> InputManager::input_actions;

QMap<QString, QVector<InputState> > InputManager::axis_maps;
QMap<QString, DELEGATE_ICLASS(InputAxis)*> InputManager::input_axis;

QTimer* InputManager::mouse_sgclick_checker;
Qt::MouseButton InputManager::cur_sgclick_button;

InputData InputManager::cur_input_data;
InputState InputManager::cur_input_state;
InputThread* InputManager::input_thread = nullptr;

QMutex InputManager::mutex;
CGLManager* InputManager::gl;

// mouse
void InputManager::exec_mouse_pressed_event(QMouseEvent* event, CGLManager* gl) {
	
	// update cur_input_data
	if (event->button() == Qt::LeftButton) { cur_input_data.mouse_left_pressed= true; }
	else if (event->button() == Qt::RightButton) { cur_input_data.mouse_right_pressed = true; }

	if (!(cur_input_data.mouse_left_pressed && cur_input_data.mouse_right_pressed)) {
		cur_input_data.mouse_pressed_position = QVector2D(event->x(), event->y());
		cur_input_data.mouse_release_position = QVector2D(event->x(), event->y());
		cur_input_data.mouse_moved_last_position = QVector2D(event->x(), event->y());
		cur_input_data.mouse_moved_curr_position = QVector2D(event->x(), event->y());
	}

	// update cur_input_state
	cur_input_state.mouse_pressed[event->button()] = true;
	cur_input_state.mouse_rigid = false;

	// set cursor
	gl->setCursor(Qt::BlankCursor);
	// clip cursor
	QRect rect = gl->rect();
	QPoint t_a = gl->mapToGlobal(QPoint(rect.left(), rect.top()));
	QPoint t_b = gl->mapToGlobal(QPoint(rect.right(), rect.bottom()));
	clip_cursor(t_a.x(), t_a.y(), t_b.x(), t_b.y());

}
void InputManager::exec_mouse_release_event(QMouseEvent* event, CGLManager* gl) {
	// update cur_input_data
	if (event->button() == Qt::LeftButton) { cur_input_data.mouse_left_pressed = false; }
	else if (event->button() == Qt::RightButton) { cur_input_data.mouse_right_pressed = false; }

	if (!cur_input_data.mouse_left_pressed && !cur_input_data.mouse_right_pressed) {
		cur_input_data.mouse_release_position = cur_input_data.mouse_pressed_position;
		cur_input_data.mouse_moved_last_position = cur_input_data.mouse_pressed_position;
		cur_input_data.mouse_moved_curr_position = cur_input_data.mouse_pressed_position;
		QPoint pos = gl->mapToGlobal(cur_input_data.mouse_pressed_position.toPoint());
		QCursor cursor(Qt::CrossCursor);
		cursor.setPos(pos);					// 释放时回到原来按下时的位置
		gl->setCursor(cursor);
		unclip_cursor();
	}

	// update cur_input_state
	cur_input_state.mouse_pressed.remove(event->button());
	if (!cur_input_data.mouse_left_pressed && !cur_input_data.mouse_right_pressed) {
		// judge rigid -- 按下与释放之间有没有移动
		QVector2D move_differ = QVector2D(event->x(), event->y()) - cur_input_data.mouse_pressed_position;
		float differ = move_differ.length();
		if (differ <= 1e-3) { cur_input_state.mouse_rigid = true; }
		else { cur_input_state.mouse_rigid = false; }
		// single click
		cur_sgclick_button = event->button();
		if (cur_input_state.mouse_rigid) mouse_sgclick_checker->start(300);				// 判断鼠标单击事件
	}
	
	// set_cursor
	// reset cursor if no pressed
	if (!cur_input_data.mouse_left_pressed && !cur_input_data.mouse_right_pressed) {
		
	}
	
}
void InputManager::exec_mouse_moveeee_event(QMouseEvent* event, CGLManager* gl) {
	// event->pos() 越界 -- 重置鼠标位置, 且不处理越界的所有事件
	if (event->x() <= qMax(0.0f, cur_input_data.mouse_pressed_position.x() - 4000.0f) 
		|| event->x() >= qMin(gl->width() - 2.0f, cur_input_data.mouse_pressed_position.x() + 4000.0f)
		|| event->y() <= qMax(0.0f, cur_input_data.mouse_pressed_position.y() - 4000.0f)
		|| event->y() >= qMin(gl->height() - 2.0f, cur_input_data.mouse_pressed_position.y() + 4000.0f)) {
		// set cursor
		QPoint pos = gl->mapToGlobal(cur_input_data.mouse_pressed_position.toPoint());
		QCursor cursor(Qt::BlankCursor);
		cursor.setPos(pos);
		gl->setCursor(cursor);

		// update cur_input_data
		cur_input_data.mouse_moved_last_position = cur_input_data.mouse_pressed_position;
		cur_input_data.mouse_moved_curr_position = cur_input_data.mouse_pressed_position;

		return;
	}

	// update cur_input_data
	cur_input_data.mouse_moved_last_position = cur_input_data.mouse_moved_curr_position;
	cur_input_data.mouse_moved_curr_position = QVector2D(event->x(), event->y());

}
void InputManager::exec_mouse_wheeeel_event(QWheelEvent* event, CGLManager* gl) {
	// update cur_input_data
	cur_input_data.wheel_delta = event->delta();
}
void InputManager::exec_mouse_dbclick_event(QMouseEvent* event, CGLManager* gl) {
	// update cur_input_data
	qDebug() << "dc" << endl;
	// update cur_input_state
	if(mouse_sgclick_checker != nullptr) mouse_sgclick_checker->stop();			// 双击时屏蔽单击
	//cur_input_state.mouse_dbclick[event->button()] = true;
	cur_input_state.mouse_dbclick = true;
}
void InputManager::exec_mouse_sgclick() {
	cur_input_state.mouse_sgclick[cur_sgclick_button] = true;
}

// key
void InputManager::exec_key_pressed_event(QKeyEvent* event, CGLManager* gl) {

}
void InputManager::exec_key_release_event(QKeyEvent* event, CGLManager* gl) {

}

// binders
void InputManager::map_action(const QString& key, InputState is) {
	action_maps[key].append(is);
}
void InputManager::bind_action(const QString& key, DELEGATE_ICLASS(InputAction)* ia) {
	input_actions[key] = ia;
}
void InputManager::exec_action() {
	bool flag = false;
	for (auto it = input_actions.cbegin(); it != input_actions.cend(); ++it) {	// 对每一个绑定的动作
		if (!action_maps.count(it.key())) continue;								// map 里没有相应的键位绑定
		for (auto itt = action_maps[it.key()].begin(); itt != action_maps[it.key()].end(); ++itt) {
			if ((*itt) == cur_input_state) {
				// 直接执行
				(*it)->invoke();
				cur_input_state.restore((*itt));								// 执行完后手动清空标志
				flag = true;
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
	}

	// 有些 flag 因为没有事件绑定而不会执行后清空 此处手动清空 -- 这个bug有点烦
	if (!flag) {
		QVector<Qt::MouseButton> tmp;
		for (auto it = cur_input_state.mouse_sgclick.begin(); it != cur_input_state.mouse_sgclick.end(); ++it) {
			if ((*it) == true) { tmp.append(it.key()); }
		}
		for (auto it : tmp) { cur_input_state.mouse_sgclick.remove(it); }
		cur_input_state.mouse_dbclick = false;
	}
}

void InputManager::map_axis(const QString& key, InputState is) {
	axis_maps[key].append(is);
}
void InputManager::bind_axis(const QString& key, DELEGATE_ICLASS(InputAxis)* ia) {
	input_axis[key] = ia;
}
void InputManager::exec_axis() {
	for (auto it = input_axis.cbegin(); it != input_axis.cend(); ++it) {	// 对每一个绑定的动作
		if (!axis_maps.count(it.key())) continue;							// map 里没有相应的键位绑定
		for (auto itt = axis_maps[it.key()].begin(); itt != axis_maps[it.key()].end(); ++itt) {
			if ((*itt) == cur_input_state) {
				float offset = (*itt).axis_scale;
				// 遍历所使用的 axis_type
				switch ((*itt).axis_type) {
				case InputState::InputAxis::MOUSE_X: 
					offset *= cur_input_data.mouse_moved_curr_position.x() - cur_input_data.mouse_moved_last_position.x();
					offset *= cur_input_data.mouse_sensitivity;
					break;
				case InputState::InputAxis::MOUSE_Y: 
					offset *= cur_input_data.mouse_moved_curr_position.y() - cur_input_data.mouse_moved_last_position.y();
					offset *= cur_input_data.mouse_sensitivity;
					break;
				case InputState::InputAxis::WHEEL: 
					offset *= cur_input_data.wheel_delta;
					offset *= cur_input_data.mouse_sensitivity;
					cur_input_data.wheel_delta = 0.0f;
					break;
				case InputState::InputAxis::NONE: break;
				default:break;
				}
				(*it)->invoke(offset);
				cur_input_state.restore((*itt));
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
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

void InputManager::init(CGLManager* cgl) {
	gl = cgl;
	input_thread = new InputThread();
	input_thread->start();

	mouse_sgclick_checker = new QTimer(gl);
	gl->connect(mouse_sgclick_checker, SIGNAL(timeout()), gl, SLOT(mouse_sgclick()));
}
void InputManager::quit() {
	mouse_sgclick_checker->stop();
	delete mouse_sgclick_checker;

	input_thread->terminate();
	delete input_thread;
}



// ======================================================================

InputState::InputState() {
	// mouse
	mouse_pressed.clear(); 
	mouse_sgclick.clear();
	mouse_dbclick = false;
	mouse_rigid = false;

	axis_type = InputAxis::NONE;
	axis_scale = 1.0f;

	// key
	key_single_click.clear();
	key_double_click.clear();
	key_longgg_click.clear();

	modifier_single_click = Qt::NoModifier;
	modifier_double_click = Qt::NoModifier;
	modifier_longgg_click = Qt::NoModifier;
}

bool InputState::operator == (const InputState& is) {
	
	if (mouse_pressed != is.mouse_pressed) return false;
	if (mouse_sgclick != is.mouse_sgclick) return false;
	if (mouse_dbclick != is.mouse_dbclick) return false;
	//if (mouse_wheel != is.mouse_wheel) return false;

	if (key_single_click != is.key_single_click) return false;
	if (key_double_click != is.key_double_click) return false;
	if (key_longgg_click != is.key_longgg_click) return false;

	if (modifier_single_click != is.modifier_single_click) return false;
	if (modifier_double_click != is.modifier_double_click) return false;
	if (modifier_longgg_click != is.modifier_longgg_click) return false;
	
	return true;
}

void InputState::restore(const InputState& is) {
	// 某些事件执行完后需要重置一下 flag
	QVector<Qt::MouseButton> tmp;
	for (auto it = mouse_sgclick.begin(); it != mouse_sgclick.end(); ++it) {
		if (it.value() && (is.mouse_sgclick.count(it.key()) && is.mouse_sgclick[it.key()])) {
			tmp.append(it.key());
		}
	}
	for (auto it : tmp) { mouse_sgclick.remove(it); }
	if (mouse_dbclick && is.mouse_dbclick) mouse_dbclick = false;
} 

InputData::InputData() {
	mouse_left_pressed = false;
	mouse_right_pressed = false;

	mouse_pressed_position = QVector2D(0.0f, 0.0f);
	mouse_release_position = QVector2D(0.0f, 0.0f);
	mouse_moved_last_position = QVector2D(0.0f, 0.0f);
	mouse_moved_curr_position = QVector2D(0.0f, 0.0f);

	wheel_delta = 0.0f;
	mouse_sensitivity = 0.01f;
}
// ======================================================================

void InputThread::run() {
	while (true) {
		QMutexLocker locker(&InputManager::mutex);
		InputManager::exec_action();
		InputManager::exec_axis();
	}
}

