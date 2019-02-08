#include "cpbr.h"
#include "cglmanager.h"

CPBR::CPBR(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	init_gl_view(50, 50, 800, 600);
}
CPBR::~CPBR() { delete gl_view; }

void CPBR::init_gl_view(int a, int b, int w, int h) {
	gl_view = new CGLManager(this);
	gl_view->setGeometry(a, b, w, h);
}
