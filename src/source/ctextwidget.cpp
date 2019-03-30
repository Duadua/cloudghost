#include "ctextwidget.h"

CTextWidget::CTextWidget(QWidget *parent) : QWidget(parent) {

	ui.setupUi(this);
	setWindowFlags(Qt::Dialog);
	ui.text_edit->setFocusPolicy(Qt::NoFocus);
	connect(ui.text_edit, SIGNAL(textChanged()), this, SIGNAL(text_changed()));
}

CTextWidget::~CTextWidget() { }

void CTextWidget::set_editable(bool b) {
	if (b) { ui.text_edit->setFocusPolicy(Qt::StrongFocus); }
	else { ui.text_edit->setFocusPolicy(Qt::NoFocus); }
}

void CTextWidget::set_text(const std::string& str) { ui.text_edit->setPlainText(QString::fromStdString(str)); }
void CTextWidget::append_text(const std::string& str) { ui.text_edit->setPlainText(ui.text_edit->toPlainText() + QString::fromStdString(str)); }
void CTextWidget::clear_text() { ui.text_edit->clear(); }
