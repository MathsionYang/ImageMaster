#include "Attributes.h"

Attributes::Attributes(QWidget *parent)
	: QWidget(parent)
{
	ui->setupUi(this);
	setWindowTitle("gammaֵ");
	// ���ͼ��
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

	//�½�һ��ˮƽ����Ļ�����QSlider�ؼ�
	slider = new QSlider(Qt::Horizontal);
	//���û������ؼ�����Сֵ
	slider->setMinimum(0);
	//���û������ؼ������ֵ
	slider->setMaximum(5);
	//���û������ؼ���ֵ
	slider->setValue(1);

	//�źźͲ� ����������ֵ�����ı�ʱ��������һ��valueChanged(int)�ź� ����QLineEdit�ؼ�����ʾ�ı�
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
	emit sendGammaValue(str);  //��ȡlineEdit�����벢�Ҵ��ݳ�ȥ
}
