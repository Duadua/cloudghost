#pragma once

#include "ctextwidget.h"
#include <string>

class CDebugerWidget : public CTextWidget {
	Q_OBJECT

public:
	CDebugerWidget(QWidget *parent = Q_NULLPTR);
	~CDebugerWidget() {}

public slots:
	void tedit_changed();
};

