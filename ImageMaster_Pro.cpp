
#include "ImageMaster_Pro.h"
#include "Attributes.h"
#include <QTextStream>
#include "FeatureMatch.h"

ImageMaster_Pro::ImageMaster_Pro(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	Init();
	InitImage();
	
}
void ImageMaster_Pro::Init()
{
	imgVector.resize(2);
	AppWinStyle();
	I_menubar = new OurMenu(this, this);
	this->setMenuBar(I_menubar);
}
void ImageMaster_Pro::InitImage()        // ��ʼ��ͼ��
{
	// ��ʼ��QDockWidget.���Ժ�ὲ�����ǿ��ƶ����ص�С���ڣ�
	// ����ʵ��PS��VSͣ�����ڵ�Ч����Ŀǰֻ���˽�
	dock_Image = new QDockWidget(tr("ͼ��"), this);          //   ͼ��
	setCentralWidget(dock_Image);

	// ��ʼ��QLabel
	imgLabel = new QLabel(dock_Image);
	imgLabel->setScaledContents(true);  // ����QLabel�Զ���Ӧͼ���С

	// ��ʼͼ��
	QImage image = QImage(800, 650, QImage::Format_RGB32);  // �½�ͼ��
	image.fill(qRgb(255, 255, 255));                        // ȫ��
	imgLabel->setPixmap(QPixmap::fromImage(image));         // ��ʾͼ��
	imgLabel->resize(image.width(), image.height());        // ͼ����imgLabelͬ��С

	imgVector[0].push_back(image);
	iter = imgVector[0].end() - 1;						// ���һ��Ԫ�ض�Ӧ�ĵ�����ָ��
	// ���ӹ�����,���ͼ���imgLabel�󣬾ͻ���ֹ�����
	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setAlignment(Qt::AlignCenter);
	scrollArea->setWidget(imgLabel);
	dock_Image->setWidget(scrollArea);
}

void ImageMaster_Pro::AppWinStyle()
{
	// �����С
	//resize(QApplication::desktop()->width()*0.5, QApplication::desktop()->height()*0.5);
	//move(QApplication::desktop()->width()*0.05, QApplication::desktop()->height()*0.01);
	resize(900, 800);
	setWindowTitle("ͼ�����ʦ");
	// ���ͼ��
	setWindowIcon(QIcon("logo/logo.png"));
	
}
cv::Mat ImageMaster_Pro::QImage2cvMat(QImage image)			// QImage�ĳ�Mat
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
QImage ImageMaster_Pro::cvMat2QImage(const cv::Mat& mat)    // Mat �ĳ� QImage
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

void ImageMaster_Pro::ShowImage(QImage Img, bool isSave)
{
	imgLabel->setPixmap(QPixmap::fromImage(Img));
	imgLabel->resize(Img.width(), Img.height());

	if (isSave == true)
	{
		if (iter != (imgVector[0].end() - 1))
		{
			imgVector[0].erase(++iter, imgVector[0].end());
		}

		imgVector[0].push_back(Img);
		iter = imgVector[0].end() - 1;
		
	}
}
//�����˲�
void ImageMaster_Pro::Img_boxFilter()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	boxFilter(img, out, -1, cv::Size(5, 5));//-1ָԭͼ���
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg,true);
}
//��ֵ�˲�
void ImageMaster_Pro::Img_blur()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	blur(img, out, cv::Size(5, 5));//-1ָԭͼ���
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
//��˹�˲�
void ImageMaster_Pro::Img_GaussianBlur()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	GaussianBlur(img, out, cv::Size(5, 5), 0, 0);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);

}
void ImageMaster_Pro::Img_Dilate()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	//��ȡ�Զ����
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	//���Ͳ���
	cv::Mat out;
	dilate(img, out, element);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
void ImageMaster_Pro::Img_Erode()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	//��ȡ�Զ����
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)); //��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	//���Ͳ���
	cv::Mat out;
	erode(img, out, element);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);

}
//��������
void ImageMaster_Pro::Img_editundo()
{
	if (iter != imgVector[0].begin())			// ǰ�滹�ж���
	{
		iter--;
		ShowImage(*iter, false);
	}
}
void ImageMaster_Pro::Img_editredo()
{
	if (iter != (imgVector[0].end() - 1))		// ���滹�ж���
	{
		iter++;
		ShowImage(*iter, false);
	}
}
//Canny����
void ImageMaster_Pro::Img_Canny()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat DstPic, edge, grayImage;
	//������srcͬ���ͺ�ͬ��С�ľ���
	DstPic.create(img.size(), img.type());
	//��ԭʼͼת��Ϊ�Ҷ�ͼ
	cvtColor(img, grayImage, CV_BGR2GRAY);
	//��ʹ��3*3�ں�������
	blur(grayImage, edge, cv::Size(3, 3));
	//����canny����
	Canny(edge, edge, 3, 9, 3);

	QImage outimg = cvMat2QImage(edge);
	ShowImage(outimg, true);
}
//Sobel����
void ImageMaster_Pro::Img_Sobel()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat  gray_scrimage, grad_x, grad_y,out;
	cvtColor(img, gray_scrimage, CV_BGR2GRAY);   //ת��Ϊ�Ҷ�ͼ
	Sobel(gray_scrimage, grad_x, -1, 1, 0); // x�����������������ȡ0,1,2
	Sobel(gray_scrimage, grad_y, -1, 0, 1); // y�������������
	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, out);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
//Laplacian����
void ImageMaster_Pro::Img_Laplacian()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat gray, dst, abs_dst;
	//��˹�˲���������
	GaussianBlur(img, img, cv::Size(3, 3), 0, 0);
	//ת��Ϊ�Ҷ�ͼ
	cvtColor(img, gray, CV_BGR2GRAY);
	Laplacian(gray, dst, CV_16S, 3, 1, 0);
	//�������ֵ���������תΪ8λ
	convertScaleAbs(dst, abs_dst);
	QImage outimg = cvMat2QImage(abs_dst);
	ShowImage(outimg, true);
}
void ImageMaster_Pro::Img_Full()
{
	QImage Img = imgLabel->pixmap()->toImage().scaled(scrollArea->width() - 2, scrollArea->height() - 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ShowImage(Img, true);
}

void ImageMaster_Pro::Img_Auto()
{
	QImage Img;
	double ImgRatio = 1.0 *  imgLabel->pixmap()->toImage().width() / imgLabel->pixmap()->toImage().height();
	double WinRatio = 1.0 * (scrollArea->width() - 2) / (scrollArea->height() - 2);
	if (ImgRatio > WinRatio)
	{
		Img = imgLabel->pixmap()->toImage().scaled((scrollArea->width() - 2), (scrollArea->width() - 2) / ImgRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	else
	{
		Img = imgLabel->pixmap()->toImage().scaled((scrollArea->height() - 2) * ImgRatio, (scrollArea->height() - 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	ShowImage(Img, true);
}
void ImageMaster_Pro::Img_RGB2GRAY()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	cvtColor(img, out, CV_BGR2GRAY);    //��ͼ��ת��ΪHSVͼ
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
void ImageMaster_Pro::Img_RGB2HSV()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	cvtColor(img, out, CV_BGR2HSV);    //��ͼ��ת��ΪHSVͼ
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
void ImageMaster_Pro::Img_Gamma()
{
	Attributes *frm = new Attributes();
	connect(frm, SIGNAL(sendGammaValue(QString)), this, SLOT(receiveData(QString)));
	frm->show();
}
void ImageMaster_Pro::receiveData(QString data)
{
	dgammavalue = data.toDouble();
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat imageGamma;
	img.convertTo(imageGamma, CV_64F, 1.0 / 255, 0);
	cv::Mat out;
	pow(imageGamma, dgammavalue, out);//dist Ҫ��imageGamma����ͬ����������
	out.convertTo(out, CV_8U, 255, 0);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
void ImageMaster_Pro::receiveflag(bool flag)
{
	isShowMatchDst = flag;
	if (isShowMatchDst)
	{
		QString dstimg = "result//siftMatch.png";
		QImage* img = new QImage();
		img->load(dstimg);
		ShowImage(*img, false);
	} 
	else
	{
		QMessageBox::information(this, tr("����"), tr("���������ļ��Ƿ���ȷ"));
	}
}

void  ImageMaster_Pro::Img_sift()
{
	FeatureMatch *fm_frm = new FeatureMatch();
	connect(fm_frm, SIGNAL(sendflag(bool)), this, SLOT(receiveflag(bool)));
	fm_frm->show();
}
void  ImageMaster_Pro::Img_surf()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
}
void  ImageMaster_Pro::Img_orb()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // ��ȡͼ��
	cv::Mat img = QImage2cvMat(srcimg);
}