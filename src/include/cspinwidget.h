#pragma once

#include <QWidget>
#include "ui_cspinwidget.h"

class CSpinWidget : public QWidget {
	Q_OBJECT

public:
	CSpinWidget(QWidget *parent = Q_NULLPTR);
	~CSpinWidget();

	void set_precision(int p);

	void set_min(float m);
	void set_max(float m);
	void set_range(float min, float max, int p = 0);
	void set_step(float s);

	void set_value(float v);

public slots:
	void value_changed_slider(int);
	void value_changed_edit();

private:
	Ui::CSpinWidget ui;

	int precision;					// 10^精度
	int dec;						// 精度

signals:
	void value_changed(float v);
};




