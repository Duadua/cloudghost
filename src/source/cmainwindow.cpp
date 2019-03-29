#include "cmainwindow.h"
#include "cglwidget.h"
#include "gamemanager.h"

#include <QColorDialog>

#include "cdebuger.h"

CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent), gl_view(nullptr) {
	// init ui
	{ init_ui(); }

	init_gl_view(geometry().top() + 50, geometry().left() + 50, geometry().width() - 100, geometry().height() -100);

}

void CMainWindow::init_ui() {
	ui.setupUi(this);
	setWindowIcon(QIcon(":/cloudghost/icon/ui/icon.ico"));

	// init shading menu
	{
		// bg_color
		ui_bg_color = new QColorDialog(this);
		ui_bg_color_custom_cnt = 0;
		connect(ui.action_color_bg, SIGNAL(triggered()), this, SLOT(trigger_bg_color()));
		
		// border color
		ui_bd_color = new QColorDialog(this);
		ui_bd_color_custom_cnt = 0;
		connect(ui.action_color_border, SIGNAL(triggered()), this, SLOT(trigger_bd_color()));

		// red_blue_3d
		connect(ui.action_rb_3d, SIGNAL(triggered()), this, SLOT(trigger_rb_3d()));

		// polygon_mode 
		{
			auto ag = new QActionGroup(this);
			ag->addAction(ui.action_pm_fill);
			ag->addAction(ui.action_pm_line);
			ag->addAction(ui.action_pm_point);
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

CMainWindow::~CMainWindow() {
	delete gl_view; 
	delete ui_bg_color;
}
void CMainWindow::closeEvent(QCloseEvent *event) {
    if(event != nullptr) {}
    QApplication::quit();
    //app->quit();
}

void CMainWindow::init_gl_view(int a, int b, int w, int h) {
	gl_view = new CGLWidget(this);
	gl_view->setGeometry(a, b, w, h);
}

void CMainWindow::resizeEvent(QResizeEvent *event) {
    if(event != nullptr) {}
	if (gl_view == nullptr) return;
	gl_view->resize( geometry().width() - 100, geometry().height() - 100);
}

// =====================================================================================

void CMainWindow::trigger_bg_color() { 
	QColor c = QColor(
		GameManager_ins().get_background_color().r(),
		GameManager_ins().get_background_color().g(),
		GameManager_ins().get_background_color().b(),
		GameManager_ins().get_background_color().a());

	ui_bg_color->setCurrentColor(c);
	if (ui_bg_color_custom_cnt == 0) { ui_bg_color->setCustomColor(0, c); }

	bool flag = true;
	auto tmp = ui_bg_color->getRgba(c.rgba(), &flag, this);
	QColor res(tmp);
	GameManager_ins().set_background_color(CColor(res.red(), res.green(), res.blue(), res.alpha())); 

	ui_bg_color_custom_cnt = (ui_bg_color_custom_cnt) % (ui_bg_color->customCount()/2 - 1) + 1;
	if(ui_bg_color_custom_cnt != 0) ui_bg_color->setCustomColor(ui_bg_color_custom_cnt * 2, tmp);
}
void CMainWindow::trigger_bd_color() {
	QColor c = QColor(
		GameManager_ins().get_border_color().r(),
		GameManager_ins().get_border_color().g(),
		GameManager_ins().get_border_color().b(),
		GameManager_ins().get_border_color().a());

	ui_bd_color->setCurrentColor(c);
	if (ui_bd_color_custom_cnt == 0) { ui_bd_color->setCustomColor(1, c); }

	bool flag = true;
	auto tmp = ui_bd_color->getRgba(c.rgba(), &flag, this);
	QColor res(tmp);
	GameManager_ins().set_border_color(CColor(res.red(), res.green(), res.blue(), res.alpha())); 

	ui_bd_color_custom_cnt = (ui_bd_color_custom_cnt) % (ui_bd_color->customCount() / 2 - 1) + 1;
	if (ui_bd_color_custom_cnt != 0) ui_bd_color->setCustomColor(ui_bd_color_custom_cnt * 2 + 1, tmp);
}

void CMainWindow::trigger_rb_3d() { GameManager_ins().set_b_use_vr(!GameManager_ins().get_b_use_vr()); }
void CMainWindow::trigger_polygon_mode(QAction* act) {
	if (act->objectName().compare("action_pm_fill") == 0) {
		GameManager_ins().set_front_polygon_mode(GL_FILL);
		GameManager_ins().set_back_polygon_mode(GL_FILL);
	}
	else if (act->objectName().compare("action_pm_line") == 0) {
		GameManager_ins().set_front_polygon_mode(GL_LINE);
		GameManager_ins().set_back_polygon_mode(GL_LINE);
	}
	else if (act->objectName().compare("action_pm_point") == 0) {
		GameManager_ins().set_front_polygon_mode(GL_POINT);
		GameManager_ins().set_back_polygon_mode(GL_POINT);
	}
}
void CMainWindow::trigger_select_pp(QAction* act) {
	if (act->objectName().compare("action_pp_normal") == 0) {
		GameManager_ins().set_pp_type(PostProcessType::NOPE);
	}
	else if (act->objectName().compare("action_pp_gray") == 0) {
		GameManager_ins().set_pp_type(PostProcessType::GRAY);
	}
	else if (act->objectName().compare("action_pp_invers") == 0) {
		GameManager_ins().set_pp_type(PostProcessType::INVERS);
	}
	else if (act->objectName().compare("action_pp_blur") == 0) {
		GameManager_ins().set_pp_type(PostProcessType::BLUR);
	}
	else if (act->objectName().compare("action_pp_sharpen") == 0) {
		GameManager_ins().set_pp_type(PostProcessType::SHARPEN);
	}
	else if (act->objectName().compare("action_pp_edge_det") == 0) {
		GameManager_ins().set_pp_type(PostProcessType::EDGE_DET);
	}
}



void CMainWindow::trigger_shader_toy() {
	GameManager_ins().set_b_use_shader_toy(!GameManager_ins().get_b_use_shader_toy());
}

