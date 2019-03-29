#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cloudghost.h"

class CGLWidget;
class QResizeEvent;
class QColorDialog;

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
	void init_ui();

public slots:
	void trigger_bg_color();
	void trigger_bd_color();
	void trigger_rb_3d();
	void trigger_polygon_mode(QAction* act);
	void trigger_select_pp(QAction* act);

	void trigger_shader_toy();

};
