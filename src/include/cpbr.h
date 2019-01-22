#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cpbr.h"

class CGLManager;

class CPBR : public QMainWindow {
	Q_OBJECT

public:
	CPBR(QWidget *parent = Q_NULLPTR);

private:
	void init_gl_view(int a, int b, int w, int h);
	CGLManager* gl_view;

private:
	Ui::CPBRClass ui;
};
