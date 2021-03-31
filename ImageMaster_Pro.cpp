
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
void ImageMaster_Pro::InitImage()        // 初始化图像
{
	// 初始化QDockWidget.在以后会讲到，是可移动隐藏的小窗口，
	// 可以实现PS、VS停靠窗口的效果，目前只需了解
	dock_Image = new QDockWidget(tr("图像"), this);          //   图像
	setCentralWidget(dock_Image);

	// 初始化QLabel
	imgLabel = new QLabel(dock_Image);
	imgLabel->setScaledContents(true);  // 设置QLabel自动适应图像大小

	// 初始图像
	QImage image = QImage(800, 650, QImage::Format_RGB32);  // 新建图像
	image.fill(qRgb(255, 255, 255));                        // 全白
	imgLabel->setPixmap(QPixmap::fromImage(image));         // 显示图像
	imgLabel->resize(image.width(), image.height());        // 图像与imgLabel同大小

	imgVector[0].push_back(image);
	iter = imgVector[0].end() - 1;						// 最后一个元素对应的迭代器指针
	// 增加滚动条,如果图像比imgLabel大，就会出现滚动条
	scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setAlignment(Qt::AlignCenter);
	scrollArea->setWidget(imgLabel);
	dock_Image->setWidget(scrollArea);
}

void ImageMaster_Pro::AppWinStyle()
{
	// 界面大小
	//resize(QApplication::desktop()->width()*0.5, QApplication::desktop()->height()*0.5);
	//move(QApplication::desktop()->width()*0.05, QApplication::desktop()->height()*0.01);
	resize(900, 800);
	setWindowTitle("图像处理大师");
	// 软件图标
	setWindowIcon(QIcon("logo/logo.png"));
	
}
cv::Mat ImageMaster_Pro::QImage2cvMat(QImage image)			// QImage改成Mat
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
QImage ImageMaster_Pro::cvMat2QImage(const cv::Mat& mat)    // Mat 改成 QImage
{
	if (mat.type() == CV_8UC1)					// 单通道
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		image.setColorCount(256);				// 灰度级数256
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		uchar *pSrc = mat.data;					// 复制mat数据
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}

	else if (mat.type() == CV_8UC3)				// 3通道
	{
		const uchar *pSrc = (const uchar*)mat.data;			// 复制像素
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);	// R, G, B 对应 0,1,2
		return image.rgbSwapped();				// rgbSwapped是为了显示效果色彩好一些。
	}
	else if (mat.type() == CV_8UC4)
	{
		const uchar *pSrc = (const uchar*)mat.data;			// 复制像素
															// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);		// B,G,R,A 对应 0,1,2,3
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
//方框滤波
void ImageMaster_Pro::Img_boxFilter()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	boxFilter(img, out, -1, cv::Size(5, 5));//-1指原图深度
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg,true);
}
//均值滤波
void ImageMaster_Pro::Img_blur()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	blur(img, out, cv::Size(5, 5));//-1指原图深度
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
//高斯滤波
void ImageMaster_Pro::Img_GaussianBlur()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	GaussianBlur(img, out, cv::Size(5, 5), 0, 0);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);

}
void ImageMaster_Pro::Img_Dilate()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	//获取自定义核
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	//膨胀操作
	cv::Mat out;
	dilate(img, out, element);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
void ImageMaster_Pro::Img_Erode()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	//获取自定义核
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	//膨胀操作
	cv::Mat out;
	erode(img, out, element);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);

}
//撤销操作
void ImageMaster_Pro::Img_editundo()
{
	if (iter != imgVector[0].begin())			// 前面还有对象
	{
		iter--;
		ShowImage(*iter, false);
	}
}
void ImageMaster_Pro::Img_editredo()
{
	if (iter != (imgVector[0].end() - 1))		// 后面还有对象
	{
		iter++;
		ShowImage(*iter, false);
	}
}
//Canny算子
void ImageMaster_Pro::Img_Canny()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat DstPic, edge, grayImage;
	//创建与src同类型和同大小的矩阵
	DstPic.create(img.size(), img.type());
	//将原始图转化为灰度图
	cvtColor(img, grayImage, CV_BGR2GRAY);
	//先使用3*3内核来降噪
	blur(grayImage, edge, cv::Size(3, 3));
	//运行canny算子
	Canny(edge, edge, 3, 9, 3);

	QImage outimg = cvMat2QImage(edge);
	ShowImage(outimg, true);
}
//Sobel算子
void ImageMaster_Pro::Img_Sobel()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat  gray_scrimage, grad_x, grad_y,out;
	cvtColor(img, gray_scrimage, CV_BGR2GRAY);   //转变为灰度图
	Sobel(gray_scrimage, grad_x, -1, 1, 0); // x方向导数运算参数，可取0,1,2
	Sobel(gray_scrimage, grad_y, -1, 0, 1); // y方向导数运算参数
	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, out);
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
//Laplacian算子
void ImageMaster_Pro::Img_Laplacian()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat gray, dst, abs_dst;
	//高斯滤波消除噪声
	GaussianBlur(img, img, cv::Size(3, 3), 0, 0);
	//转换为灰度图
	cvtColor(img, gray, CV_BGR2GRAY);
	Laplacian(gray, dst, CV_16S, 3, 1, 0);
	//计算绝对值，并将结果转为8位
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
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	cvtColor(img, out, CV_BGR2GRAY);    //将图像转换为HSV图
	QImage outimg = cvMat2QImage(out);
	ShowImage(outimg, true);
}
void ImageMaster_Pro::Img_RGB2HSV()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat out;
	cvtColor(img, out, CV_BGR2HSV);    //将图像转换为HSV图
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
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
	cv::Mat imageGamma;
	img.convertTo(imageGamma, CV_64F, 1.0 / 255, 0);
	cv::Mat out;
	pow(imageGamma, dgammavalue, out);//dist 要与imageGamma有相同的数据类型
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
		QMessageBox::information(this, tr("错误"), tr("请检查输入文件是否正确"));
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
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
}
void  ImageMaster_Pro::Img_orb()
{
	QImage srcimg = imgLabel->pixmap()->toImage();     // 读取图像
	cv::Mat img = QImage2cvMat(srcimg);
}