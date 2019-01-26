#include "inputmanager.h"

QVector<DELEGATE_ICLASS(MousePress)*> InputManager::mouse_press_handles;

void InputManager::bind_mouse_press_event() {
}

void InputManager::exec_mouse_press_event() {
	for (auto it = mouse_press_handles.cbegin(); it != mouse_press_handles.cend(); ++it) {
		(*it)->invoke();
	}
}

