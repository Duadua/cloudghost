#include "cglwidget.h"
#include "gamemanager.h"

CGLWidget::CGLWidget(QWidget *parent) : QOpenGLWidget(parent) { GameManager::get_instance()->pre_init(this); }
CGLWidget::~CGLWidget()					{ GameManager::get_instance()->exit(this); }

void CGLWidget::initializeGL()			{ GameManager::get_instance()->init(this); }
void CGLWidget::resizeGL(int w, int h)	{ GameManager::get_instance()->resize(this, w, h); }
void CGLWidget::paintGL()				{ GameManager::get_instance()->draw(this); update(); }

void CGLWidget::wheelEvent(QWheelEvent* event)				{ GameManager::get_instance()->exec_mouse_wheeeel_event(event, this); }
void CGLWidget::mouseMoveEvent(QMouseEvent* event)			{ GameManager::get_instance()->exec_mouse_moveeee_event(event, this); }
void CGLWidget::mousePressEvent(QMouseEvent* event)			{ GameManager::get_instance()->exec_mouse_pressed_event(event, this); }
void CGLWidget::mouseReleaseEvent(QMouseEvent* event)		{ GameManager::get_instance()->exec_mouse_release_event(event, this); }
void CGLWidget::mouseDoubleClickEvent(QMouseEvent* event)	{ GameManager::get_instance()->exec_mouse_pressed_event(event, this); }
void CGLWidget::mouse_pressed_over()						{ GameManager::get_instance()->mouse_pressed_over(); }

void CGLWidget::keyPressEvent(QKeyEvent* event)				{ GameManager::get_instance()->exec_key_pressed_event(event, this); }
void CGLWidget::keyReleaseEvent(QKeyEvent* event)			{ GameManager::get_instance()->exec_key_release_event(event, this); }
void CGLWidget::key_pressed_over()							{ GameManager::get_instance()->key_pressed_over(); }
