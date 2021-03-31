#pragma once
#include <QtGui>
#include <QtWidgets>
#include <QMenuBar>
#include <QMainWindow>
class ImageMaster_Pro;

class OurMenu : public QMenuBar
{
	Q_OBJECT

public:
	OurMenu(QWidget *parent, ImageMaster_Pro *p);
	~OurMenu();
	QAction *Act_edit_undo, *Act_edit_redo, *Act_help_option;
private:
	ImageMaster_Pro *I_MainWindow;
	QString currentImgPath;                     // 当前图像的路径
	QToolBar *I_ToolBar;
	void Menu_File();
	void Menu_ImgFilte();
	void Menu_ImgEdgeDetect();
	void Menu_ImgChange();
	void Menu_FeatureMatch();

private slots:
	
	void NewProject();
	void File_Open();
	void File_save();
	void File_saveas();
};
