#include "cpbr.h"
#include "cglwidget.h"
#include "gamemanager.h"
#include <QDebug>

CPBR::CPBR(QWidget *parent) : QMainWindow(parent) {
	// init ui
	{
		ui.setupUi(this);
		connect(ui.menu_shading, SIGNAL(triggered(QAction*)), this, SLOT(trigger_menu_shading(QAction*)));
	}

	init_gl_view(geometry().top() + 50, geometry().left() + 50, geometry().width() - 100, geometry().height() -100);
}
CPBR::~CPBR() { delete gl_view; }

void CPBR::init_gl_view(int a, int b, int w, int h) {
	gl_view = new CGLWidget(this);
	gl_view->setGeometry(a, b, w, h);
}

void CPBR::resizeEvent(QResizeEvent *event) {
	if (gl_view == nullptr) return;
	gl_view->resize( geometry().width() - 100, geometry().height() - 100);
}

void CPBR::trigger_menu_shading(QAction* action) {
	// toggle use red_blue_3d
	if (action->objectName().compare("red_blue_3d") == 0) {
		GameManager::get_instance()->set_b_use_vr(!GameManager::get_instance()->get_b_use_vr());
	}
}

