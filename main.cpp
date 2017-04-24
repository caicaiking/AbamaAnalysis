
#include <QApplication>
#include "qeastmoneyblockthread.h"
#include <QDebug>
#include "clsMainWindow.h"
#include "clsDBOp.h"
#include "clsDBCreateTables.h"
#include <QDate>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    clsDBOp::instance()->isOpen();
    clsDBCreateTables::createUpdateTable();

    clsMainWindow w;
    w.show();
    return a.exec();
}
