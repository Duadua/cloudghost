#include "cglmanager.h"
#include "assetmanager.h"

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
	GLfloat vertices[] = {
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top
	};

	// vbo
	core->glGenBuffers(1, &vbo);
	core->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	core->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vao
	core->glGenVertexArrays(1, &vao);
	core->glBindVertexArray(vao);
	core->glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	core->glEnableVertexAttribArray(0);
	core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	core->glEnableVertexAttribArray(1);

	core->glBindBuffer(GL_ARRAY_BUFFER, 0);
	core->glBindVertexArray(0);
	// texture

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
	core->glBindVertexArray(vao);
	core->glDrawArrays(GL_TRIANGLES, 0, 3);
	core->glBindVertexArray(0);
}
