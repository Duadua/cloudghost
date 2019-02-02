#include <QTime>
#include <QTimer>
#include <QCursor>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLShader>	// 为了使用 wind32 的 ClipCursor
#include "cglmanager.h"
#include "inputmanager.h"

// mouse
QTimer* InputManager::timer_mouse_pressed_over;

// key
QTimer* InputManager::timer_key_pressed_over;

// binders

QMap<QString, QVector<InputState> > InputManager::axis_maps;
QMap<QString, QVector<InputState> > InputManager::action_maps;

QMap<QString, DELEGATE_ICLASS(InputAxis)*> InputManager::input_axis;
QMap<QString, DELEGATE_ICLASS(InputAction)*> InputManager::input_actions;

InputData InputManager::cur_input_data;
InputState InputManager::cur_input_state;

CGLManager* InputManager::gl;

// mouse
void InputManager::exec_mouse_pressed_event(QMouseEvent* event, CGLManager* gl) {
	cur_input_state.mouse_pressing = event->buttons();

	if (event->buttons() != (Qt::LeftButton | Qt::RightButton)) {
		cur_input_state.mouse_pressed = event->button();
		cur_input_data.mouse_pressed_pos = event->pos();
		cur_input_data.mouse_pressed_count = (cur_input_data.mouse_pressed_count + 1) % 3;

		timer_mouse_pressed_over->stop();
		timer_mouse_pressed_over->start(300);

		cur_input_data.mouse_last_pos = event->pos();
		cur_input_data.mouse_cur_pos = event->pos();
	} // 只有一个键按下
	
	exec_axis_mouse_move();
}
void InputManager::exec_mouse_release_event(QMouseEvent* event, CGLManager* gl) {

	cur_input_state.mouse_released = event->button();
	cur_input_state.mouse_pressing ^= event->button();

	exec_axis_mouse_move();
}
void InputManager::exec_mouse_moveeee_event(QMouseEvent* event, CGLManager* gl) {
	if (cur_input_data.mouse_move_ignore_count > 0) {
		--cur_input_data.mouse_move_ignore_count;
		return;
	} // ignore move event called by move cursor

	cur_input_data.mouse_move_delta_x = 180.0f * (event->x() - cur_input_data.mouse_last_pos.x()) / gl->width();
	cur_input_data.mouse_move_delta_y = 180.0f * (event->y() - cur_input_data.mouse_last_pos.y()) / gl->width();
	cur_input_data.mouse_last_pos = event->pos();
	cur_input_data.mouse_cur_pos = event->pos();
	
	exec_axis_mouse_move();
}
void InputManager::exec_mouse_wheeeel_event(QWheelEvent* event, CGLManager* gl) {
	cur_input_data.mouse_wheel_delta = event->delta();
	exec_axis_mouse_wheel();
}
void InputManager::mouse_pressed_over() {
	cur_input_data.mouse_pressed_count = 0;
	timer_mouse_pressed_over->stop();
}

// key
void InputManager::exec_key_pressed_event(QKeyEvent* event, CGLManager* gl) {

	if (!event->isAutoRepeat()) {
		cur_input_state.key_pressing.insert(event->key());

		cur_input_state.key_pressed.insert(event->key());
		cur_input_data.key_pressed_count = (cur_input_data.key_pressed_count + 1) % 3;
		timer_key_pressed_over->stop();
		timer_key_pressed_over->start(300);

	} // 第一次按下
	else { }
}
void InputManager::exec_key_release_event(QKeyEvent* event, CGLManager* gl) {
	if (!event->isAutoRepeat()) {
		cur_input_state.key_pressing.remove(event->key());
	
		cur_input_state.key_released.insert(event->key());
	}
	else { }
}
void InputManager::key_pressed_over() {
	timer_key_pressed_over->stop();
	cur_input_data.key_pressed_count = 0;
}

// binders
void InputManager::map_action(const QString& key, InputState is) { action_maps[key].append(is); }
void InputManager::bind_action(const QString& key, DELEGATE_ICLASS(InputAction)* ia) { input_actions[key] = ia; }
void InputManager::exec_action() {
	for (auto it = input_actions.cbegin(); it != input_actions.cend(); ++it) {	// 对每一个绑定的动作
		if (!action_maps.count(it.key())) continue;								// map 里没有相应的键位绑定
		for (auto itt = action_maps[it.key()].begin(); itt != action_maps[it.key()].end(); ++itt) {
			if (cur_input_state.contain((*itt))) {
				// 直接执行
				(*it)->invoke();
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
	}

	// 有些 flag 因为没有事件绑定而不会执行后清空 此处手动清空 -- 这个bug有点烦
	cur_input_state.mouse_pressed = Qt::NoButton;
	cur_input_state.mouse_released = Qt::NoButton;

	cur_input_state.key_pressed.clear();
	cur_input_state.key_released.clear();
}

void InputManager::map_axis(const QString& key, InputState is) { axis_maps[key].append(is); }
void InputManager::bind_axis(const QString& key, DELEGATE_ICLASS(InputAxis)* ia) { input_axis[key] = ia; }
void InputManager::exec_axis() {
	for (auto it = input_axis.cbegin(); it != input_axis.cend(); ++it) {	// 对每一个绑定的动作
		if (!axis_maps.count(it.key())) continue;							// map 里没有相应的键位绑定
		for (auto itt = axis_maps[it.key()].begin(); itt != axis_maps[it.key()].end(); ++itt) {
			if (cur_input_state.contain((*itt))) {
				float offset = (*itt).axis_scale;
				// 遍历所使用的 axis_types
				bool flag = true;
				switch ((*itt).axis_types) {
				case InputAxisType::MOUSE_X: 
					offset *= cur_input_data.mouse_move_delta_x;
					offset *= cur_input_data.mouse_sensitivity;
					cur_input_data.mouse_move_delta_x = 0.0f;
					break;
				case InputAxisType::MOUSE_Y: 
					offset *= cur_input_data.mouse_move_delta_y;
					offset *= cur_input_data.mouse_sensitivity;
					cur_input_data.mouse_move_delta_y = 0.0f;
					break;
				case InputAxisType::WHEEL: 
					offset *= cur_input_data.mouse_wheel_delta;
					offset *= cur_input_data.mouse_sensitivity;
					cur_input_data.mouse_wheel_delta = 0.0f;
					break;
				case InputAxisType::KEY_PRESSING:
					//offset *= cur_input_data.key_longclick_speed;
					break;
				case InputAxisType::NONE: flag = false; break;
				default: flag = false; break;
				}
				if(flag) (*it)->invoke(offset);
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
	}
	
}
void InputManager::exec_axis_mouse_move() {
	for (auto it = input_axis.cbegin(); it != input_axis.cend(); ++it) {	// 对每一个绑定的动作
		if (!axis_maps.count(it.key())) continue;							// map 里没有相应的键位绑定
		for (auto itt = axis_maps[it.key()].begin(); itt != axis_maps[it.key()].end(); ++itt) {
			if (cur_input_state.contain((*itt))) {
				float offset = (*itt).axis_scale;
				// 遍历所使用的 axis_types
				bool flag = true;
				switch ((*itt).axis_types) {
				case InputAxisType::MOUSE_X: 
					offset *= cur_input_data.mouse_move_delta_x;
					offset *= cur_input_data.mouse_sensitivity;
					cur_input_data.mouse_move_delta_x = 0.0f;
					break;
				case InputAxisType::MOUSE_Y: 
					offset *= cur_input_data.mouse_move_delta_y;
					offset *= cur_input_data.mouse_sensitivity;
					cur_input_data.mouse_move_delta_y = 0.0f;
					break;
				default: flag = false; break;
				}
				if(flag) (*it)->invoke(offset);
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
	}
}
void InputManager::exec_axis_mouse_wheel() {
	for (auto it = input_axis.cbegin(); it != input_axis.cend(); ++it) {	// 对每一个绑定的动作
		if (!axis_maps.count(it.key())) continue;							// map 里没有相应的键位绑定
		for (auto itt = axis_maps[it.key()].begin(); itt != axis_maps[it.key()].end(); ++itt) {
			if (cur_input_state.contain((*itt))) {
				float offset = (*itt).axis_scale;
				// 遍历所使用的 axis_types
				bool flag = true;
				switch ((*itt).axis_types) {
				case InputAxisType::WHEEL: 
					offset *= cur_input_data.mouse_wheel_delta;
					offset *= cur_input_data.mouse_sensitivity;
					cur_input_data.mouse_wheel_delta = 0.0f;
					break;
				default: flag = false; break;
				}
				if(flag) (*it)->invoke(offset);
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
	}
}
void InputManager::exec_axis_key_pressing() {
	for (auto it = input_axis.cbegin(); it != input_axis.cend(); ++it) {	// 对每一个绑定的动作
		if (!axis_maps.count(it.key())) continue;							// map 里没有相应的键位绑定
		for (auto itt = axis_maps[it.key()].begin(); itt != axis_maps[it.key()].end(); ++itt) {
			if (cur_input_state.contain((*itt))) {
				float offset = (*itt).axis_scale;
				// 遍历所使用的 axis_types
				bool flag = true;
				switch ((*itt).axis_types) {
				case InputAxisType::KEY_PRESSING: 
					offset *= cur_input_data.key_pressing_speed;
					break;
				default: flag = false; break;
				}
				if(flag) (*it)->invoke(offset);
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
	}
}

void InputManager::cursor_clip() {
	QRect tmp = gl->rect();
	QPoint t_a = gl->mapToGlobal(QPoint(tmp.left(), tmp.top()));
	QPoint t_b = gl->mapToGlobal(QPoint(tmp.right(), tmp.bottom()));

	RECT rect;
	rect.left = t_a.x(); rect.top = t_a.y();
	rect.right = t_b.x(); rect.bottom = t_b.y();
	ClipCursor(&rect);
}
void InputManager::cursor_unclip() { ClipCursor(nullptr); }
void InputManager::cursor_move(const QPoint& pos, int move_ignore_count) {
	if (gl == nullptr) return;
	QCursor::setPos(gl->mapToGlobal(pos));
	cur_input_data.mouse_last_pos = pos;
	cur_input_data.mouse_cur_pos = pos;
	cur_input_data.mouse_move_ignore_count = move_ignore_count;
}
void InputManager::cursor_show(const Qt::CursorShape& shape) {
	if (gl == nullptr) return;
	QCursor cursor(shape);
	gl->setCursor(cursor);
}
bool InputManager::cursor_in_edge() {
	if (cur_input_data.mouse_cur_pos.x() <= qMax(0.0f, cur_input_data.mouse_pressed_pos.x() - 4000.0f)) return true;
	if (cur_input_data.mouse_cur_pos.x() >= qMin(gl->width() - 2.0f, cur_input_data.mouse_pressed_pos.x() + 4000.0f)) return true;
	if (cur_input_data.mouse_cur_pos.y() <= qMax(0.0f, cur_input_data.mouse_pressed_pos.y() - 4000.0f)) return true;
	if (cur_input_data.mouse_cur_pos.y() >= qMin(gl->height() - 2.0f, cur_input_data.mouse_pressed_pos.y() + 4000.0f)) return true;
	return false;
}

void InputManager::init(CGLManager* cgl) {
	gl = cgl;

	timer_mouse_pressed_over = new QTimer(gl);
	gl->connect(timer_mouse_pressed_over, SIGNAL(timeout()), gl, SLOT(mouse_pressed_over()));

	timer_key_pressed_over = new QTimer(gl);
	gl->connect(timer_key_pressed_over, SIGNAL(timeout()), gl, SLOT(key_pressed_over()));

}

void InputManager::quit() {

	timer_mouse_pressed_over->stop();
	delete timer_mouse_pressed_over;

	timer_key_pressed_over->stop();
	delete timer_key_pressed_over;

}

const InputData& InputManager::get_input_data() { return cur_input_data; }

// ======================================================================

InputState::InputState() {
	// mouse
	mouse_pressed = Qt::NoButton;
	mouse_released = Qt::NoButton;
	mouse_pressing = Qt::NoButton;

	axis_types = InputAxisType::NONE;
	axis_scale = 1.0f;

	// key
	key_pressed.clear();
	key_released.clear();
	key_pressing.clear();

	modifier_single_click = Qt::NoModifier;
	modifier_double_click = Qt::NoModifier;
	modifier_longgg_click = Qt::NoModifier;
}

bool InputState::contain(const InputState& is) {
	// mouse
	if (mouse_pressed != (mouse_pressed | is.mouse_pressed)) return false;
	if (mouse_released != (mouse_released | is.mouse_released)) return false;
	if (mouse_pressing != is.mouse_pressing) return false;

	// key
	if (!key_pressed.contains(is.key_pressed)) return false;
	if (!key_released.contains(is.key_released)) return false;
	if (!key_pressing.contains(is.key_pressing)) return false;

	return true;
}
bool InputState::operator == (const InputState& is) const {
	// mouse
	if (mouse_pressed != is.mouse_pressed) return false;
	if (mouse_released != is.mouse_pressed) return false;
	if (mouse_pressing != is.mouse_pressing) return false;

	// key
	if (key_pressed != is.key_pressed) return false;
	if (key_released != is.key_released) return false;
	if (key_pressing != is.key_pressing) return false;

	if (modifier_single_click != is.modifier_single_click) return false;
	if (modifier_double_click != is.modifier_double_click) return false;
	if (modifier_longgg_click != is.modifier_longgg_click) return false;
	
	return true;
}
bool InputState::operator < (const InputState& is) const { return !((*this) == is); }

InputData::InputData() {

	// mouse
	mouse_pressed_pos = QPoint(0.0f, 0.0f);
	mouse_last_pos = QPoint(0.0f, 0.0f);
	mouse_cur_pos = QPoint(0.0f, 0.0f);

	mouse_move_delta_x = 0.0f;
	mouse_move_delta_y = 0.0f;
	mouse_wheel_delta = 0.0f;
	mouse_sensitivity = 1.0f;

	mouse_pressed_count = 0;
	mouse_move_ignore_count = 0;

	// key
	key_pressed_count = 0;
	key_pressing_speed = 1.0;

}
