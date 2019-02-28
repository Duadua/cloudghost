#include "freecamera.h"
#include "inputmanager.h"
#include "scenecomponent.h"
#include "cameracomponent.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

IMPLEMENT_CLASS(FreeCamera)

FreeCamera::FreeCamera() {}
FreeCamera::~FreeCamera() {}

void FreeCamera::bind_input() {

	IM_BIND_AXIS(turn, FreeCamera, this, &FreeCamera::turn);
	IM_BIND_ACTION(turn_over, FreeCamera, this, &FreeCamera::turn_over);
	IM_BIND_AXIS(look_up, FreeCamera, this, &FreeCamera::look_up);

	IM_BIND_AXIS(move_forward, FreeCamera, this, &FreeCamera::move_forward);
	IM_BIND_AXIS(move_forward_plane, FreeCamera, this, &FreeCamera::move_forward_plane);
	IM_BIND_AXIS(move_right, FreeCamera, this, &FreeCamera::move_right);
	IM_BIND_AXIS(move_up, FreeCamera, this, &FreeCamera::move_up);

}

void FreeCamera::turn(float offset) {
	CVector3D new_rotation = get_root_component()->get_rotation();

	new_rotation += CVector3D(0.0f, offset, 0.0f);
	get_root_component()->set_rotation(new_rotation);

	// set cursor pos
	InputManager::cursor_clip();
	InputManager::cursor_show(Qt::BlankCursor);
	if(InputManager::cursor_in_edge()) InputManager::cursor_move(InputManager::get_input_data().mouse_pressed_pos);
}
void FreeCamera::turn_over() {
	InputManager::cursor_unclip();
	InputManager::cursor_show(Qt::CrossCursor);
	InputManager::cursor_move(InputManager::get_input_data().mouse_pressed_pos, 4);
}
void FreeCamera::look_up(float offset) {
	CVector3D new_rotation = get_root_component()->get_rotation();

	// rotate x_axis
	new_rotation += CVector3D(offset, 0.0f, 0.0f);
	get_root_component()->set_rotation(new_rotation);
	
	// set cursor pos
	InputManager::cursor_clip();
	InputManager::cursor_show(Qt::BlankCursor);
	if (InputManager::cursor_in_edge()) InputManager::cursor_move(InputManager::get_input_data().mouse_pressed_pos);
}

void FreeCamera::move_forward(float offset) {
	CVector3D new_location = get_root_component()->get_location();
	CVector3D new_rotation = get_root_component()->get_rotation();

	// move z_axis
	float yaw = CMath::deg_to_rad(new_rotation.y());
	float pitch = CMath::deg_to_rad(new_rotation.x());
	new_location += offset * CVector3D(std::sin(yaw)*std::cos(pitch), std::sin(pitch), std::cos(yaw)*std::cos(pitch));

	get_root_component()->set_location(new_location);

}
void FreeCamera::move_forward_plane(float offset) {
	CVector3D new_location = get_root_component()->get_location();
	CVector3D new_rotation = get_root_component()->get_rotation();

	// move z_axis
	float yaw = CMath::deg_to_rad(new_rotation.y());
	new_location += offset * CVector3D(std::sin(yaw), 0.0f, std::cos(yaw));

	get_root_component()->set_location(new_location);
}
void FreeCamera::move_right(float offset) {
	CVector3D new_location = get_root_component()->get_location();
	CVector3D new_rotation = get_root_component()->get_rotation();

	// move x_axis
	float yaw = CMath::deg_to_rad(new_rotation.y());
	new_location += offset * CVector3D(std::cos(yaw), 0.0f, -std::sin(yaw));

	get_root_component()->set_location(new_location);

	// set cursor pos
	InputManager::cursor_clip();
	InputManager::cursor_show(Qt::BlankCursor);
	if (InputManager::cursor_in_edge()) InputManager::cursor_move(InputManager::get_input_data().mouse_pressed_pos);
}
void FreeCamera::move_up(float offset) {
	CVector3D new_location = get_root_component()->get_location();
	CVector3D new_rotation = get_root_component()->get_rotation();

	// move y_axis
	float pitch = CMath::deg_to_rad(new_rotation.x());
	float yaw = CMath::deg_to_rad(new_rotation.y());
	new_location += offset * CVector3D(-std::sin(yaw)*std::sin(pitch), std::cos(pitch), -std::cos(yaw)*std::sin(pitch));

	get_root_component()->set_location(new_location);
}
