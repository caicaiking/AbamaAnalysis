#include "clsMainWindow.h"
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

    mStockHisThread = new clsStockHisThread(this);
    connect(mStockHisThread,SIGNAL(signalUpdateMsg(QString)),label,SLOT(setText(QString)));
    connect(mBlockThread,SIGNAL(getBlockDataList(BlockDataMap)),
            mStockHisThread,SLOT(setShareBlockCode(BlockDataMap)));
    connect(mBlockThread,SIGNAL(updateBlockCodesFinished()),
            this,SLOT(startGetHisData()));
}

clsMainWindow::~clsMainWindow()
{
    if(mBlockThread->isRunning())
    {
        mBlockThread->quit();
    }
}

void clsMainWindow::startGetHisData()
{
    mStockHisThread->start();
}
