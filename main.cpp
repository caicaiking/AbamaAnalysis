#include "clsMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clsMainWindow w;
    w.show();

    return a.exec();
}
