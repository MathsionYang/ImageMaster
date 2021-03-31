#include "ImageMaster_Pro.h"
#include <QtWidgets/QApplication>
#include <QSplitter>
#include <QTextEdit>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ImageMaster_Pro w;
    w.show();
    return a.exec();
}
