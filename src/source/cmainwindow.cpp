#include "cmainwindow.h"
#include "cglwidget.h"
#include "cspinwidget.h"
#include "csubmitwidget.h"
#include "gamemanager.h"
#include "assetmanager.h"
#include "ctextwidget.h"
#include "texture3d.h"

#include <QColorDialog>
#include <QActionGroup>
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

	// init view menu
	{
		// polygon_mode 
		{
			auto ag = new QActionGroup(this);
			ag->addAction(ui.action_pm_fill);
			ag->addAction(ui.action_pm_line);
			ag->addAction(ui.action_pm_point);
			connect(ag, SIGNAL(triggered(QAction*)), this, SLOT(trigger_polygon_mode(QAction*)));
		}

		// red_blue_3d
		{
			connect(ui.action_rb_3d, SIGNAL(triggered()), this, SLOT(trigger_rb_3d()));

			// 生成菜单栏 spinbox
			ui_eye_delta = new CSpinWidget(this);
			ui_eye_delta->set_range(0.1f, 100.0f, 2);
			connect(ui_eye_delta, SIGNAL(value_changed(float)), this, SLOT(trigger_eye_delta(float)));

			QWidgetAction *ui_action_eye_delta = new QWidgetAction(this);
			ui_action_eye_delta->setDefaultWidget(ui_eye_delta);
			ui.menu_eye_delta->addAction(ui_action_eye_delta);
			connect(ui.menu_eye_delta, SIGNAL(aboutToShow()), this, SLOT(trigger_eye_delta_init()));
		}
		
		// depth view
		{
			connect(ui.action_view_depth, SIGNAL(triggered()), this, SLOT(trigger_view_depth()));
		}
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

		// skybox 
		{
			connect(ui.menu_skybox, SIGNAL(aboutToShow()), this, SLOT(trigger_skybox_init()));
			ui_skybox_ag = new QActionGroup(this);
			ui_skybox_ag->addAction(ui.action_skybox_nope);
			connect(ui_skybox_ag, SIGNAL(triggered(QAction*)), this, SLOT(trigger_skybox(QAction*)));

		}

		// normal visual
		{
			connect(ui.action_normal_visual, SIGNAL(triggered()), this, SLOT(trigger_normal_visual()));
		}
		// explode
		{
			connect(ui.action_explode, SIGNAL(triggered()), this, SLOT(trigger_explode()));
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
			ui_gamma_value->set_range(1.0f, 5.0f, 1);
			connect(ui_gamma_value, SIGNAL(value_changed(float)), this, SLOT(trigger_gamma_value(float)));

			QWidgetAction *ui_action_gamma = new QWidgetAction(this);
			ui_action_gamma->setDefaultWidget(ui_gamma_value);
			ui.menu_gamma_value->addAction(ui_action_gamma);
			connect(ui.menu_gamma_value, SIGNAL(aboutToShow()), this, SLOT(trigger_gamma_init()));
		}
		
		// msaa
		{
			connect(ui.action_msaa, SIGNAL(triggered()), this, SLOT(trigger_msaa()));
		}

		// hdr
		{
			auto ag = new QActionGroup(this);
			ag->addAction(ui.action_hdr_nope);
			ag->addAction(ui.action_hdr_reinhard);
			ag->addAction(ui.action_hdr_exposure);
			ui.action_hdr_reinhard->setChecked(true);
			connect(ag, SIGNAL(triggered(QAction*)), this, SLOT(trigger_select_hdr(QAction*)));

			ui_hdr_exposure = new CSpinWidget(this);
			ui_hdr_exposure->set_range(0.1f, 5.0f, 1);
			connect(ui_hdr_exposure, SIGNAL(value_changed(float)), this, SLOT(trigger_hdr_exposure_value(float)));

			QWidgetAction *ui_action_hdr_exposure = new QWidgetAction(this);
			ui_action_hdr_exposure->setDefaultWidget(ui_hdr_exposure);
			ui.menu_hdr_exposure->addAction(ui_action_hdr_exposure);
			connect(ui.menu_hdr_exposure, SIGNAL(aboutToShow()), this, SLOT(trigger_hdr_exposure_init()));
			
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
			str += "[后期处理]\n\t[红蓝3D]\n\t[骨骼动画]\n\t[shadertoy]\n\t";
			str += "[法线可视化]\n\t[抗锯齿(MSAA)]\n\t[gamma校正]\n\t[HDR(tonemap)]\n\t";
			str += "[天空盒]\n\t[深度图]\n\t[阴影(shadowmap)]\n";
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

void CMainWindow::trigger_rb_3d() { 
	GameManager_ins().set_b_use_vr(!GameManager_ins().get_b_use_vr()); 
	if (GameManager_ins().get_b_use_vr()) { ui.menu_eye_delta->setEnabled(true); }
	else { ui.menu_eye_delta->setEnabled(false); }
}
void CMainWindow::trigger_eye_delta_init() { ui_eye_delta->set_value(GameManager_ins().get_vr_delta()); }
void CMainWindow::trigger_eye_delta(float v) { GameManager_ins().set_vr_delta(v); }

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

void CMainWindow::trigger_view_depth() { GameManager_ins().set_b_depth(!GameManager_ins().get_b_depth()); }

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

void CMainWindow::trigger_skybox_init() {
	static bool flag = true;
	if (flag) {
		flag = false;
		for (auto t3d : AssetManager_ins().map_texture3Ds) {
			auto t_name = t3d.first;
			auto t_a = new QAction(QString::fromStdString(t_name), this);
			t_a->setObjectName(QString::fromStdString(t_name));
			t_a->setCheckable(true);
			if (t_name.compare(GameManager_ins().get_skybox()->get_name()) == 0) { t_a->setChecked(true); }
			ui_skybox_ag->addAction(t_a);
			ui.menu_skybox->addAction(t_a);
		}
	}
}
void CMainWindow::trigger_skybox(QAction* act) {
	if (act->objectName().compare("action_skybox_nope") == 0) { GameManager_ins().set_b_skybox(false); }
	else {
		auto t_tex = AssetManager_ins().get_texture3D(act->objectName().toStdString());
		if (t_tex) { GameManager_ins().set_b_skybox(true); GameManager_ins().set_skybox(t_tex); }
	}
}

void CMainWindow::trigger_normal_visual() { GameManager_ins().set_b_normal_visual(!GameManager_ins().get_b_normal_visual()); }
void CMainWindow::trigger_explode() { GameManager_ins().set_b_explode(!GameManager_ins().get_b_explode()); }
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
void CMainWindow::trigger_gamma_init() { ui_gamma_value->set_value(GameManager_ins().get_v_gamma()); }
void CMainWindow::trigger_gamma_value(float v) { GameManager_ins().set_v_gamma(v); }

void CMainWindow::trigger_msaa() { GameManager_ins().set_b_msaa(!GameManager_ins().get_b_msaa()); }

void CMainWindow::trigger_shader_toy() {
	GameManager_ins().set_b_use_shader_toy(!GameManager_ins().get_b_use_shader_toy());
}

void CMainWindow::trigger_guide() { ui_guide->setVisible(true); }
void CMainWindow::trigger_about() { ui_about->setVisible(true); }

void CMainWindow::trigger_select_hdr(QAction* act) {
	if (act->objectName().compare("action_hdr_nope") == 0) {
		GameManager_ins().set_b_hdr(false);
		ui.menu_hdr_exposure->setEnabled(false);

	}
	else if (act->objectName().compare("action_hdr_reinhard") == 0) {
		GameManager_ins().set_b_hdr(true);
		GameManager_ins().set_hdr_type(HDR_Type::REINHARD);
		ui.menu_hdr_exposure->setEnabled(false);

	}
	else if (act->objectName().compare("action_hdr_exposure") == 0) {
		GameManager_ins().set_b_hdr(true);
		GameManager_ins().set_hdr_type(HDR_Type::EXPOSURE);
		ui.menu_hdr_exposure->setEnabled(true);
	}
}
void CMainWindow::trigger_hdr_exposure_init() { ui_hdr_exposure->set_value(GameManager_ins().get_hdr_exposure()); }
void CMainWindow::trigger_hdr_exposure_value(float v) { GameManager_ins().set_hdr_exposure(v); }




