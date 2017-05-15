#include "clsAttact.h"

#include "clsSingleStockData.h"
#include <QApplication>
#include <QDateTime>
#include "clsGetLastWorkDay.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
clsAttact::clsAttact(QObject *parent):clsStrategy(parent)
{
    db = new clsDBCreateTables(this);
    hsl =0;
}

QStringList clsAttact::findStockCodes()
{
    QStringList codes;
    if(lastCode.isEmpty())
        codes= db->getStockCodes();
    else
        codes = lastCode;
    showProgress(tr("正在获取最后一个交易日日期"));
    QString workDay =clsGetLastWorkDay::getLastWorkDate(
                QDateTime::currentDateTime().addSecs(-1*18*60*60).date()).toString("yyyy-MM-dd");

    QStringList stockCodes;
    int i=0;
    foreach (QString strCode, codes) {
        i++;

        showProgress(QString("现在进度 %1/%2--找到：%3股票.")
                     .arg(i).arg(codes.length()).arg(stockCodes.length()));
        SingleStockDataList tmp = db->getStockData(strCode);
        if(tmp.length()< 60)
            continue;

        if(QDate::currentDate().toString("yyyy-MM-dd")> workDay)
        {
            if(tmp.first().date < workDay)
                continue;
        }
        else
        {
            if(tmp.first().date < QDateTime::currentDateTime().addSecs(-1*18*60*60).date().toString("yyyy-MM-dd"))
                continue;
        }

        double ave20 = getAverage(tmp,20);
        double ave40 = getAverage(tmp,40);
        double ave60 = getAverage(tmp,60);

        double c = tmp.first().close;
        double o = tmp.first().open;

        if(o>c)
            continue;
        if(ave40<=ave60)
            continue;
        if(ave20 <= ave40)
            continue;
        if(c<ave20)
            continue;
        if(o>ave20)
            continue;

        if((tmp.first().hsl * 10000.0) < (this->hsl*100.0))
            continue;

        stockCodes.append(strCode);
        qApp->processEvents();
    }
    showProgress(QString("查找已经完毕-共计 %1 股票").arg(stockCodes.length()));

    return stockCodes;
}

void clsAttact::setCondition(QString condition)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(condition.toUtf8(),
                                                &error);

    if(error.error == QJsonParseError::NoError)
    {
        if(doc.isObject())
        {
            QJsonObject obj = doc.object();

            this->average = obj.value("average").toInt();
            this->hsl = obj.value("hsl").toInt();
            this->lastCode = obj.value("stocks").toString().split(",", QString::SkipEmptyParts);
        }
    }
}

double clsAttact::getAverage(const SingleStockDataList &tmp,int average)
{
    if(tmp.length()< average)
        return 0.0;


    double sum=0;
    for(int i=0; i< average; i++)
    {
        sum+= tmp.at(i).close;
    }
    qApp->processEvents();
    return sum/average;

}
