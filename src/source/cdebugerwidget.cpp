#include "cdebugerwidget.h"
#include <QIcon>

CDebugerWidget::CDebugerWidget(QWidget *parent) : CTextWidget(parent) {

	setWindowTitle(QString::fromUtf8("c_debuger [dua 专用]"));
	setWindowIcon(QIcon(":/cloudghost/icon/ui/debug.ico"));
	setWindowFlags(Qt::WindowMinimizeButtonHint);
	setGeometry(0, 0, 600, 300);
	move(100, 0);
	connect(this, SIGNAL(text_changed()), this, SLOT(tedit_changed()));
}

void CDebugerWidget::tedit_changed() { ui.text_edit->moveCursor(QTextCursor::End); } 


