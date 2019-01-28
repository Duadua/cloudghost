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

	//IM_BIND_ACTION(turn, FreeCamera, this, &FreeCamera::turn);
	//ActionBinder<FreeCamera>* FreeCamera_turn = new ActionBinder<FreeCamera>(this, &FreeCamera::turn);
	//InputManager::bind_action("turn", FreeCamera_turn);
	IM_BIND_AXIS(turn_rate, FreeCamera, this, &FreeCamera::turn_rate);

}

void FreeCamera::turn_rate(float offset) {
	QVector3D new_location = get_root()->get_location();
	QVector3D new_rotation = get_root()->get_rotation();

	// x rotate(y_axis)
	new_rotation += QVector3D(0.0f, offset, 0.0f);
	get_root()->set_roataion(new_rotation);
}
