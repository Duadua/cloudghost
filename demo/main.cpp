#include "cpbr.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CPBR w;
	w.show();
	return a.exec();
}
