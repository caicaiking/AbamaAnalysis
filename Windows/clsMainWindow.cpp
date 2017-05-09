#include "clsMainWindow.h"
#include <QDebug>
#include "clsDBCreateTables.h"
#include "clsDBOp.h"
#include <QInputDialog>
#include "clsAttact.h"
#include "clsWeekMa.h"
#include "clsMaStrategy.h"
#include <QJsonDocument>
#include <QVariantMap>
#include <QJsonObject>

clsMainWindow::clsMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    mBlockThread = new QEastMoneyBlockThread(this);
    connect(mBlockThread,SIGNAL(signalUpdateMsg(QString)),label,SLOT(setText(QString)));


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

//更新股票数据
void clsMainWindow::startGetHisData()
{
    mStockHisThread->start();
}

//设置换手率
void clsMainWindow::on_actHsl_triggered()
{
    this->hsl = QInputDialog::getInt(this,
                                     tr("设置换手率"),
                                     tr("换手率"),
                                     this->hsl,
                                     0,100);
}
//搜索均线系统
void clsMainWindow::on_btnMa_clicked()
{

    QInputDialog * dlg = new QInputDialog(this);
    dlg->setWindowTitle(tr("设置要使用的日均线"));

    bool ok;
    average = QInputDialog::getInt(this,tr("设置要使用的日均线"),tr("均线天数：")
                                   ,this->average,5,200,1,&ok);

    if(!ok)
        return;

    strategy = clsStrategyFactory::getStrategy(DayMa);
    connect(strategy,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));

    strategy->setCondition(getJsonString());

    this->lastResult = strategy->findStockCodes();
    txtCodes->setText(lastResult.join("\t"));


}
//搜索上攻形态股票
void clsMainWindow::on_btnAttact_clicked()
{
    strategy = clsStrategyFactory::getStrategy(Attack);
    connect(strategy,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));

    strategy->setCondition(getJsonString());
    lastResult = strategy->findStockCodes();
    txtCodes->setText(lastResult.join("\t"));
}
//搜索周平均值穿插平均线的股票
void clsMainWindow::on_btnWeekMa_clicked()
{
    QInputDialog * dlg = new QInputDialog(this);
    dlg->setWindowTitle(tr("设置要使用的周均线"));

    bool ok;
    average = QInputDialog::getInt(this,tr("设置要使用的周均线"),tr("均线周数：")
                                   ,this->average,5,200,1,&ok);

    if(!ok)
        return;
    strategy = clsStrategyFactory::getStrategy(WeekMa);
    connect(strategy,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));
    strategy->setCondition(getJsonString());
    lastResult= strategy->findStockCodes();
    txtCodes->setText(lastResult.join("\t"));
}

void clsMainWindow::on_btnUpdateData_clicked()
{
    mBlockThread->start();

}

QString clsMainWindow::getJsonString()
{
    QVariantMap obj;

    obj.insert("hsl",hsl);
    obj.insert("average",this->average);

    if(chkUseLastResut->isChecked())
        obj.insert("stocks",this->lastResult.join(","));
    else
        obj.insert("stocks", QStringList());

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(obj);
    if(!jsonDocument.isNull())
        return jsonDocument.toJson();
    else
        return "";
}

void clsMainWindow::on_btnGenerate_clicked()
{
    qDebug()<< getJsonString();
}
