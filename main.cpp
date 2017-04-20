
#include <QApplication>
#include "qsinastkinfothread.h"
#include "qeastmoneyblockthread.h"
#include <QDebug>
#include "clsMainWindow.h"
#include "clsDBOp.h"
#include "clsDBCreateTables.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


//    QStringList indexlist;
//    indexlist<<"sh000001"<<"sh000300"<<"sz399001"<<"sh000043"<<"sz399006";

//    QSinaStkInfoThread * mIndexThread = new QSinaStkInfoThread(&a);
//    mIndexThread->setStkList(indexlist);
//    mIndexThread->start();

//    QEastMoneyBlockThread *mBlockThread = new QEastMoneyBlockThread(&a);
//    mBlockThread->start();


    clsDBOp::instance()->isOpen();

    clsMainWindow w;
    w.show();



    return a.exec();
}
