#pragma once

#include <QWidget>
#include "ui_csubmitwidget.h"

class QSpacerItem;

class CSubmitWidget : public QWidget {
	Q_OBJECT

public:
	CSubmitWidget(QWidget *parent = Q_NULLPTR);
	~CSubmitWidget();

	void add_hspacer(int w = 40, int h = 20);
	void rem_hspacer();

private:
	Ui::CSubmitWidget ui;
	QSpacerItem* hspacer;
	QSpacerItem* vspacer;

	bool has_hspacer;
	bool has_vspacer;

signals:
	void ok();
	void cancel();
};
