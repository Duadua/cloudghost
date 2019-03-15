#pragma once

#include <QWidget>
#include "ui_qtdebugwidget.h"
#include <string>

class QtDebugWidget : public QWidget
{
	Q_OBJECT

public:
	QtDebugWidget(QWidget *parent = Q_NULLPTR);
	~QtDebugWidget();

	void append_text(const std::string& str);
	
private:
	Ui::QtDebugWidget ui;

public slots:
	void tedit_changed();
};
