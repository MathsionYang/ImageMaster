#pragma once

#include <QWidget>
#include "ui_Attributes.h"
#include "Header.h"
class Attributes : public QWidget
{
	Q_OBJECT
private:
	Ui::Attributes *ui;

public:
	Attributes(QWidget *parent = Q_NULLPTR);
	~Attributes();
	void myslider();

	QLineEdit *lineEdit;
	QSlider *slider;
signals:
	//´«µÝgammaÖµ
	void sendGammaValue(QString);
public slots:
	void setLineEditValue(int value);


};
