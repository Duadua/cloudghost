#pragma once

#include "ui_ctextwidget.h"
#include <QWidget>
#include <string>

class CTextWidget : public QWidget {
	Q_OBJECT

public:
	CTextWidget(QWidget *parent = Q_NULLPTR);
	~CTextWidget();

	void set_editable(bool b);

	void set_text(const std::string& str);
	void append_text(const std::string& str);
	void clear_text();

protected:
	Ui::CTextWidget ui;

signals:
	void text_changed();
};
