#include "cglmanager.h"
#include "assetmanager.h"
#include "meshcomponent.h"
#include "gameobject.h"

CGLManager::CGLManager(QWidget *parent) : QOpenGLWidget(parent) {
}

CGLManager::~CGLManager() {
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
	auto mc = new MeshComponent();
	mc->set_mesh(&AssetManager::get_mesh("cube"));
	auto go = new GameObject();
	go->set_root(mc);
	
	// shader 静态参数赋值

	// gl 状态初始化
	core->glEnable(GL_DEPTH_TEST);
	//core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void CGLManager::resizeGL(int w, int h) {
	core->glViewport(0, 0, w, h);
}
void CGLManager::paintGL() {
	// gl 缓存初始化
	core->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// shader 动态参数赋值
	AssetManager::get_shader("triangle").use();

	// render
	AssetManager::get_shader("triangle").use();
}
