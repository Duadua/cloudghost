#include "qtdebugwidget.h"
#include <QIcon>

QtDebugWidget::QtDebugWidget(QWidget *parent) : QWidget(parent) {

	ui.setupUi(this);
	setWindowIcon(QIcon(":/cloudghost/icon/ui/debug.ico"));
	move(100, 0);
	connect(ui.tedit_debug, SIGNAL(textChanged()), this, SLOT(tedit_changed()));
	setWindowFlags(Qt::WindowMinimizeButtonHint);
}

QtDebugWidget::~QtDebugWidget() { }

void QtDebugWidget::append_text(const std::string& str) {
	ui.tedit_debug->setPlainText(ui.tedit_debug->toPlainText() + QString::fromStdString(str));
}
void QtDebugWidget::tedit_changed() {
	ui.tedit_debug->moveCursor(QTextCursor::End);
}
