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
	qDebug() << "move" << event->x() << " " << event->y() << endl;
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




