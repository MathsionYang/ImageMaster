#include "OurMenu.h"
#include "ImageMaster_Pro.h"
OurMenu::OurMenu(QWidget *parent, ImageMaster_Pro *p)
	: QMenuBar(parent)
{
	I_MainWindow = (ImageMaster_Pro*)p;
	I_ToolBar = new QToolBar;
	I_ToolBar = I_MainWindow->addToolBar(tr("文件工具栏"));
	I_ToolBar->setMovable(false);
	Menu_File();
	Menu_ImgFilte();
	Menu_ImgEdgeDetect();
	Menu_ImgChange();
	Menu_FeatureMatch();
}

OurMenu::~OurMenu()
{
}


void OurMenu::Menu_File()
{
	// 文件菜单
	QMenu *menu1 =addMenu(tr("文件"));

	// 菜单栏添加子菜单
	// 创建子菜单，第一个参数QIcon为添加图标，注意路径是否正确，第二个参数是子菜单的显示文本
	QAction *Act_Pro_New = new QAction(QIcon("logo//Image/file/New.png"), tr("新建工程"), this);
	// 创建快捷方式：Ctrl+N，如果想自定义快捷键，可以设置为act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	Act_Pro_New->setShortcuts(QKeySequence::New);
	// 子菜单触发时执行槽函数，不明白的可以参见前文：信号与槽
	connect(Act_Pro_New, SIGNAL(triggered()), this, SLOT(NewProject()));
	// 菜单栏添加子菜单
	QAction *Act_file_open = new QAction(QIcon("logo/Image/file/Open.png"), tr("打开"), this);
	Act_file_open->setShortcuts(QKeySequence::Open);// 快捷键 Ctrl+O
	connect(Act_file_open, SIGNAL(triggered()), this, SLOT(File_Open()));

	QAction *Act_file_save = new QAction(QIcon("logo/Image/file/Save.png"), tr("保存"), this);
	Act_file_save->setShortcuts(QKeySequence::Save);
	//Act_file_save->setStatusTip(tr("保存图像"));
	connect(Act_file_save, SIGNAL(triggered()), this, SLOT(File_save()));

	QAction *Act_file_saveas = new QAction(QIcon("logo/Image/file/SaveAs.png"), tr("另存为"), this);
	Act_file_saveas->setShortcuts(QKeySequence::SaveAs);
	//Act_file_saveas->setStatusTip(tr("图像另存为"));
	connect(Act_file_saveas, SIGNAL(triggered()), this, SLOT(File_saveas()));

	QAction *Act_file_close = new QAction(QIcon("logo/Image/file/Close.png"), tr("关闭"), this);
	Act_file_close->setShortcuts(QKeySequence::Close);
	//Act_file_close->setStatusTip(tr("关闭软件"));
	connect(Act_file_close, SIGNAL(triggered()), I_MainWindow, SLOT(close()));

	QAction *Act_edit_undo = new QAction(QIcon("logo/Image/edit/Edit_Cancel.png"), tr("撤销"), this);
	Act_edit_undo->setShortcuts(QKeySequence::Undo);
	connect(Act_edit_undo, SIGNAL(triggered()), I_MainWindow, SLOT(Img_editundo()));

	QAction *Act_edit_redo = new QAction(QIcon("logo/Image/edit/Edit_Resume.png"), tr("重做"), this);
	Act_edit_redo->setShortcuts(QKeySequence::Redo);
	connect(Act_edit_redo, SIGNAL(triggered()), I_MainWindow, SLOT(Img_editredo()));

	QAction *Act_img_fullscreen = new QAction(QIcon("logo/Image/view/resize.png"), tr("布满屏幕"), this);
	connect(Act_img_fullscreen, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Full()));

	QAction *Act_edit_Auto = new QAction(QIcon("logo/Image/view/resize2.png"), tr("自适应"), this);
	Act_edit_Auto->setShortcuts(QKeySequence::Redo);
	connect(Act_edit_Auto, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Auto()));


	menu1->addAction(Act_Pro_New);
	menu1->addAction(Act_file_open);
	menu1->addAction(Act_file_save);
	menu1->addAction(Act_file_saveas);
	menu1->addAction(Act_file_close);
	I_ToolBar->addAction(Act_file_open);
	I_ToolBar->addAction(Act_file_saveas);
	I_ToolBar->addAction(Act_img_fullscreen);
	I_ToolBar->addAction(Act_edit_Auto);
	I_ToolBar->addAction(Act_edit_undo);
	I_ToolBar->addAction(Act_edit_redo);
}
// 新建文件
void OurMenu::NewProject()	
{
	QImage image = QImage(800, 650, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	I_MainWindow->ShowImage(image, true);
}
void OurMenu::File_Open()
{
	QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), ".", tr("Images(*.jpg *.png *.bmp)"));
	if (!path.isEmpty())
	{
		QImage* img = new QImage();
		if (!(img->load(path)))
		{
			QMessageBox::information(this, tr("错误"), tr("打开图像失败！"));
			delete img;
			return;
		}
		I_MainWindow->ShowImage(*img, true);
		
		currentImgPath = path;
	}
}

void OurMenu::File_save()
{
	if (currentImgPath.isEmpty())
	{
		QString path = QFileDialog::getSaveFileName(this, tr("保存图像"), ".", tr("Images(*.jpg *.png *.bmp)"));
		{
			if (!path.isEmpty())
			{
				QImage img = I_MainWindow->imgLabel->pixmap()->toImage();     // 读取图像
				img.save(currentImgPath);                          // 保存图像
				currentImgPath = path;
				I_MainWindow->ShowImage(img, true);
			}
		}
	}
}
void OurMenu::File_saveas()
{
	QString path = QFileDialog::getSaveFileName(this, tr("图像另存为"), ".", tr("Images(*.jpg *.png *.bmp)"));
	if (!path.isEmpty())
	{
		
		QImage img = I_MainWindow->imgLabel->pixmap()->toImage();
		img.save(path);
		I_MainWindow->ShowImage(img, true);
	}
}
void OurMenu::Menu_ImgFilte()
{
	QMenu *menu2 = addMenu(tr("图像滤波"));
	QAction *Act_file_boxFilter = new QAction(QIcon("logo/Image/view/filter1.png"), tr("方框滤波"), this);
	connect(Act_file_boxFilter, SIGNAL(triggered()), I_MainWindow, SLOT(Img_boxFilter()));

	QAction *Act_file_blur = new QAction(QIcon("logo/Image/view/filter2.png"), tr("均值滤波"), this);
	connect(Act_file_blur, SIGNAL(triggered()), I_MainWindow, SLOT(Img_blur()));

	QAction *Act_file_GaussianBlur = new QAction(QIcon("logo/Image/view/filter3.png"), tr("高斯滤波"), this);
	connect(Act_file_GaussianBlur, SIGNAL(triggered()), I_MainWindow, SLOT(Img_GaussianBlur()));

	QAction *Act_file_Dilate = new QAction(QIcon("logo/Image/view/dilate.png"), tr("膨胀"), this);
	connect(Act_file_Dilate, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Dilate()));

	QAction *Act_file_Erode = new QAction(QIcon("logo/Image/view/erode.png"), tr("腐蚀"), this);
	connect(Act_file_Erode, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Erode()));

	menu2->addAction(Act_file_boxFilter);
	menu2->addAction(Act_file_blur);
	menu2->addAction(Act_file_GaussianBlur);
	menu2->addAction(Act_file_Dilate);
	menu2->addAction(Act_file_Erode);
}


void OurMenu::Menu_ImgChange()
{
	QMenu *menu2 = addMenu(tr("图像变换"));
	QAction *Act_file_RGB2HSV = new QAction(QIcon("logo/Image/toolbox/colors.png"), tr("HSV颜色空间"), this);
	connect(Act_file_RGB2HSV, SIGNAL(triggered()), I_MainWindow, SLOT(Img_RGB2HSV()));

	QAction *Act_file_GRAY = new QAction(QIcon("logo/Image/toolbox/colors.png"), tr("灰度影像"), this);
	connect(Act_file_GRAY, SIGNAL(triggered()), I_MainWindow, SLOT(Img_RGB2GRAY()));

	QAction *Act_file_Gama = new QAction(QIcon("logo/Image/toolbox/colors.png"), tr("伽马变换"), this);
	connect(Act_file_Gama, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Gamma()));

	menu2->addAction(Act_file_GRAY);
	menu2->addAction(Act_file_RGB2HSV);
	menu2->addAction(Act_file_Gama);
}
//图像边缘检测
void OurMenu::Menu_ImgEdgeDetect()
{
	QMenu *menu3 =addMenu(tr("图像边缘检测"));
	QAction *Act_file_Cammy = new QAction(QIcon("logo/Image/toolbox/margin.png"), tr("Canny算子"), this);
	connect(Act_file_Cammy, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Canny()));

	QAction *Act_file_Sobel = new QAction(QIcon("logo/Image/toolbox/margin.png"), tr("Sobel算子"), this);
	connect(Act_file_Sobel, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Sobel()));

	QAction *Act_file_Laplacian = new QAction(QIcon("logo/Image/toolbox/margin.png"), tr("Laplacian算子"), this);
	connect(Act_file_Laplacian, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Laplacian()));
	menu3->addAction(Act_file_Cammy);
	menu3->addAction(Act_file_Sobel);
	menu3->addAction(Act_file_Laplacian);
}
void OurMenu::Menu_FeatureMatch()
{
	QMenu *menu3 = addMenu(tr("特征提取"));
	QAction *Act_file_sift = new QAction(QIcon("logo/Image/view/point.png"), tr("SIFT特征点"), this);
	connect(Act_file_sift, SIGNAL(triggered()), I_MainWindow, SLOT(Img_sift()));

	QAction *Act_file_surf = new QAction(QIcon("logo/Image/view/point1.png"), tr("SURF特征点"), this);
	connect(Act_file_surf, SIGNAL(triggered()), I_MainWindow, SLOT(Img_surf()));

	QAction *Act_file_Orb = new QAction(QIcon("logo/Image/view/point2.png"), tr("ORB特征点"), this);
	connect(Act_file_Orb, SIGNAL(triggered()), I_MainWindow, SLOT(Img_orb()));

	menu3->addAction(Act_file_sift);
	menu3->addAction(Act_file_surf);
	menu3->addAction(Act_file_Orb);

}                                                                                                                                                  