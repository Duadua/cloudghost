#include "cspinwidget.h"
#include <QValidator>

CSpinWidget::CSpinWidget(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	dec = 0;
	precision = 1;
	ui.hedit->setValidator(new QIntValidator(0, 100, this));

	connect(ui.hslider, SIGNAL(valueChanged(int)), this, SLOT(value_changed_slider(int)));
	connect(ui.hedit, SIGNAL(editingFinished()), this, SLOT(value_changed_edit()));
	//connect(ui.spin, SIGNAL(valueChanged(int)), this, SIGNAL(value_changed(int)));
}
CSpinWidget::~CSpinWidget() { }

void CSpinWidget::set_precision(int p) { dec = p; precision = 1; while (p--) { precision *= 10; } }

void CSpinWidget::set_min(float m) {
	int t_m = static_cast<int>(m * precision);
	ui.hslider->setMinimum(t_m); 
	if (precision == 1) { 
		ui.hedit->setValidator(new QIntValidator(ui.hslider->minimum(), ui.hslider->maximum(), this)); 
	}
	else {
		ui.hedit->setValidator(new QDoubleValidator(m, 1.0*ui.hslider->maximum() / precision, dec, this));
	}
}
void CSpinWidget::set_max(float m) { 
	int t_m = static_cast<int>(m * precision);
	ui.hslider->setMaximum(t_m); 
	if (precision == 1) {
		ui.hedit->setValidator(new QIntValidator(ui.hslider->minimum(), ui.hslider->maximum(), this));
	}
	else {
		ui.hedit->setValidator(new QDoubleValidator(1.0*ui.hslider->maximum() / precision, m, dec, this));
	}
}
void CSpinWidget::set_range(float min, float max, int p) {
	set_precision(p);
	int t_min = static_cast<int>(min * precision + 0.5f);
	int t_max = static_cast<int>(max * precision + 0.5f);
	ui.hslider->setRange(t_min, t_max);
	if (precision == 1) {
		ui.hedit->setValidator(new QIntValidator(ui.hslider->minimum(), ui.hslider->maximum(), this));
	}
	else { ui.hedit->setValidator(new QDoubleValidator(min, max, dec, this)); }
}

void CSpinWidget::set_step(float s) { 
	int t_s = static_cast<int>(s * precision);
	ui.hslider->setSingleStep(t_s); 
}

void CSpinWidget::set_value(float v) { 
	int t_v = static_cast<int>(v * precision);
	ui.hslider->setValue(t_v); 
	ui.hedit->setText(QString::number(v, 'f', dec));
}

void CSpinWidget::value_changed_slider(int v) {
	float t_v = 1.0f * v / precision;
	ui.hedit->setText(QString::number(t_v, 'f', dec));
	emit value_changed(t_v);
}
void CSpinWidget::value_changed_edit() {
	if (ui.hedit->hasFocus()) {
		int t_v = static_cast<int>(ui.hedit->text().toFloat() * precision);
		if (t_v < ui.hslider->minimum()) t_v = ui.hslider->minimum();
		if (t_v > ui.hslider->maximum()) t_v = ui.hslider->maximum();
		ui.hslider->setValue(t_v);
	}
}
