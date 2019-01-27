#include "scenecomponent.h"
#include "freecamera.h"
#include "inputmanager.h"
#include <QtMath>
#include <QMouseEvent>
#include <QWheelEvent>

FreeCamera::FreeCamera() {}
FreeCamera::~FreeCamera() {}

void FreeCamera::bind_input() {
	IM_BIND_MOUSE_PRESS(freecamera_mouse_press, FreeCamera, this, &FreeCamera::mouse_press);
	IM_BIND_MOUSE_RELEASE(freecamera_mouse_release, FreeCamera, this, &FreeCamera::mouse_release);
	IM_BIND_MOUSE_MOVE(freecamera_mouse_move, FreeCamera, this, &FreeCamera::mouse_move);
	IM_BIND_MOUSE_WHEEL(freecamera_mouse_wheel, FreeCamera, this, &FreeCamera::mouse_wheel);
	IM_BIND_MOUSE_DCLICK(freecamera_mouse_dclick, FreeCamera, this, &FreeCamera::mouse_dclick);

	//IM_BIND_KEY_PRESS(move_forward, FreeCamera, this, &FreeCamera::move_forward);
}

void FreeCamera::mouse_press(QMouseEvent* event) {
	/*qDebug() << event->x() << " " << event->y() << endl;
	if (event->button() == Qt::LeftButton) {
		qDebug() << "left" << endl;
	}
	else if (event->button() == Qt::RightButton) {
		qDebug() << "right" << endl;
	}*/
	// set mouse state flag
	
}
void FreeCamera::mouse_release(QMouseEvent* event) {
	/*qDebug() << event->x() << " " << event->y() << endl;
	if (event->button() == Qt::LeftButton) {
		qDebug() << "release_left" << endl;
	}
	else if (event->button() == Qt::RightButton) {
		qDebug() << "release_right" << endl;
	}*/

}
void FreeCamera::mouse_move(QMouseEvent* event) {
	float x_offset = event->x() - InputManager::mouse_last_position.x();
	float y_offset = event->y() - InputManager::mouse_last_position.y();
	x_offset *= InputManager::mouse_sensitivity;
	y_offset *= -InputManager::mouse_sensitivity;

	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	if (InputManager::mouse_left_pressed && !InputManager::mouse_right_pressed) {
		// x rotate(y_axis)
		new_rotation += QVector3D(0.0f, x_offset, 0.0f);
		// y move(z_axis) 
		float yaw = qDegreesToRadians(new_rotation.y());
		new_location += y_offset*0.1f * QVector3D(qSin(yaw), 0.0f, qCos(yaw));
	}
	else if (!InputManager::mouse_left_pressed && InputManager::mouse_right_pressed) {
		// x rotate(y_axis)
		new_rotation += QVector3D(0.0f, x_offset, 0.0f);
		// y rotate(x_axis)
		new_rotation += QVector3D(y_offset, 0.0f, 0.0f);
	}
	else if (InputManager::mouse_left_pressed && InputManager::mouse_right_pressed) {
		// x move(x_axis)
		float yaw = qDegreesToRadians(new_rotation.y());
		float pitch = qDegreesToRadians(new_rotation.x());
		new_location += x_offset*0.1f * QVector3D(qCos(yaw), 0.0f, -qSin(yaw));
		// y move(y_axis)
		new_location += y_offset * 0.1f * QVector3D(-qSin(yaw)*qSin(pitch), qCos(pitch), -qCos(yaw)*qSin(pitch));
	}
	
	get_root()->set_location(new_location);
	get_root()->set_roataion(new_rotation);
}
void FreeCamera::mouse_wheel(QWheelEvent* event) {
	float offset = event->delta() * InputManager::mouse_sensitivity;
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// move z_axis
	float yaw = qDegreesToRadians(new_rotation.y());
	new_location += offset*0.1f * QVector3D(qSin(yaw), 0.0f, qCos(yaw));
	get_root()->set_location(new_location);

}
void FreeCamera::mouse_dclick(QMouseEvent* event) {
}

void FreeCamera::move_forward() {

}
