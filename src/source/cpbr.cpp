#include "cpbr.h"
#include "cglwidget.h"

CPBR::CPBR(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

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
