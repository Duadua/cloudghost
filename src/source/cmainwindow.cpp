#include "cmainwindow.h"
#include "cglwidget.h"
#include "cspinwidget.h"
#include "csubmitwidget.h"
#include "gamemanager.h"

#include <QColorDialog>
#include <QWidgetAction>

#include "cdebuger.h"

CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent), gl_view(nullptr) {
	// init ui
	{ init_ui(); }

	init_gl_view(geometry().left() + 50, geometry().top() + 60, geometry().width() - 100, geometry().height() -100);

}

void CMainWindow::init_ui() {
	ui.setupUi(this);
	setWindowIcon(QIcon(":/cloudghost/icon/ui/icon.ico"));

	// init start menu
	{
		connect(ui.action_exit, SIGNAL(triggered()), this, SLOT(close()));
	}
	// init shading menu
	{
		// bg_color
		{
			ui_bg_color = new QColorDialog(this);
			ui_bg_color->setOption(QColorDialog::NoButtons);
			ui_bg_color->setOption(QColorDialog::ShowAlphaChannel);
			ui_bg_color_custom_cnt = 0;
			QWidgetAction *ui_action_bg_color = new QWidgetAction(this);
			ui_action_bg_color->setDefaultWidget(ui_bg_color);
			ui.menu_bg_color->addAction(ui_action_bg_color);
			connect(ui.menu_bg_color, SIGNAL(aboutToShow()), this, SLOT(trigger_bg_color_init()));

			QWidgetAction * ui_action_bg_color_submit = new QWidgetAction(this);
			CSubmitWidget* ui_bg_color_submit = new CSubmitWidget(this);
			ui_bg_color_submit->add_hspacer();
			ui_action_bg_color_submit->setDefaultWidget(ui_bg_color_submit);
			ui.menu_bg_color->addAction(ui_action_bg_color_submit);

			connect(ui_bg_color_submit, SIGNAL(ok()), this, SLOT(trigger_bg_color_ok()));
			connect(ui_bg_color_submit, SIGNAL(cancel()), this, SLOT(trigger_bg_color_cancel()));
			
		}
		
		// border color
		{
			ui_bd_color = new QColorDialog(this);
			ui_bd_color->setOption(QColorDialog::NoButtons);
			ui_bd_color->setOption(QColorDialog::ShowAlphaChannel);
			ui_bd_color_custom_cnt = 0;
			QWidgetAction *ui_action_bd_color = new QWidgetAction(this);
			ui_action_bd_color->setDefaultWidget(ui_bd_color);
			ui.menu_bd_color->addAction(ui_action_bd_color);
			connect(ui.menu_bd_color, SIGNAL(aboutToShow()), this, SLOT(trigger_bd_color_init()));

			QWidgetAction * ui_action_bd_color_submit = new QWidgetAction(this);
			CSubmitWidget* ui_bd_color_submit = new CSubmitWidget(this);
			ui_bd_color_submit->add_hspacer();
			ui_action_bd_color_submit->setDefaultWidget(ui_bd_color_submit);
			ui.menu_bd_color->addAction(ui_action_bd_color_submit);

			connect(ui_bd_color_submit, SIGNAL(ok()), this, SLOT(trigger_bd_color_ok()));
			connect(ui_bd_color_submit, SIGNAL(cancel()), this, SLOT(trigger_bg_color_cancel()));
		}

		// red_blue_3d
		{
			connect(ui.action_rb_3d, SIGNAL(triggered()), this, SLOT(trigger_rb_3d()));

			// 生成菜单栏 spinbox
			ui_eye_delta = new CSpinWidget(this);
			connect(ui_eye_delta, SIGNAL(value_changed(int)), this, SLOT(trigger_eye_delta(int)));

			QWidgetAction *ui_action_eye_delta = new QWidgetAction(this);
			ui_action_eye_delta->setDefaultWidget(ui_eye_delta);
			ui.menu_eye_delta->addAction(ui_action_eye_delta);
			connect(ui.menu_eye_delta, SIGNAL(aboutToShow()), this, SLOT(trigger_eye_delta_init()));
		}

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

void CMainWindow::trigger_bg_color_init() {
	QColor c = QColor(
		GameManager_ins().get_background_color().r(),
		GameManager_ins().get_background_color().g(),
		GameManager_ins().get_background_color().b(),
		GameManager_ins().get_background_color().a());

	ui_bg_color->setCurrentColor(c);
	if (ui_bg_color_custom_cnt == 0) { ui_bg_color->setCustomColor(0, c); }
}
void CMainWindow::trigger_bg_color_ok() {

	auto tmp = ui_bg_color->currentColor();
	QColor res(tmp);
	GameManager_ins().set_background_color(CColor(res.red(), res.green(), res.blue(), res.alpha())); 

	ui_bg_color_custom_cnt = (ui_bg_color_custom_cnt) % (ui_bg_color->customCount()/2 - 1) + 1;
	if (ui_bg_color_custom_cnt != 0) {
		ui_bg_color->setCustomColor(ui_bg_color_custom_cnt * 2, tmp);
		ui_bg_color->update();
	}

}
void CMainWindow::trigger_bg_color_cancel() { ui.menu_shading->setVisible(false); }
void CMainWindow::trigger_bd_color_init() {
	QColor c = QColor(
		GameManager_ins().get_border_color().r(),
		GameManager_ins().get_border_color().g(),
		GameManager_ins().get_border_color().b(),
		GameManager_ins().get_border_color().a());

	ui_bd_color->setCurrentColor(c);
	if (ui_bd_color_custom_cnt == 0) { ui_bd_color->setCustomColor(1, c); }

}
void CMainWindow::trigger_bd_color_ok() {
	auto tmp = ui_bd_color->currentColor();
	QColor res(tmp);
	GameManager_ins().set_border_color(CColor(res.red(), res.green(), res.blue(), res.alpha())); 

	ui_bd_color_custom_cnt = (ui_bd_color_custom_cnt) % (ui_bd_color->customCount() / 2 - 1) + 1;
	if (ui_bd_color_custom_cnt != 0) {
		ui_bd_color->setCustomColor(ui_bd_color_custom_cnt * 2 + 1, tmp);
		ui_bd_color->update();
	}

}

void CMainWindow::trigger_rb_3d() { GameManager_ins().set_b_use_vr(!GameManager_ins().get_b_use_vr()); }
void CMainWindow::trigger_eye_delta_init() {
	ui_eye_delta->set_value(static_cast<int>(GameManager_ins().get_vr_delta()*200 + 0.5f));
}
void CMainWindow::trigger_eye_delta(int v) {
	GameManager_ins().set_vr_delta(CMath_ins().linear_lerp(0.0f, 0.5f, v*0.01f));
}

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

