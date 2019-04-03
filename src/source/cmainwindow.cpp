#include "cmainwindow.h"
#include "cglwidget.h"
#include "cspinwidget.h"
#include "csubmitwidget.h"
#include "gamemanager.h"
#include "ctextwidget.h"

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

		// normal visual
		{
			connect(ui.action_normal_visual, SIGNAL(triggered()), this, SLOT(trigger_normal_visual()));
		}
		// explode
		{
			connect(ui.action_explode, SIGNAL(triggered()), this, SLOT(trigger_explode()));
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
			ag->addAction(ui.action_pp_real_rb3d);
			ui.action_pp_normal->setChecked(true);
			connect(ag, SIGNAL(triggered(QAction*)), this, SLOT(trigger_select_pp(QAction*)));
		}

		// gamma
		{
			connect(ui.action_gamma, SIGNAL(triggered()), this, SLOT(trigger_gamma()));

			// 生成菜单栏 spinbox
			ui_gamma_value = new CSpinWidget(this);
			connect(ui_gamma_value, SIGNAL(value_changed(int)), this, SLOT(trigger_gamma_value(int)));

			QWidgetAction *ui_action_gamma = new QWidgetAction(this);
			ui_action_gamma->setDefaultWidget(ui_gamma_value);
			ui.menu_gamma_value->addAction(ui_action_gamma);
			connect(ui.menu_gamma_value, SIGNAL(aboutToShow()), this, SLOT(trigger_gamma_init()));
		}
		
		// msaa
		{
			connect(ui.action_msaa, SIGNAL(triggered()), this, SLOT(trigger_msaa()));
		}

		
	}

	// init shader_toy menu
	{
		connect(ui.action_st_show, SIGNAL(triggered()), this, SLOT(trigger_shader_toy()));
	}
	
	// init help menu
	{
		// init guide widget
		{
			ui_guide = new CTextWidget(this);
			ui_guide->setVisible(false);
			ui_guide->setWindowTitle(QString::fromUtf8("操作指南"));
			std::string str = "[移动]\t鼠标右键 + WASD\n";
			str += "[移动]\t鼠标左键 + WASD\n";
			str += "[移动]\t鼠标左右键(同时按) + WASD\n";
			str += "[环顾]\t鼠标右键滑动\n";
			str += "[环顾]\t鼠标左键左右滑动\n";
			str += "[开车]\t鼠标左键前后滑动\n";
			str += "[上天]\t鼠标左右键向前滑动 or 鼠标按下+E\n";
			str += "[入地]\t鼠标左右键向后滑动 or 鼠标按下+Q\n";
			str += "[说明]\t可参考 unreal engine 4 基础操作\n";
			ui_guide->set_text(str);
			ui_guide->setWindowModality(Qt::WindowModal);
			connect(ui.action_guide, SIGNAL(triggered()), this, SLOT(trigger_guide()));
		}
		// init about widget
		{
			ui_about = new CTextWidget(this);
			ui_about->setVisible(false);
			ui_about->setMinimumSize(400, 500);
			ui_about->setWindowTitle(QString::fromUtf8("关于"));
			std::string str = "[作者]\tDua\n[时间]\t2019.03.30\n[清单]\t已支持的功能\n\t";
			str += "[图元生成]\n\t[资源导入]\n\t[事件分发]\n\t[数学库]\n\t[BlinnPhong直接光照]\n\t[拾取]\n\t";
			str += "[后期处理]\n\t[红蓝3D]\n\t[骨骼动画]\n\t[shadertoy]\n";
			str += "[说明]\t持续更新中, 有好的bug或者好的建议\n\t欢迎与本人联系, 共同学习, 共同进步。\n\t兴趣是最好的老师。\n";
			str += "[声明]\t水平有限, 仅供学习和参考, \n\t不得进行任何商业用途, \n\t否则产生的一切后果将由使用者本人承担\n";
			str += "[邮箱]\t270389972@qq.com\n";
			str += "[以上]\t\\(^o^)/\n";
			ui_about->set_text(str);
			ui_about->setWindowModality(Qt::WindowModal);
			connect(ui.action_about, SIGNAL(triggered()), this, SLOT(trigger_about()));
		}
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

void CMainWindow::trigger_normal_visual() { GameManager_ins().set_b_normal_visual(!GameManager_ins().get_b_normal_visual()); }
void CMainWindow::trigger_explode() { GameManager_ins().set_b_explode(!GameManager_ins().get_b_explode()); }

void CMainWindow::trigger_rb_3d() { 
	GameManager_ins().set_b_use_vr(!GameManager_ins().get_b_use_vr()); 
	if (GameManager_ins().get_b_use_vr()) { ui.menu_eye_delta->setEnabled(true); }
	else { ui.menu_eye_delta->setEnabled(false); }
}
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
	else if (act->objectName().compare("action_pp_real_rb3d") == 0) {
		GameManager_ins().set_pp_type(PostProcessType::REAL_RB);
	}
}

void CMainWindow::trigger_gamma() {
	GameManager_ins().set_b_gamma(!GameManager_ins().get_b_gamma());
	if (GameManager_ins().get_b_gamma()) { ui.menu_gamma_value->setEnabled(true); } // 如果打开了 gamma
	else { ui.menu_gamma_value->setEnabled(false); }

}
void CMainWindow::trigger_gamma_init() {
	ui_gamma_value->set_value(static_cast<int>(GameManager_ins().get_v_gamma() * 10 + 0.5f));
}
void CMainWindow::trigger_gamma_value(int v) {
	GameManager_ins().set_v_gamma(CMath_ins().linear_lerp(0.4f, 5.0f, v*0.1f));
}

void CMainWindow::trigger_msaa() { GameManager_ins().set_b_msaa(!GameManager_ins().get_b_msaa()); }

void CMainWindow::trigger_shader_toy() {
	GameManager_ins().set_b_use_shader_toy(!GameManager_ins().get_b_use_shader_toy());
}

void CMainWindow::trigger_guide() { ui_guide->setVisible(true); }
void CMainWindow::trigger_about() { ui_about->setVisible(true); }




