#include "freecamera.h"
#include "inputmanager.h"
#include "assetmanager.h"
#include "mygamemanager.h"
#include "meshcomponent.h"
#include <QOpenGLWidget>

void MyGameManager::load_asset() {
	// 原料 -- 着色器
	AssetManager::load_shader("triangle", ":/asset/shaders/single.vert", ":/asset/shaders/single.frag");

	// 原料 -- 顶点数据
	AssetManager::load_mesh("triangle_right", "resources/models/txt/triangle_right.txt");
	AssetManager::load_mesh("triangle_regular", "resources/models/txt/triangle_regular.txt");
	AssetManager::load_mesh("rect", "resources/models/txt/rect.txt");
	AssetManager::load_mesh("circle", "resources/models/txt/circle.txt");

	AssetManager::load_mesh("cube", "resources/models/txt/cube.txt");
	AssetManager::load_mesh("cone", "resources/models/txt/cone.txt");
	AssetManager::load_mesh("cylinder", "resources/models/txt/cylinder.txt");
	AssetManager::load_mesh("sphere", "resources/models/txt/sphere.txt");

	// texture

}

void MyGameManager::begin_play(QOpenGLWidget* gl) {
	// 生成 gameobject
	auto cube_object = CREATE_CLASS(GameObject);
	add_game_object("cube01", cube_object);

	// gameobject 的 root
	auto mc = CREATE_CLASS(MeshComponent);
	mc->set_mesh("cube");
	cube_object->set_root(mc);

	// gameobject 的另一个显示组件
	auto mcc = CREATE_CLASS(MeshComponent);
	mcc->set_mesh("sphere");
	mcc->attach_to(mc);
	mcc->set_location(QVector3D(2.0f, 0.0f, 0.0f));
	mcc->set_scale(QVector3D(0.5f, 0.5f, 0.5f));

	auto mccc = CREATE_CLASS(MeshComponent);
	mccc->set_mesh("cone");
	mccc->attach_to(mcc);
	mccc->set_location(QVector3D(0.0f, 2.0f, 0.0f));
	mccc->set_scale(QVector3D(0.5f, 0.5f, 0.5f));

	// shader 静态参数赋值
	QMatrix4x4 projection, view;
	view.translate(QVector3D(0.0f, 0.0f, 3.0f));
	projection.perspective(45.0f, (GLfloat)gl->width() / gl->height(), 0.1f, 100.0f);
	auto t_shader = AssetManager::get_shader("triangle")->use();
	t_shader->set_mat4("u_view", view);
	t_shader->set_mat4("u_projection", projection);
}

void MyGameManager::tick() {

}

void MyGameManager::map_input() {
	InputState is;
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::RightButton;
	InputManager::map_action("turn_over", is);
	is = InputState();
	is.modifiers = Qt::ControlModifier;
	is.mouse_released = Qt::RightButton;
	InputManager::map_action("turn_over", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_X;
	InputManager::map_axis("turn", is);
	is = InputState();
	is.mouse_released = Qt::LeftButton;
	InputManager::map_action("turn_over", is);
	is = InputState();
	is.modifiers = Qt::ControlModifier;
	is.mouse_released = Qt::LeftButton;
	InputManager::map_action("turn_over", is);

	// look_up
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -1.0f;
	InputManager::map_axis("look_up", is);

	// move_forward
	is = InputState();
	is.axis_types = InputAxisType::WHEEL;
	is.axis_scale = 0.01f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward_plane", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_W);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_S);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_forward", is);

	// move right
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.axis_types = InputAxisType::MOUSE_X;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_D);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_right", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_A);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_right", is);

	// move up
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.axis_types = InputAxisType::MOUSE_Y;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
	// key
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_E);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = 0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::LeftButton;
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = Qt::RightButton;
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
	is = InputState();
	is.mouse_pressing = (Qt::LeftButton | Qt::RightButton);
	is.key_pressing.insert(Qt::Key_Q);
	is.axis_types = InputAxisType::KEY_PRESSING;
	is.axis_scale = -0.1f;
	InputManager::map_axis("move_up", is);
}
