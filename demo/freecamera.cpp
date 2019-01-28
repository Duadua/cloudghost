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
	is.mouse_pressed[Qt::RightButton] = true;
	is.axis_type = InputState::InputAxis::MOUSE_X;
	InputManager::map_axis("turn", is);

	is = InputState();
	is.mouse_pressed[Qt::LeftButton] = true;
	is.axis_type = InputState::InputAxis::MOUSE_X;
	InputManager::map_axis("turn", is);

	is = InputState();
	is.mouse_pressed[Qt::RightButton] = true;
	is.axis_type = InputState::InputAxis::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager::map_axis("look_up", is);

	is = InputState();
	is.mouse_pressed[Qt::LeftButton] = true;
	is.axis_type = InputState::InputAxis::MOUSE_Y;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward_plane", is);

	is = InputState();
	is.axis_type = InputState::InputAxis::WHEEL;
	is.axis_scale = 0.2f;
	InputManager::map_axis("move_forward", is);

	IM_BIND_AXIS(turn, FreeCamera, this, &FreeCamera::turn);
	IM_BIND_AXIS(look_up, FreeCamera, this, &FreeCamera::look_up);
	IM_BIND_AXIS(move_forward, FreeCamera, this, &FreeCamera::move_forward);
	IM_BIND_AXIS(move_forward_plane, FreeCamera, this, &FreeCamera::move_forward_plane);

}

void FreeCamera::turn(float offset) {
	QVector3D new_rotation = get_root()->get_rotation();
	//qDebug() << "location" << get_root()->get_location() << endl;
	//qDebug() << "location" << get_root()->get_rotation() << endl;

	new_rotation += QVector3D(0.0f, offset, 0.0f);
	get_root()->set_roataion(new_rotation);
}
void FreeCamera::look_up(float offset) {
	QVector3D new_rotation = get_root()->get_rotation();

	// rotate x_axis
	new_rotation += QVector3D(offset, 0.0f, 0.0f);
	get_root()->set_roataion(new_rotation);
}
void FreeCamera::move_forward(float offset) {
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// move z_axis
	float yaw = qDegreesToRadians(new_rotation.y());
	float pitch = qDegreesToRadians(new_rotation.x());
	new_location += offset * QVector3D(qSin(yaw)*qCos(pitch), qSin(pitch), qCos(yaw)*qCos(pitch));

	get_root()->set_location(new_location);

}
void FreeCamera::move_right(float offset) {

}


void FreeCamera::move_forward_plane(float offset) {
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// move z_axis
	float yaw = qDegreesToRadians(new_rotation.y());
	new_location += offset * QVector3D(qSin(yaw), 0.0f, qCos(yaw));

	get_root()->set_location(new_location);
}
