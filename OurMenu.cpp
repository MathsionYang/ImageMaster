#include "OurMenu.h"
#include "ImageMaster_Pro.h"
OurMenu::OurMenu(QWidget *parent, ImageMaster_Pro *p)
	: QMenuBar(parent)
{
	I_MainWindow = (ImageMaster_Pro*)p;
	I_ToolBar = new QToolBar;
	I_ToolBar = I_MainWindow->addToolBar(tr("�ļ�������"));
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
	// �ļ��˵�
	QMenu *menu1 =addMenu(tr("�ļ�"));

	// �˵�������Ӳ˵�
	// �����Ӳ˵�����һ������QIconΪ���ͼ�꣬ע��·���Ƿ���ȷ���ڶ����������Ӳ˵�����ʾ�ı�
	QAction *Act_Pro_New = new QAction(QIcon("logo//Image/file/New.png"), tr("�½�����"), this);
	// ������ݷ�ʽ��Ctrl+N��������Զ����ݼ�����������Ϊact->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	Act_Pro_New->setShortcuts(QKeySequence::New);
	// �Ӳ˵�����ʱִ�вۺ����������׵Ŀ��Բμ�ǰ�ģ��ź����
	connect(Act_Pro_New, SIGNAL(triggered()), this, SLOT(NewProject()));
	// �˵�������Ӳ˵�
	QAction *Act_file_open = new QAction(QIcon("logo/Image/file/Open.png"), tr("��"), this);
	Act_file_open->setShortcuts(QKeySequence::Open);// ��ݼ� Ctrl+O
	connect(Act_file_open, SIGNAL(triggered()), this, SLOT(File_Open()));

	QAction *Act_file_save = new QAction(QIcon("logo/Image/file/Save.png"), tr("����"), this);
	Act_file_save->setShortcuts(QKeySequence::Save);
	//Act_file_save->setStatusTip(tr("����ͼ��"));
	connect(Act_file_save, SIGNAL(triggered()), this, SLOT(File_save()));

	QAction *Act_file_saveas = new QAction(QIcon("logo/Image/file/SaveAs.png"), tr("���Ϊ"), this);
	Act_file_saveas->setShortcuts(QKeySequence::SaveAs);
	//Act_file_saveas->setStatusTip(tr("ͼ�����Ϊ"));
	connect(Act_file_saveas, SIGNAL(triggered()), this, SLOT(File_saveas()));

	QAction *Act_file_close = new QAction(QIcon("logo/Image/file/Close.png"), tr("�ر�"), this);
	Act_file_close->setShortcuts(QKeySequence::Close);
	//Act_file_close->setStatusTip(tr("�ر����"));
	connect(Act_file_close, SIGNAL(triggered()), I_MainWindow, SLOT(close()));

	QAction *Act_edit_undo = new QAction(QIcon("logo/Image/edit/Edit_Cancel.png"), tr("����"), this);
	Act_edit_undo->setShortcuts(QKeySequence::Undo);
	connect(Act_edit_undo, SIGNAL(triggered()), I_MainWindow, SLOT(Img_editundo()));

	QAction *Act_edit_redo = new QAction(QIcon("logo/Image/edit/Edit_Resume.png"), tr("����"), this);
	Act_edit_redo->setShortcuts(QKeySequence::Redo);
	connect(Act_edit_redo, SIGNAL(triggered()), I_MainWindow, SLOT(Img_editredo()));

	QAction *Act_img_fullscreen = new QAction(QIcon("logo/Image/view/resize.png"), tr("������Ļ"), this);
	connect(Act_img_fullscreen, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Full()));

	QAction *Act_edit_Auto = new QAction(QIcon("logo/Image/view/resize2.png"), tr("����Ӧ"), this);
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
// �½��ļ�
void OurMenu::NewProject()	
{
	QImage image = QImage(800, 650, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	I_MainWindow->ShowImage(image, true);
}
void OurMenu::File_Open()
{
	QString path = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), ".", tr("Images(*.jpg *.png *.bmp)"));
	if (!path.isEmpty())
	{
		QImage* img = new QImage();
		if (!(img->load(path)))
		{
			QMessageBox::information(this, tr("����"), tr("��ͼ��ʧ�ܣ�"));
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
		QString path = QFileDialog::getSaveFileName(this, tr("����ͼ��"), ".", tr("Images(*.jpg *.png *.bmp)"));
		{
			if (!path.isEmpty())
			{
				QImage img = I_MainWindow->imgLabel->pixmap()->toImage();     // ��ȡͼ��
				img.save(currentImgPath);                          // ����ͼ��
				currentImgPath = path;
				I_MainWindow->ShowImage(img, true);
			}
		}
	}
}
void OurMenu::File_saveas()
{
	QString path = QFileDialog::getSaveFileName(this, tr("ͼ�����Ϊ"), ".", tr("Images(*.jpg *.png *.bmp)"));
	if (!path.isEmpty())
	{
		
		QImage img = I_MainWindow->imgLabel->pixmap()->toImage();
		img.save(path);
		I_MainWindow->ShowImage(img, true);
	}
}
void OurMenu::Menu_ImgFilte()
{
	QMenu *menu2 = addMenu(tr("ͼ���˲�"));
	QAction *Act_file_boxFilter = new QAction(QIcon("logo/Image/view/filter1.png"), tr("�����˲�"), this);
	connect(Act_file_boxFilter, SIGNAL(triggered()), I_MainWindow, SLOT(Img_boxFilter()));

	QAction *Act_file_blur = new QAction(QIcon("logo/Image/view/filter2.png"), tr("��ֵ�˲�"), this);
	connect(Act_file_blur, SIGNAL(triggered()), I_MainWindow, SLOT(Img_blur()));

	QAction *Act_file_GaussianBlur = new QAction(QIcon("logo/Image/view/filter3.png"), tr("��˹�˲�"), this);
	connect(Act_file_GaussianBlur, SIGNAL(triggered()), I_MainWindow, SLOT(Img_GaussianBlur()));

	QAction *Act_file_Dilate = new QAction(QIcon("logo/Image/view/dilate.png"), tr("����"), this);
	connect(Act_file_Dilate, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Dilate()));

	QAction *Act_file_Erode = new QAction(QIcon("logo/Image/view/erode.png"), tr("��ʴ"), this);
	connect(Act_file_Erode, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Erode()));

	menu2->addAction(Act_file_boxFilter);
	menu2->addAction(Act_file_blur);
	menu2->addAction(Act_file_GaussianBlur);
	menu2->addAction(Act_file_Dilate);
	menu2->addAction(Act_file_Erode);
}


void OurMenu::Menu_ImgChange()
{
	QMenu *menu2 = addMenu(tr("ͼ��任"));
	QAction *Act_file_RGB2HSV = new QAction(QIcon("logo/Image/toolbox/colors.png"), tr("HSV��ɫ�ռ�"), this);
	connect(Act_file_RGB2HSV, SIGNAL(triggered()), I_MainWindow, SLOT(Img_RGB2HSV()));

	QAction *Act_file_GRAY = new QAction(QIcon("logo/Image/toolbox/colors.png"), tr("�Ҷ�Ӱ��"), this);
	connect(Act_file_GRAY, SIGNAL(triggered()), I_MainWindow, SLOT(Img_RGB2GRAY()));

	QAction *Act_file_Gama = new QAction(QIcon("logo/Image/toolbox/colors.png"), tr("٤��任"), this);
	connect(Act_file_Gama, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Gamma()));

	menu2->addAction(Act_file_GRAY);
	menu2->addAction(Act_file_RGB2HSV);
	menu2->addAction(Act_file_Gama);
}
//ͼ���Ե���
void OurMenu::Menu_ImgEdgeDetect()
{
	QMenu *menu3 =addMenu(tr("ͼ���Ե���"));
	QAction *Act_file_Cammy = new QAction(QIcon("logo/Image/toolbox/margin.png"), tr("Canny����"), this);
	connect(Act_file_Cammy, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Canny()));

	QAction *Act_file_Sobel = new QAction(QIcon("logo/Image/toolbox/margin.png"), tr("Sobel����"), this);
	connect(Act_file_Sobel, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Sobel()));

	QAction *Act_file_Laplacian = new QAction(QIcon("logo/Image/toolbox/margin.png"), tr("Laplacian����"), this);
	connect(Act_file_Laplacian, SIGNAL(triggered()), I_MainWindow, SLOT(Img_Laplacian()));
	menu3->addAction(Act_file_Cammy);
	menu3->addAction(Act_file_Sobel);
	menu3->addAction(Act_file_Laplacian);
}
void OurMenu::Menu_FeatureMatch()
{
	QMenu *menu3 = addMenu(tr("������ȡ"));
	QAction *Act_file_sift = new QAction(QIcon("logo/Image/view/point.png"), tr("SIFT������"), this);
	connect(Act_file_sift, SIGNAL(triggered()), I_MainWindow, SLOT(Img_sift()));

	QAction *Act_file_surf = new QAction(QIcon("logo/Image/view/point1.png"), tr("SURF������"), this);
	connect(Act_file_surf, SIGNAL(triggered()), I_MainWindow, SLOT(Img_surf()));

	QAction *Act_file_Orb = new QAction(QIcon("logo/Image/view/point2.png"), tr("ORB������"), this);
	connect(Act_file_Orb, SIGNAL(triggered()), I_MainWindow, SLOT(Img_orb()));

	menu3->addAction(Act_file_sift);
	menu3->addAction(Act_file_surf);
	menu3->addAction(Act_file_Orb);

}                                                                                                                                                  