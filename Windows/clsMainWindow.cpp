#include "clsMainWindow.h"
#include "profiles.h"
#include <QDebug>
#include "clsDBCreateTables.h"
#include "clsDBOp.h"


clsMainWindow::clsMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    mBlockThread = new QEastMoneyBlockThread(this);

    connect(mBlockThread,SIGNAL(signalUpdateMsg(QString)),label,SLOT(setText(QString)));
    mBlockThread->start();
}
