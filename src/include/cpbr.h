#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cpbr.h"

class CGLWidget;
class QResizeEvent;

class CPBR : public QMainWindow {
	Q_OBJECT

public:
	CPBR(QWidget *parent = Q_NULLPTR);
	~CPBR();

protected:
	virtual void resizeEvent(QResizeEvent *event) override;

private:
	void init_gl_view(int a, int b, int w, int h);
	CGLWidget* gl_view;

private:
	Ui::CPBRClass ui;
	void init_ui();

public slots:
	void trigger_rb_3d();
	void trigger_polygon_mode(QAction* act);
	void trigger_select_pp(QAction* act);

	void trigger_shader_toy();


};
