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

QTimer* InputManager::timer_mouse_sgclick;
Qt::MouseButton InputManager::mouse_sgclick_bt;

InputData InputManager::cur_input_data;
InputState InputManager::cur_input_state;

CGLManager* InputManager::gl;

// mouse
void InputManager::exec_mouse_pressed_event(QMouseEvent* event, CGLManager* gl) {
	// update cur_input_data
	if (event->button() == Qt::LeftButton) { cur_input_data.mouse_left_pressed= true; }
	else if (event->button() == Qt::RightButton) { cur_input_data.mouse_right_pressed = true; }

	if (!(cur_input_data.mouse_left_pressed && cur_input_data.mouse_right_pressed)) {
		cur_input_data.mouse_pressed_pos = event->pos();
		cur_input_data.mouse_last_pos = event->pos();
	} // 并非全按下
	else { }
	cur_input_data.mouse_rigid = false;

	// update cur_input_state
	cur_input_state.mouse_pressed.insert(event->button());

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
		cur_input_data.mouse_last_pos = cur_input_data.mouse_pressed_pos;
	} // 全释放
	else { }

	// update cur_input_state
	cur_input_state.mouse_pressed.remove(event->button());
	cur_input_state.mouse_dbpress.remove(event->button());
	if (!cur_input_data.mouse_left_pressed && !cur_input_data.mouse_right_pressed) {
		// judge rigid -- 按下与释放之间有没有移动
		QPoint move_differ = event->pos() - cur_input_data.mouse_pressed_pos;
		float differ = move_differ.manhattanLength();
		if (differ <= 1e-3) { cur_input_data.mouse_rigid = true; }
		else { cur_input_data.mouse_rigid = false; }
		// click
		mouse_sgclick_bt = event->button();
		if (cur_input_data.mouse_rigid) {
			timer_mouse_sgclick->start(300);				// 判断鼠标单击事件
			// 鼠标双击事件
			if (cur_input_data.mouse_left_dbpressed || cur_input_data.mouse_right_dbpressed) {
				if (event->button() == Qt::LeftButton) { cur_input_data.mouse_left_dbpressed = false; }
				else if (event->button() == Qt::RightButton) { cur_input_data.mouse_right_dbpressed = false; }

				if (!cur_input_data.mouse_left_dbpressed && !cur_input_data.mouse_right_dbpressed) {
					cur_input_state.mouse_dbclick.insert(event->button());
				}
			}
		}
		else {
			if (event->button() == Qt::LeftButton) { cur_input_data.mouse_left_dbpressed = false; }
			else if (event->button() == Qt::RightButton) { cur_input_data.mouse_right_dbpressed = false; }
		}
	}
	
	// set_cursor
	if (!cur_input_data.mouse_left_pressed && !cur_input_data.mouse_right_pressed) {
		QPoint pos = gl->mapToGlobal(cur_input_data.mouse_pressed_pos);
		QCursor cursor(Qt::CrossCursor);
		cursor.setPos(pos);					// 释放时回到原来按下时的位置
		gl->setCursor(cursor);
		unclip_cursor();
		if(cur_input_data.mouse_move_ignore_count == 0) cur_input_data.mouse_move_ignore_count += 8;
	} // reset cursor if all released
	
}
void InputManager::exec_mouse_moveeee_event(QMouseEvent* event, CGLManager* gl) {
	if (cur_input_data.mouse_move_ignore_count > 0) {
		--cur_input_data.mouse_move_ignore_count;
		return;
	} // ignore move event called by move cursor

	cur_input_data.mouse_move_delta_x = 180.0f * (event->x() - cur_input_data.mouse_last_pos.x()) / gl->width();
	cur_input_data.mouse_move_delta_y = 180.0f * (event->y() - cur_input_data.mouse_last_pos.y()) / gl->width();
	if (event->buttons() & Qt::LeftButton) { exec_axis(); }
	else if (event->buttons() & Qt::RightButton) { exec_axis(); }
	cur_input_data.mouse_last_pos = event->pos();

	// event->pos() 越界 -- 重置鼠标位置 
	if (event->x() <= qMax(0.0f, cur_input_data.mouse_pressed_pos.x() - 4000.0f) 
		|| event->x() >= qMin(gl->width() - 2.0f, cur_input_data.mouse_pressed_pos.x() + 4000.0f)
		|| event->y() <= qMax(0.0f, cur_input_data.mouse_pressed_pos.y() - 4000.0f)
		|| event->y() >= qMin(gl->height() - 2.0f, cur_input_data.mouse_pressed_pos.y() + 4000.0f)) {

		// update cur_input_data
		cur_input_data.mouse_last_pos = cur_input_data.mouse_pressed_pos;
		++cur_input_data.mouse_move_ignore_count;

		// set cursor
		QPoint pos = gl->mapToGlobal(cur_input_data.mouse_pressed_pos);
		QCursor cursor(Qt::BlankCursor);
		cursor.setPos(pos);
		gl->setCursor(cursor);

	}
	else { }

}
void InputManager::exec_mouse_wheeeel_event(QWheelEvent* event, CGLManager* gl) {
	// update cur_input_data
	cur_input_data.mouse_wheel_delta = event->delta();
	InputManager::exec_axis();
}
void InputManager::exec_mouse_dbclick_event(QMouseEvent* event, CGLManager* gl) {
	timer_mouse_sgclick->stop();			// 双击时屏蔽单击
	
	if (event->button() == Qt::LeftButton) { cur_input_data.mouse_left_dbpressed = true; }
	else if (event->button() == Qt::RightButton) { cur_input_data.mouse_right_dbpressed = true; }

	cur_input_state.mouse_dbpress.insert(event->button());
}
void InputManager::exec_mouse_sgclick() { cur_input_state.mouse_sgclick.insert(mouse_sgclick_bt); }

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
				flag = true;
			} // 如果与当前的键位状态相同， 则执行
		} // 遍历所有的键位绑定
	}

	// 有些 flag 因为没有事件绑定而不会执行后清空 此处手动清空 -- 这个bug有点烦
	cur_input_state.mouse_sgclick.clear();
	cur_input_state.mouse_dbclick.clear();
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
				// 遍历所使用的 axis_types
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
				case InputAxisType::NONE: break;
				default:break;
				}
				(*it)->invoke(offset);
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

	timer_mouse_sgclick = new QTimer(gl);
	gl->connect(timer_mouse_sgclick, SIGNAL(timeout()), gl, SLOT(mouse_sgclick()));
}
void InputManager::quit() {
	timer_mouse_sgclick->stop();
	delete timer_mouse_sgclick;

}

// ======================================================================

InputState::InputState() {
	// mouse
	mouse_pressed.clear(); 
	mouse_dbpress.clear();
	mouse_sgclick.clear();
	mouse_dbclick.clear();

	axis_types = InputAxisType::NONE;
	axis_scale = 1.0f;

	// key
	key_single_click.clear();
	key_double_click.clear();
	key_longgg_click.clear();

	modifier_single_click = Qt::NoModifier;
	modifier_double_click = Qt::NoModifier;
	modifier_longgg_click = Qt::NoModifier;
}

bool InputState::operator == (const InputState& is) const {

	if (mouse_pressed != is.mouse_pressed) return false;
	if (mouse_dbpress != is.mouse_dbpress) return false;
	if (mouse_sgclick != is.mouse_sgclick) return false;
	if (mouse_dbclick != is.mouse_dbclick) return false;

	if (key_single_click != is.key_single_click) return false;
	if (key_double_click != is.key_double_click) return false;
	if (key_longgg_click != is.key_longgg_click) return false;

	if (modifier_single_click != is.modifier_single_click) return false;
	if (modifier_double_click != is.modifier_double_click) return false;
	if (modifier_longgg_click != is.modifier_longgg_click) return false;
	
	return true;
}
bool InputState::operator < (const InputState& is) const { return !((*this) == is); }

InputData::InputData() {
	mouse_left_pressed = false;
	mouse_right_pressed = false;
	mouse_left_dbpressed = false;
	mouse_right_dbpressed = false;
	mouse_rigid = false;

	mouse_pressed_pos = QPoint(0.0f, 0.0f);
	mouse_last_pos = QPoint(0.0f, 0.0f);
	mouse_move_delta_x = 0.0f;
	mouse_move_delta_y = 0.0f;

	mouse_wheel_delta = 0.0f;
	mouse_sensitivity = 1.0f;

	mouse_move_ignore_count = 0;
}
