#include "cglmanager.h"
#include "gameobject.h"
#include "inputmanager.h"
#include "cameraobject.h"
#include "assetmanager.h"
#include "meshcomponent.h"
#include "cameracomponent.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "demo/freecamera.h"

CGLManager::CGLManager(QWidget *parent) : QOpenGLWidget(parent) {
	cube_object = nullptr;

	// init mouse
	//setMouseTracking(true);
	this->setCursor(Qt::CrossCursor);
	this->setFocusPolicy(Qt::StrongFocus);

	InputManager::init(this);
}

CGLManager::~CGLManager() {
	InputManager::quit();
}

void CGLManager::initializeGL() {
	// 初始化 gl_core
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	// 原料 -- 着色器
	AssetManager::load_shader("triangle", ":/asset/shaders/single.vert", ":/asset/shaders/single.frag");

	// 原料 -- 顶点数据
	AssetManager::load_mesh("cube", ":/asset/models/txt/cube.txt");
	// texture

	// 生成 gameobject
	auto mc = new MeshComponent(&AssetManager::get_mesh("cube"));
	cube_object = new GameObject(mc);

	// gameobject 的另一个显示组件
	auto mcc = new MeshComponent(&AssetManager::get_mesh("cube"));
	mcc->attach_to(mc);
	mcc->set_location(QVector3D(1.2f,0.0f, 0.0f));
	mcc->set_scale(QVector3D(0.5f, 0.5f, 0.5f));

	auto mccc = new MeshComponent(&AssetManager::get_mesh("cube"));
	mccc->attach_to(mcc);
	mccc->set_location(QVector3D(0.0f, 2.0f, 0.0f));
	mccc->set_scale(QVector3D(0.5f, 0.5f, 0.f));
	
	// init action binders
	InputState is;
	is.mouse_pressed[Qt::RightButton] = true;
	is.axis_mouse_x = true;
	InputManager::map_axis("turn_rate", is);
	InputState is2;
	is2.mouse_pressed[Qt::LeftButton] = true;
	is2.axis_mouse_x = true;
	InputManager::map_axis("turn_rate", is2);

	// 初始化 camera
	auto camera = new CameraObject();
	auto free_camera = new FreeCamera();
	free_camera->bind_input();
	main_camera = free_camera->get_camera_component();
	free_camera->get_root()->set_location(QVector3D(0.0f, 1.5f, -3.0f));
	free_camera->get_root()->set_roataion(QVector3D(-20.0f, 0.0f, 0.0f));

	// shader 静态参数赋值
	QMatrix4x4 projection, view;
	view.translate(QVector3D(0.0f, 0.0f, 3.0f));
	projection.perspective(45.0f, (GLfloat)width() / height(), 0.1f, 100.0f);
	auto& t_shader = AssetManager::get_shader("triangle").use();
	t_shader.set_mat4("view", view);
	t_shader.set_mat4("projection", projection);

	// gl 状态初始化
	core->glEnable(GL_DEPTH_TEST);
	//core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void CGLManager::resizeGL(int w, int h) {
	core->glViewport(0, 0, w, h);
}
void CGLManager::paintGL() {
	// gl 缓存初始化
	//core->glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	core->glClearColor(205.0f/255.0f, 220.0f/255.0f, 232.0f/255.0f, 1.0f);
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader 动态参数赋值
	//cube_object->get_root()->set_roataion(QVector3D(45.0f, 0.0f, 0.0f));
	auto t_shader = AssetManager::get_shader("triangle").use();
	t_shader.set_mat4("view", main_camera->get_view_mat());

	// render
	t_shader.use();
	if (cube_object != nullptr) {
		cube_object->draw(t_shader);
	}
	
	update(); // 否则 paintGL不会循环调用
}

void CGLManager::wheelEvent(QWheelEvent* event)				{ InputManager::exec_mouse_wheeeel_event(event, this); }
void CGLManager::mouseMoveEvent(QMouseEvent* event)			{ InputManager::exec_mouse_moveeee_event(event, this); }
void CGLManager::mousePressEvent(QMouseEvent* event)		{ InputManager::exec_mouse_pressed_event(event, this); }
void CGLManager::mouseReleaseEvent(QMouseEvent* event)		{ InputManager::exec_mouse_release_event(event, this); }
void CGLManager::mouseDoubleClickEvent(QMouseEvent* event)	{ InputManager::exec_mouse_dbclick_event(event, this); }

void CGLManager::keyPressEvent(QKeyEvent* event) { 
	if (event->isAutoRepeat()) qDebug() << "press ar" << endl;
	else qDebug() << "press not ar" << endl;
	InputManager::exec_key_pressed_event(event, this); 
}
void CGLManager::keyReleaseEvent(QKeyEvent* event) { 
	if (event->isAutoRepeat()) qDebug() << "release ar" << endl;
	else qDebug() << "release not ar" << endl;
	InputManager::exec_key_release_event(event, this); 
}
