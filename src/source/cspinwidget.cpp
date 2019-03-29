#include "cspinwidget.h"

CSpinWidget::CSpinWidget(QWidget *parent) : QWidget(parent) {

	ui.setupUi(this);

	connect(ui.spin, SIGNAL(valueChanged(int)), ui.hslider, SLOT(setValue(int)));
	connect(ui.hslider, SIGNAL(valueChanged(int)), ui.spin, SLOT(setValue(int)));
	connect(ui.spin, SIGNAL(valueChanged(int)), this, SIGNAL(value_changed(int)));
}

void CSpinWidget::set_value(int v) { ui.spin->setValue(v); }
void CSpinWidget::set_min(int m) { ui.spin->setMinimum(m);  ui.hslider->setMinimum(m); }
void CSpinWidget::set_max(int m) { ui.spin->setMaximum(m);  ui.hslider->setMaximum(m); }
void CSpinWidget::set_step(int s) { ui.spin->setSingleStep(s);  ui.hslider->setSingleStep(s); }

CSpinWidget::~CSpinWidget() { }
