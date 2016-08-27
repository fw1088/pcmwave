#include "drawpcm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	drawPcm w;
	w.show();
	return a.exec();
}
