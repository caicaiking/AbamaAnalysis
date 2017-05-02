
#include <QApplication>
#include "qeastmoneyblockthread.h"
#include <QDebug>
#include "clsMainWindow.h"
#include "clsDBOp.h"
#include "clsDBCreateTables.h"
#include <QDate>
#include "clsMaStrategy.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    clsDBOp::instance()->isOpen();
    clsDBCreateTables::createUpdateTable();

    clsMaStrategy ma;
    ma.setCondition("60");

   qDebug()<< ma.findStockCodes();

//    clsMainWindow w;
//    w.show();
    return a.exec();
}
