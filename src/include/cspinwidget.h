#pragma once

#include <QWidget>
#include "ui_cspinwidget.h"

class CSpinWidget : public QWidget {
	Q_OBJECT

public:
	CSpinWidget(QWidget *parent = Q_NULLPTR);
	~CSpinWidget();

	void set_value(int v);
	void set_min(int m);
	void set_max(int m);
	void set_step(int s);

private:
	Ui::CSpinWidget ui;

signals:
	void value_changed(int v);
};




