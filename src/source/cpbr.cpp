#include "cpbr.h"
#include "cglwidget.h"
#include "gamemanager.h"

CPBR::CPBR(QWidget *parent) : QMainWindow(parent), gl_view(nullptr) {
	// init ui
	{ init_ui(); }

	init_gl_view(geometry().top() + 50, geometry().left() + 50, geometry().width() - 100, geometry().height() -100);

}

void CPBR::init_ui() {
	ui.setupUi(this);

	// init shading menu
	{
		// red_blue_3d
		connect(ui.action_rb_3d, SIGNAL(triggered()), this, SLOT(trigger_rb_3d()));

		// polygon_mode 
		{
			auto ag = new QActionGroup(this);
			ag->addAction(ui.action_pm_fill);
			ag->addAction(ui.action_pm_line);
			connect(ag, SIGNAL(triggered(QAction*)), this, SLOT(trigger_polygon_mode(QAction*)));
		}

		// post process
		{
			auto ag = new QActionGroup(this);
			ag->addAction(ui.action_pp_normal);
			ag->addAction(ui.action_pp_gray);
			ag->addAction(ui.action_pp_invers);
			ag->addAction(ui.action_pp_blur);
			ag->addAction(ui.action_pp_sharpen);
			ag->addAction(ui.action_pp_edge_det);
			ui.action_pp_normal->setChecked(true);
			connect(ag, SIGNAL(triggered(QAction*)), this, SLOT(trigger_select_pp(QAction*)));
		}

		
	}

	// init shader_toy menu
	{
		connect(ui.action_st_show, SIGNAL(triggered()), this, SLOT(trigger_shader_toy()));
	}
	
}

CPBR::~CPBR() { 
	delete gl_view; 
	
}

void CPBR::init_gl_view(int a, int b, int w, int h) {
	gl_view = new CGLWidget(this);
	gl_view->setGeometry(a, b, w, h);
}

void CPBR::resizeEvent(QResizeEvent *event) {
	if (gl_view == nullptr) return;
	gl_view->resize( geometry().width() - 100, geometry().height() - 100);
}

void CPBR::trigger_rb_3d() { GameManager::get_instance()->set_b_use_vr(!GameManager::get_instance()->get_b_use_vr()); }
void CPBR::trigger_polygon_mode(QAction* act) {
	if (act->objectName().compare("action_pm_fill") == 0) {
		GameManager::get_instance()->set_front_polygon_mode(GL_FILL);
		GameManager::get_instance()->set_back_polygon_mode(GL_LINE);
	}
	else if (act->objectName().compare("action_pm_line") == 0) {
		GameManager::get_instance()->set_front_polygon_mode(GL_LINE);
		GameManager::get_instance()->set_back_polygon_mode(GL_FILL);
	}
}
void CPBR::trigger_select_pp(QAction* act) {
	if (act->objectName().compare("action_pp_normal") == 0) {
		GameManager::get_instance()->set_pp_type(PostProcessType::NOPE);
	}
	else if (act->objectName().compare("action_pp_gray") == 0) {
		GameManager::get_instance()->set_pp_type(PostProcessType::GRAY);
	}
	else if (act->objectName().compare("action_pp_invers") == 0) {
		GameManager::get_instance()->set_pp_type(PostProcessType::INVERS);
	}
	else if (act->objectName().compare("action_pp_blur") == 0) {
		GameManager::get_instance()->set_pp_type(PostProcessType::BLUR);
	}
	else if (act->objectName().compare("action_pp_sharpen") == 0) {
		GameManager::get_instance()->set_pp_type(PostProcessType::SHARPEN);
	}
	else if (act->objectName().compare("action_pp_edge_det") == 0) {
		GameManager::get_instance()->set_pp_type(PostProcessType::EDGE_DET);
	}
}



void CPBR::trigger_shader_toy() {
	GameManager::get_instance()->set_b_use_shader_toy(!GameManager::get_instance()->get_b_use_shader_toy());
}

