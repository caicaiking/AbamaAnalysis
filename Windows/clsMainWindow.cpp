#include "clsMainWindow.h"
#include <QMessageBox>
#include <QDebug>
#include "clsDBCreateTables.h"
#include "clsDBOp.h"
#include <QInputDialog>
#include <QTextBlock>
#include "clsAttact.h"
#include "clsWeekMa.h"
#include "clsMaStrategy.h"
#include <QJsonDocument>
#include <QVariantMap>
#include <QJsonObject>
#include "smtpclient.h"
#include "mimetext.h"
#include "mimemessage.h"
#include "clsNumberInput.h"
#include "clsBollin.h"
#include "clsFavoriteOp.h"
//#include "clsStockShow.h"
#include "clsShowStockUi.h"

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
    this->hsl = clsNumberInput::getNumber(this,
                                          tr("设置换手率"),
                                          tr("换手率"),
                                          this->hsl,
                                          0,100);
}
//搜索均线系统
void clsMainWindow::on_btnMa_clicked()
{

    lblTitle->setText(btnMa->text());

    QInputDialog * dlg = new QInputDialog(this);
    dlg->setWindowTitle(tr("设置要使用的日均线"));

    bool ok;
    average = clsNumberInput::getNumber(this,tr("设置要使用的日均线"),tr("均线天数：")
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
    lblTitle->setText(btnAttact->text());
    strategy = clsStrategyFactory::getStrategy(Attack);
    connect(strategy,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));

    strategy->setCondition(getJsonString());
    lastResult = strategy->findStockCodes();
    txtCodes->setText(lastResult.join("\t"));
}
//搜索周平均值穿插平均线的股票
void clsMainWindow::on_btnWeekMa_clicked()
{
    bool ok;

    lblTitle->setText(btnWeekMa->text());
    average = clsNumberInput::getNumber(this,tr("设置要使用的周均线"),tr("均线周数：")
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




void clsMainWindow::on_btnShowStock_clicked()
{

    QString stockCode = txtCodes->textCursor().selectedText();

    //QString stockCode = "sz300081";
    if(stockCode.isEmpty())
        return;

    clsShowStockUi * dlg = new clsShowStockUi(this);
    dlg->setStockCode(stockCode);
    dlg->drawChart();
    dlg->show();

}

void clsMainWindow::on_btnSendEmail_clicked()
{
    //实例化发送邮件对象
    SmtpClient smtp("smtp.163.com",25,SmtpClient::TcpConnection);
    smtp.setUser("a_tsai@163.com");
    smtp.setPassword("caicaiking142514");

    //构建邮件主题，包括发件人附件等
    MimeMessage message;
    message.setSender(new EmailAddress("a_tsai@163.com"));

    //添加收件人
    message.addRecipient(new EmailAddress("abama.cai@waynekerr.net"));;

    // message.addRecipient(new EmailAddress("sclean.mao@waynekerr.net"));
    message.setSubject(QString("%1 Stock Codes").arg(QDate::currentDate().toString("yyyy-MM-dd")));

    MimeText text;
    text.setText(txtCodes->document()->toPlainText());

    message.addPart(&text);

    if(!smtp.connectToHost())
    {
        label->setText(tr("服务器连接失败"));
    }

    if(!smtp.login())
        label->setText(tr("用户登录失败"));

    if(!smtp.sendMail(message))
        label->setText(tr("邮件发送失败"));
    else
        label->setText(tr("邮件发送成功"));

    smtp.quit();

}

void clsMainWindow::on_btnBollin_clicked()
{
    lblTitle->setText(btnBollin->text());
    strategy = clsStrategyFactory::getStrategy(Bollin);
    connect(strategy,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));
    strategy->setCondition(getJsonString());
    lastResult= strategy->findStockCodes();
    txtCodes->setText(lastResult.join("\t"));
}

void clsMainWindow::on_btnBollinGreen_clicked()
{
    lblTitle->setText(btnBollinGreen->text());
    strategy = clsStrategyFactory::getStrategy(Bollin2);
    connect(strategy,SIGNAL(showProgress(QString)),label,SLOT(setText(QString)));
    strategy->setCondition(getJsonString());
    lastResult= strategy->findStockCodes();
    txtCodes->setText(lastResult.join("\t"));
}

void clsMainWindow::on_btnAddFavorite_clicked()
{
    clsFavoriteOp *op = new clsFavoriteOp(this);

    QString selectText = txtCodes->textCursor().selectedText();

    selectText = selectText.remove(QRegExp("\\s"));

    if(selectText.length() ==8)
    {
        op->addFavorite(selectText, txtDetail->document()->toPlainText());
    }


}

void clsMainWindow::on_btnShowFavorite_clicked()
{
    clsFavoriteOp *op = new clsFavoriteOp(this);

    lastResult = op->showFavorite();
    txtCodes->setText(lastResult.join("\t"));
}

void clsMainWindow::on_btnDeleteFavirate_clicked()
{
    clsFavoriteOp *op = new clsFavoriteOp(this);

    QString selectText = txtCodes->textCursor().selectedText();

    selectText = selectText.remove(QRegExp("\\s"));

    if(selectText.length() ==8)
    {
        op->deleteFavorite(selectText);
        txtDetail->clear();
    }

}

void clsMainWindow::on_txtCodes_selectionChanged()
{

    QString selectText = txtCodes->textCursor().selectedText();
    selectText = selectText.remove(QRegExp("\\s"));

    if(selectText.length() ==8)
    {
        clsFavoriteOp *op = new clsFavoriteOp(this);
        QStringList res =op->showFavorite(selectText);

        if(res.length() ==2)
        {
            txtDetail->setText(res.at(1));
        }
    }
    else
    {
        txtDetail->clear();
    }
}
