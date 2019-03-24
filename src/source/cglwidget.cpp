#include "cglwidget.h"
#include "timemanager.h"
#include "gamemanager.h"
#include "inputmanager.h"

CGLWidget::CGLWidget(QWidget *parent) : QOpenGLWidget(parent) { 
	// init gl mouse cursor
	setCursor(Qt::CrossCursor);
	setFocusPolicy(Qt::StrongFocus);  

	// init input manager
	InputManager_ins().init(this);

    GameManager_ins().pre_init(static_cast<uint>(width()), static_cast<uint>(height()));
}
CGLWidget::~CGLWidget()					{ GameManager_ins().exit(); }

// =====================================================================================

void CGLWidget::initializeGL() { 
	GameManager_ins().init(); 
}
void CGLWidget::resizeGL(int w, int h)	{ GameManager_ins().resize(static_cast<uint>(w), static_cast<uint>(h)); }
void CGLWidget::paintGL() { 
	// exec input
	{
		InputManager_ins().exec_action();
		InputManager_ins().exec_axis_key_pressing(static_cast<float>(TimeManager_ins().get_delta_tick_msconds()));
	}

	GameManager_ins().draw(); 
	update(); 
}

void CGLWidget::wheelEvent(QWheelEvent* event)				{ InputManager_ins().exec_mouse_wheeeel_event(event); }
void CGLWidget::mouseMoveEvent(QMouseEvent* event)			{ InputManager_ins().exec_mouse_moveeee_event(event); }
void CGLWidget::mousePressEvent(QMouseEvent* event)			{ InputManager_ins().exec_mouse_pressed_event(event); }
void CGLWidget::mouseReleaseEvent(QMouseEvent* event)		{ InputManager_ins().exec_mouse_release_event(event); }
void CGLWidget::mouseDoubleClickEvent(QMouseEvent* event)	{ InputManager_ins().exec_mouse_pressed_event(event); }
void CGLWidget::mouse_pressed_over()						{ InputManager_ins().mouse_pressed_over(); }

void CGLWidget::keyPressEvent(QKeyEvent* event)				{ InputManager_ins().exec_key_pressed_event(event); }
void CGLWidget::keyReleaseEvent(QKeyEvent* event)			{ InputManager_ins().exec_key_release_event(event); }
void CGLWidget::key_pressed_over()							{ InputManager_ins().key_pressed_over(); }
