#include "csubmitwidget.h"
#include <QtWidgets/QSpacerItem>

CSubmitWidget::CSubmitWidget(QWidget *parent) : QWidget(parent) {
	has_hspacer = false; hspacer = nullptr;
	has_vspacer = false; vspacer = nullptr;

	ui.setupUi(this);

	connect(ui.button_ok, SIGNAL(clicked()), this, SIGNAL(ok()));
	connect(ui.button_cancel, SIGNAL(clicked()), this, SIGNAL(cancel()));
}

CSubmitWidget::~CSubmitWidget() { }

void CSubmitWidget::add_hspacer(int w, int h) {
	if (has_hspacer) return;
	has_hspacer = true;
	hspacer = new QSpacerItem(w, h, QSizePolicy::Expanding, QSizePolicy::Minimum);
	ui.horizontalLayout->insertItem(0, hspacer);
}
void CSubmitWidget::rem_hspacer() {
	if (!has_hspacer) return;
	has_hspacer = false;
	ui.horizontalLayout->removeItem(hspacer);
}

