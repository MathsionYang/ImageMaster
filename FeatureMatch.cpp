#include "FeatureMatch.h"



#pragma execution_character_set("utf-8")
FeatureMatch::FeatureMatch(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	resize(450, 300);
	LayoutView();
	
}

FeatureMatch::~FeatureMatch()
{
}
void FeatureMatch::LayoutView()
{
	setWindowTitle("ͼ������ƥ��");
	setWindowIcon(QIcon("logo/logo.png"));
	imgLabel = new QLabel("one");
	imgLabe2 = new QLabel("two");
	button3 = new QPushButton("�ο�Ӱ��");
	button4 = new QPushButton("��ƥ��Ӱ��");
	button5 = new QPushButton("ִ��");
	layout = new QGridLayout;
	layout->addWidget(imgLabel, 0, 0);
	layout->addWidget(imgLabe2, 0, 1);
	layout->addWidget(button3, 1, 0);
	layout->addWidget(button4, 1, 1);
	layout->addWidget(button5, 2, 0,2,2);
	QImage image2 = QImage(220, 180, QImage::Format_RGB32);  // �½�ͼ��
	image2.fill(qRgb(255, 255, 255));                        // ȫ��
	imgLabel->setPixmap(QPixmap::fromImage(image2));         // ��ʾͼ��
	imgLabel->resize(image2.width(), image2.height());        // ͼ����imgLabelͬ��С
	image2.fill(qRgb(255, 255, 255));                        // ȫ��
	imgLabe2->setPixmap(QPixmap::fromImage(image2));         // ��ʾͼ��
	imgLabe2->resize(image2.width(), image2.height());        // ͼ����imgLabelͬ��С
	this->setLayout(layout);
	connect(button3, SIGNAL(clicked()), this, SLOT(OpenImg1()));    // �ź����
	connect(button4, SIGNAL(clicked()), this, SLOT(OpenImg2()));
	connect(button5, SIGNAL(clicked()), this, SLOT(SendQimage()));
}
void FeatureMatch::OpenImg1()
{
	
	 img1 = OpenImgPath(imgLabel, image1);
}
void FeatureMatch::OpenImg2()
{
	img2 = OpenImgPath(imgLabe2,image2);
}
void FeatureMatch::SendQimage()
{
	
	std::vector<cv::KeyPoint> kp1, kp2;
	std::vector<cv::DMatch>  matches_all;
	cv::Mat _desc1, _desc2;
	cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::SIFT::create();
	detector->detect(image1, kp1);
	detector->detect(image2, kp2);
	detector->compute(image1, kp1, _desc1);
	detector->compute(image2, kp2, _desc2);
	cv::FlannBasedMatcher matcherByFlann;
	std::vector<std::vector<cv::DMatch>>matches_Pt;
	std::vector<cv::Mat>train_desc(1, _desc2);
	matcherByFlann.add(train_desc);
	matcherByFlann.train();
	matcherByFlann.knnMatch(_desc1, matches_Pt, 2);
	//matcher.match(descript1, descript2, matches_1_2);
	for (int matches_Pt_index = 0; matches_Pt_index < matches_Pt.size(); matches_Pt_index++)
	{
		if (matches_Pt[matches_Pt_index][0].distance < 0.45*matches_Pt[matches_Pt_index][1].distance)
		{
			matches_all.push_back(matches_Pt[matches_Pt_index][0]);
		}
	}
	cv::Mat img_match;
	cv::drawMatches(image1, kp1, image2, kp2, matches_all, img_match,cv::Scalar(0,255,0),cv::Scalar::all(-1));
	cv::imwrite("result//siftMatch.png", img_match);
	bool flag = true;
	emit sendflag(flag);
	this->close();
	/*QImage matchImg = cvMat2QImage(img_match);
	ImageMaster_Pro *p = new ImageMaster_Pro();
	p->ShowImage(matchImg,true);*/
}
QImage FeatureMatch::OpenImgPath(QLabel *imgLabe,cv::Mat &srcimg)
{
	QImage* img = new QImage();
	QString path = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), ".", tr("Images(*.jpg *.png *.bmp)"));
	if (!path.isEmpty())
	{
		
		if (!(img->load(path)))
		{
			QMessageBox::information(this, tr("����"), tr("��ͼ��ʧ�ܣ�"));
			delete img;
		}
		cv::Mat img2 = QImage2cvMat(*img);
		srcimg = cv::imread(path.toStdString());
		cv::Mat img2_1 = img2.clone();
		cv::Mat res;
		cv::resize(img2_1, res, cv::Size(220, 180), 0, 0, cv::INTER_LINEAR);
		QImage outimg = cvMat2QImage(res);
		imgLabe->setPixmap(QPixmap::fromImage(outimg));
		imgLabe->resize(outimg.width(), outimg.height());
		return outimg;
	}
}
cv::Mat FeatureMatch::QImage2cvMat(QImage image)			// QImage�ĳ�Mat
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}
QImage FeatureMatch::cvMat2QImage(const cv::Mat& mat)    // Mat �ĳ� QImage
{
	if (mat.type() == CV_8UC1)					// ��ͨ��
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		image.setColorCount(256);				// �Ҷȼ���256
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		uchar *pSrc = mat.data;					// ����mat����
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}

	else if (mat.type() == CV_8UC3)				// 3ͨ��
	{
		const uchar *pSrc = (const uchar*)mat.data;			// ��������
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);	// R, G, B ��Ӧ 0,1,2
		return image.rgbSwapped();				// rgbSwapped��Ϊ����ʾЧ��ɫ�ʺ�һЩ��
	}
	else if (mat.type() == CV_8UC4)
	{
		const uchar *pSrc = (const uchar*)mat.data;			// ��������
															// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);		// B,G,R,A ��Ӧ 0,1,2,3
		return image.copy();
	}
	else
	{
		return QImage();
	}
}
