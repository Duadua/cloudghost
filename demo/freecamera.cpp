#include "freecamera.h"
#include "inputmanager.h"
#include "scenecomponent.h"
#include "cameracomponent.h"
#include <QtMath>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

FreeCamera::FreeCamera() {}
FreeCamera::~FreeCamera() {}

void FreeCamera::bind_input() {
	InputState is;
	is.mouse_pressed = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);

	is = InputState();
	is.mouse_pressed = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);

	is = InputState();
	is.mouse_pressed = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager::map_axis("look_up", is);

	is = InputState();
	is.mouse_pressed = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward_plane", is);

	is = InputState();
	is.axis_types = InputAxisType::WHEEL;
	is.axis_scale = 0.01f;
	InputManager::map_axis("move_forward", is);

	is = InputState();
	is.mouse_pressed = Qt::LeftButton;
	is.mouse_pressed = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_X;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);

	is = InputState();
	is.mouse_pressed = Qt::LeftButton;
	is.mouse_pressed = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);

	//is = InputState();
	//is.mouse_dbclick.insert(Qt::LeftButton);
	//InputManager::map_action("dbclick", is);

	//is = InputState();
	//is.key_longgg_click.insert(Qt::Key_W);
	//is.axis_types = InputAxisType::KEY_LONG_CLICK;
	//InputManager::map_axis("move_forward", is);

	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::RightButton;
	InputManager::map_action("turn_over", is);

	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::LeftButton;
	InputManager::map_action("turn_over", is);

	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager::map_axis("look_up", is);

	is = InputState();
	is.mouse_pressed = Qt::LeftButton | Qt::RightButton;
	InputManager::map_action("test", is);

	IM_BIND_AXIS(turn, FreeCamera, this, &FreeCamera::turn);
	IM_BIND_ACTION(turn_over, FreeCamera, this, &FreeCamera::turn_over);
	IM_BIND_AXIS(look_up, FreeCamera, this, &FreeCamera::look_up);
	IM_BIND_AXIS(move_forward, FreeCamera, this, &FreeCamera::move_forward);
	IM_BIND_AXIS(move_forward_plane, FreeCamera, this, &FreeCamera::move_forward_plane);
	IM_BIND_AXIS(move_right, FreeCamera, this, &FreeCamera::move_right);
	IM_BIND_AXIS(move_up, FreeCamera, this, &FreeCamera::move_up);

	IM_BIND_ACTION(dbclick, FreeCamera, this, &FreeCamera::dc);
	IM_BIND_ACTION(wclick, FreeCamera, this, &FreeCamera::wclick);
	IM_BIND_AXIS(testa, FreeCamera, this, &FreeCamera::testa);
	IM_BIND_ACTION(test, FreeCamera, this, &FreeCamera::test);

}

void FreeCamera::turn(float offset) {
	QVector3D new_rotation = get_root()->get_rotation();

	new_rotation += QVector3D(0.0f, offset, 0.0f);
	get_root()->set_roataion(new_rotation);

	// set cursor pos
	InputManager::cursor_clip();
	InputManager::cursor_show(Qt::BlankCursor);
	if(InputManager::cursor_in_edge()) InputManager::cursor_move(InputManager::get_input_data().mouse_pressed_pos);
}
void FreeCamera::turn_over() {
	InputManager::cursor_unclip();
	InputManager::cursor_show(Qt::CrossCursor);
	InputManager::cursor_move(InputManager::get_input_data().mouse_pressed_pos);
}
void FreeCamera::look_up(float offset) {
	QVector3D new_rotation = get_root()->get_rotation();

	// rotate x_axis
	new_rotation += QVector3D(offset, 0.0f, 0.0f);
	get_root()->set_roataion(new_rotation);

	// set cursor pos
	InputManager::cursor_clip();
	InputManager::cursor_show(Qt::BlankCursor);
	if (InputManager::cursor_in_edge()) InputManager::cursor_move(InputManager::get_input_data().mouse_pressed_pos);
}
void FreeCamera::move_forward(float offset) {
	qDebug() << "move forward " << offset << endl;
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// move z_axis
	float yaw = qDegreesToRadians(new_rotation.y());
	float pitch = qDegreesToRadians(new_rotation.x());
	new_location += offset * QVector3D(qSin(yaw)*qCos(pitch), qSin(pitch), qCos(yaw)*qCos(pitch));

	get_root()->set_location(new_location);

}
void FreeCamera::move_right(float offset) {
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// move x_axis
	float yaw = qDegreesToRadians(new_rotation.y());
	new_location += offset * QVector3D(qCos(yaw), 0.0f, -qSin(yaw));

	get_root()->set_location(new_location);

}

void FreeCamera::move_up(float offset) {
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// move y_axis
	float pitch = qDegreesToRadians(new_rotation.x());
	float yaw = qDegreesToRadians(new_rotation.y());
	new_location += offset * QVector3D(-qSin(yaw)*qSin(pitch), qCos(pitch), -qCos(yaw)*qSin(pitch));

	get_root()->set_location(new_location);
}

void FreeCamera::move_forward_plane(float offset) {
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// move z_axis
	float yaw = qDegreesToRadians(new_rotation.y());
	new_location += offset * QVector3D(qSin(yaw), 0.0f, qCos(yaw));

	get_root()->set_location(new_location);
}

void FreeCamera::dc() {
	qDebug() << "camera dc" << endl;
}

void FreeCamera::wclick() {
	qDebug() << "w clicked from camera" << endl;
}

void FreeCamera::test() {
	qDebug() << "free camera test function" << endl;
	if (InputManager::get_input_data().mouse_pressed_count < 2) qDebug() << "single clicked" << endl;
	else { qDebug() << "db clicked" << endl; }
}

void FreeCamera::testa(float offset) {
	qDebug() << "free camera test function axis " << offset << endl;

}



