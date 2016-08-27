#ifndef DRAWPCM_H
#define DRAWPCM_H

#include <QtWidgets/QMainWindow>
#include <QPaintEvent>
#include <QPushButton>
#include <stdint.h>
#include <QProcess>
#include <QDir>
#include <QImage>
class drawPcm : public QMainWindow
{
	Q_OBJECT
public:
	drawPcm(QWidget *parent = 0);
	~drawPcm();
	void paintEvent(QPaintEvent* e);
public slots:
	void handleClick();
	void onFinished(int code, QProcess::ExitStatus status);  
private:
	int maxValue;
	int minValue;
	int length;
	QPushButton *btn;
	short *ptrtest;
	float *fdst;
	int mduration;
};

#endif // DRAWPCM_H
