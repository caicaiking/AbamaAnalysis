#include "clsMainWindow.h"
#include <QDebug>
#include "clsDBCreateTables.h"
#include "clsDBOp.h"
#include <QInputDialog>
#include "clsAttact.h"

#include "clsMaStrategy.h"
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

    this->hsl =2;
    this->average =60;
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

void clsMainWindow::on_btnMa_clicked()
{

    QInputDialog * dlg = new QInputDialog(this);
    dlg->setWindowTitle(tr("设置要使用的均线"));

    bool ok;
      average = QInputDialog::getInt(this,tr("设置要使用的均线"),tr("均线天数：")
                                       ,this->average,5,200,1,&ok);

    if(!ok)
        return;


    clsMaStrategy ma;
    connect(&ma,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));

    ma.setCondition(QString::number(average)+","+QString::number(this->hsl));

    QStringList tmp = ma.findStockCodes();
    txtCodes->setText(tmp.join(","));


}



void clsMainWindow::on_actHsl_triggered()
{
    this->hsl = QInputDialog::getInt(this,
                                        tr("设置换手率"),
                                        tr("换手率"),
                                        this->hsl,
                                        0,100);
}

void clsMainWindow::on_btnAttact_clicked()
{
    clsAttact ma;
    connect(&ma,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));
    ma.setCondition(QString::number(this->hsl));
    QStringList tmp = ma.findStockCodes();
    txtCodes->setText(tmp.join(","));
}
