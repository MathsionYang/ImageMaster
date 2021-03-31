#pragma once

#include <QWidget>
#include "ui_FeatureMatch.h"
#include "Header.h"
#include "ImageMaster_Pro.h"
class FeatureMatch : public QWidget
{
	Q_OBJECT

public:
	FeatureMatch(QWidget *parent = Q_NULLPTR);
	~FeatureMatch();
	void LayoutView();
	cv::Mat QImage2cvMat(QImage image);
	// Mat 改成 QImage
	QImage cvMat2QImage(const cv::Mat& mat);
	QImage OpenImgPath(QLabel *imgLabe, cv::Mat  &srcimg);

	QLabel *imgLabel, *imgLabe2;
	QGridLayout *layout;
	QPushButton *button3, *button4, *button5;
	QImage img1,img2;
	//原始影像
	cv::Mat image1,image2;
signals:
	void sendflag(bool);
private:
	Ui::FeatureMatch ui;
private slots:
	void OpenImg1();
	void OpenImg2();
	void SendQimage();

};
