#include "cglwidget.h"
#include "gamemanager.h"
#include "inputmanager.h"

CGLWidget::CGLWidget(QWidget *parent) : QOpenGLWidget(parent) { 
	// init gl mouse cursor
	setCursor(Qt::CrossCursor);
	setFocusPolicy(Qt::StrongFocus);  

	// init input manager
	InputManager::init(this);

	GameManager::get_instance()->pre_init(width(), height()); 
}
CGLWidget::~CGLWidget()					{ GameManager::get_instance()->exit(); }

// =====================================================================================

void CGLWidget::initializeGL() { 
	GameManager::get_instance()->init(); 
}
void CGLWidget::resizeGL(int w, int h)	{ GameManager::get_instance()->resize(w, h); }
void CGLWidget::paintGL() { 
	// exec input
	{
		InputManager::exec_action();
		InputManager::exec_axis_key_pressing();
	}

	GameManager::get_instance()->draw(); 
	update(); 
}

void CGLWidget::wheelEvent(QWheelEvent* event)				{ InputManager::exec_mouse_wheeeel_event(event, this); }
void CGLWidget::mouseMoveEvent(QMouseEvent* event)			{ InputManager::exec_mouse_moveeee_event(event, this); }
void CGLWidget::mousePressEvent(QMouseEvent* event)			{ InputManager::exec_mouse_pressed_event(event, this); }
void CGLWidget::mouseReleaseEvent(QMouseEvent* event)		{ InputManager::exec_mouse_release_event(event, this); }
void CGLWidget::mouseDoubleClickEvent(QMouseEvent* event)	{ InputManager::exec_mouse_pressed_event(event, this); }
void CGLWidget::mouse_pressed_over()						{ InputManager::mouse_pressed_over(); }

void CGLWidget::keyPressEvent(QKeyEvent* event)				{ InputManager::exec_key_pressed_event(event, this); }
void CGLWidget::keyReleaseEvent(QKeyEvent* event)			{ InputManager::exec_key_release_event(event, this); }
void CGLWidget::key_pressed_over()							{ InputManager::key_pressed_over(); }
