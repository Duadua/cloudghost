#include "freecamera.h"
#include "inputmanager.h"

FreeCamera::FreeCamera() {}
FreeCamera::~FreeCamera() {}

void FreeCamera::bind_input() {
	MousePress<FreeCamera>* mp = new MousePress<FreeCamera>(this, &FreeCamera::click);
	InputManager::mouse_press_handles.append(mp);

	/*DELEGATE_BIND(MousePress, FreeCamera, this, click)
		DELEGATE_BIND(class_name, type, obj, func) 
		class_name<type> class_name##type##func = new class_name##type##func(obj, &type::func)

	INPUT_BIND_MOUSE_PRESS(type, obj, func)
		DELEGATE_BIND(MousePress, type, obj, func)
		InputManager::mouse_press_handles.append(MousePress##type##func)
	*/
		
}

void FreeCamera::click() {
	qDebug() << "free camera click" << endl;
}
