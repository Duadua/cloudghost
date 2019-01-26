#include "scenecomponent.h"
#include "freecamera.h"
#include "inputmanager.h"
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
}

void FreeCamera::mouse_press(QMouseEvent* event) {
	qDebug() << event->x() << " " << event->y() << endl;
	if (event->button() == Qt::LeftButton) {
		qDebug() << "left" << endl;
	}
	else if (event->button() == Qt::RightButton) {
		qDebug() << "right" << endl;
	}
}

void FreeCamera::mouse_release(QMouseEvent* event) {
	qDebug() << event->x() << " " << event->y() << endl;
	if (event->button() == Qt::LeftButton) {
		qDebug() << "release_left" << endl;
	}
	else if (event->button() == Qt::RightButton) {
		qDebug() << "release_right" << endl;
	}

}

void FreeCamera::mouse_move(QMouseEvent* event) {
	float x_offset = event->x() - InputManager::mouse_last_position.x();
	float y_offset = event->y() - InputManager::mouse_last_position.y();
	x_offset *= InputManager::mouse_sensitivity;
	y_offset *= InputManager::mouse_sensitivity;

	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	if (InputManager::mouse_left_pressed && !InputManager::mouse_right_pressed) {
		// x rotate(y_axis)
		new_rotation += QVector3D(0.0f, x_offset, 0.0f);
		// y move(z_axis) 
		
	}
	else if (!InputManager::mouse_left_pressed && InputManager::mouse_right_pressed) {
		// x rotate(y_axis)
		new_rotation += QVector3D(0.0f, x_offset, 0.0f);
		// y rotate(x_axis)
		new_rotation += QVector3D(-y_offset, 0.0f, 0.0f);
	}
	else if (InputManager::mouse_left_pressed && InputManager::mouse_right_pressed) {
		// x move(x_axis)

		// y move(y_axis)
	}
	
	//qDebug() << new_rotation << endl;
	get_root()->set_location(new_location);
	get_root()->set_roataion(new_rotation);
}

void FreeCamera::mouse_wheel(QWheelEvent* event) {
	if (event->delta() > 0) {
		qDebug() << "up wheel" << endl;
	}
	else {
		qDebug() << "down wheel" << endl;
	}
}

void FreeCamera::mouse_dclick(QMouseEvent* event) {
	qDebug() << "double clicked" << endl;
}




