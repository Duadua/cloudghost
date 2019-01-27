#pragma once

#include "cameraobject.h"

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

class FreeCamera : public CameraObject {
public:
	FreeCamera();
	~FreeCamera();

	virtual void bind_input();

protected:
	void mouse_press(QMouseEvent* event);
	void mouse_release(QMouseEvent* event);
	void mouse_move(QMouseEvent* event);
	void mouse_wheel(QWheelEvent* event);
	void mouse_dclick(QMouseEvent* event);

	void key_press(QKeyEvent* event);
	void key_release(QKeyEvent* event);

};
