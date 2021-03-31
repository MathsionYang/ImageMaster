#pragma once

#include "ui_ImageMaster_Pro.h"
#include "Header.h"
#include "Attributes.h"
#include "OurMenu.h"


#pragma execution_character_set("utf-8")    // 解决汉字乱码问题，注意！！！

class ImageMaster_Pro : public QMainWindow
{
    Q_OBJECT

public:
    ImageMaster_Pro(QWidget *parent = Q_NULLPTR);

	void ShowImage(QImage Img, bool isSave);	// 显示图像
private:
    Ui::ImageMaster_ProClass ui;
	void Init();
	// 初始化图像
	void InitImage();     
	//程序界面
	void AppWinStyle();
	//Qimage 转Mat
	cv::Mat QImage2cvMat(QImage image);
	 // Mat 改成 QImage
	QImage cvMat2QImage(const cv::Mat& mat);

	OurMenu *I_menubar;							// 菜单栏
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
	//新建工作区间文件夹
	QString Pro_PathName, current_Pro;
	QDateTime current_Pro_time;
	QString file_suffix;//图像后缀
	QToolBar *toolBar = new QToolBar();
	QDockWidget *dock_Image;                    // 图像窗口
	QLabel *imgLabel;                           // 图像显示框
	QScrollArea* scrollArea;
	//伽马值
	double dgammavalue;
	bool isShowMatchDst;
	std::vector<std::vector<QImage>> imgVector;	// 存储图像的Vector容器
	std::vector<QImage>::iterator iter;			// Vector迭代器
};