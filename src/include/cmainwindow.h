#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cloudghost.h"

class CGLWidget;
class QResizeEvent;
class QColorDialog;
class CSpinWidget;
class CTextWidget;
class QActionGroup;

class CMainWindow : public QMainWindow {
	Q_OBJECT

public:
	CMainWindow(QWidget *parent = Q_NULLPTR);
    ~CMainWindow() override;

protected:
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;

private:
	void init_gl_view(int a, int b, int w, int h);
	CGLWidget* gl_view;

private:
	Ui::CloudGhostClass ui;

	QColorDialog* ui_bg_color;
	int ui_bg_color_custom_cnt;
	QColorDialog* ui_bd_color;
	int ui_bd_color_custom_cnt;

	QActionGroup* ui_skybox_ag;

	CSpinWidget* ui_eye_delta;

	CSpinWidget* ui_gamma_value;

	CTextWidget* ui_about;
	CTextWidget* ui_guide;

	CSpinWidget* ui_hdr_exposure;

	void init_ui();

public slots:
	void trigger_rb_3d();
	void trigger_eye_delta_init();
	void trigger_eye_delta(float v);
	void trigger_polygon_mode(QAction* act);
	void trigger_view_depth();
	void trigger_view_light_mode(QAction* act);

	void trigger_bg_color_init();
	void trigger_bg_color_ok();
	void trigger_bg_color_cancel();
	void trigger_bd_color_init();
	void trigger_bd_color_ok();

	void trigger_skybox_init();
	void trigger_skybox(QAction* act);

	void trigger_shadow();
	void trigger_normalmap();

	void trigger_normal_visual();
	void trigger_explode();

	void trigger_select_pp(QAction* act);

	void trigger_gamma();
	void trigger_gamma_init();
	void trigger_gamma_value(float v);

	void trigger_msaa();

	void trigger_shader_toy();

	void trigger_guide();
	void trigger_about();

	void trigger_select_hdr(QAction* act);
	void trigger_hdr_exposure_init();
	void trigger_hdr_exposure_value(float v);

	void trigger_rt_pbr();
};
