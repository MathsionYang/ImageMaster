#pragma once

#include "ui_ImageMaster_Pro.h"
#include "Header.h"
#include "Attributes.h"
#include "OurMenu.h"


#pragma execution_character_set("utf-8")    // ��������������⣬ע�⣡����

class ImageMaster_Pro : public QMainWindow
{
    Q_OBJECT

public:
    ImageMaster_Pro(QWidget *parent = Q_NULLPTR);

	void ShowImage(QImage Img, bool isSave);	// ��ʾͼ��
private:
    Ui::ImageMaster_ProClass ui;
	void Init();
	// ��ʼ��ͼ��
	void InitImage();     
	//�������
	void AppWinStyle();
	//Qimage תMat
	cv::Mat QImage2cvMat(QImage image);
	 // Mat �ĳ� QImage
	QImage cvMat2QImage(const cv::Mat& mat);

	OurMenu *I_menubar;							// �˵���
private slots:

	void Img_boxFilter();
	void Img_blur();
	void Img_GaussianBlur();
	void Img_editundo();
	void Img_editredo();
	void Img_Canny();
	void Img_Sobel();
	void Img_Laplacian();
	void Img_Full();
	void Img_Auto();

	void Img_RGB2GRAY();
	void Img_RGB2HSV();
	void Img_Gamma();
	void receiveData(QString data);
	void receiveflag(bool flag);
	void Img_Dilate();
	void Img_Erode();

	void Img_sift();
	void Img_surf();
	void Img_orb();

public:
	//�½����������ļ���
	QString Pro_PathName, current_Pro;
	QDateTime current_Pro_time;
	QString file_suffix;//ͼ���׺
	QToolBar *toolBar = new QToolBar();
	QDockWidget *dock_Image;                    // ͼ�񴰿�
	QLabel *imgLabel;                           // ͼ����ʾ��
	QScrollArea* scrollArea;
	//٤��ֵ
	double dgammavalue;
	bool isShowMatchDst;
	std::vector<std::vector<QImage>> imgVector;	// �洢ͼ���Vector����
	std::vector<QImage>::iterator iter;			// Vector������
};