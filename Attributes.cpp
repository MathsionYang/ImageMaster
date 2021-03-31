#include "Attributes.h"

Attributes::Attributes(QWidget *parent)
	: QWidget(parent)
{
	ui->setupUi(this);
	setWindowTitle("gamma值");
	// 软件图标
	setWindowIcon(QIcon("logo/logo.png"));
	myslider();
}

Attributes::~Attributes()
{
	delete ui;
}
void Attributes::myslider()
{
	lineEdit = new QLineEdit("0");

	//新建一个水平方向的滑动条QSlider控件
	slider = new QSlider(Qt::Horizontal);
	//设置滑动条控件的最小值
	slider->setMinimum(0);
	//设置滑动条控件的最大值
	slider->setMaximum(5);
	//设置滑动条控件的值
	slider->setValue(1);

	//信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue(int)));

	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(lineEdit);
	layout->addWidget(slider);

	this->setLayout(layout);
	this->resize(200, 60);
}
void Attributes::setLineEditValue(int value)
{
	int pos = slider->value();

	QString str = QString("%1").arg(pos);
	lineEdit->setText(str);
	emit sendGammaValue(str);  //获取lineEdit的输入并且传递出去
}
