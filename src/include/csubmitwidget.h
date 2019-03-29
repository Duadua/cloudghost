#pragma once

#include <QWidget>
#include "ui_csubmitwidget.h"

class CSubmitWidget : public QWidget {
	Q_OBJECT

public:
	CSubmitWidget(QWidget *parent = Q_NULLPTR);
	~CSubmitWidget();

private:
	Ui::CSubmitWidget ui;
};
