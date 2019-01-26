#pragma once

#include "delegate.h"
#include <QVector>

DELEGATE0(MousePress, void);

class InputManager {
public:

	static void bind_mouse_press_event();
	static void exec_mouse_press_event();

	static QVector<DELEGATE_ICLASS(MousePress)*> mouse_press_handles;

private:
	InputManager(){}
};

