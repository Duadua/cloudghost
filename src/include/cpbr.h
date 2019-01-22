#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_cpbr.h"

class CPBR : public QMainWindow
{
	Q_OBJECT

public:
	CPBR(QWidget *parent = Q_NULLPTR);

private:
	Ui::CPBRClass ui;
};
