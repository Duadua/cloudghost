#pragma once

#include <QWidget>
#include "ui_qtdebugwidget.h"

class QtDebugWidget : public QWidget
{
	Q_OBJECT

public:
	QtDebugWidget(QWidget *parent = Q_NULLPTR);
	~QtDebugWidget();

private:
	Ui::QtDebugWidget ui;
};
