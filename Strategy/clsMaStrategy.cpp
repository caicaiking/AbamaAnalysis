#include "clsMaStrategy.h"
#include "clsSingleStockData.h"
#include "clsDBCreateTables.h"
#include <QDateTime>
#include <QApplication>
#include <QStringList>
#include <QDebug>
#include "clsGetLastWorkDay.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

clsMaStrategy::clsMaStrategy(QObject *parent): clsStrategy(parent)
{
    db = new clsDBCreateTables(this);

}

QStringList clsMaStrategy::findStockCodes()
{
    showProgress(tr("正在获取最后一个交易日日期"));
    QString workDay =clsGetLastWorkDay::getLastWorkDate(
                QDateTime::currentDateTime().addSecs(-1*18*60*60).date()).toString("yyyy-MM-dd");


    QStringList codes;
    if(lastCode.isEmpty())
        codes= db->getStockCodes();
    else
        codes = lastCode;

    QStringList stockCode;
    int x=0;
    foreach (QString strCode , codes) {
        x++;
        showProgress(QString("现在进度 %1/%2--找到：%3股票.")
                     .arg(x).arg(codes.length()).arg(stockCode.length()));
        qApp->processEvents();
        SingleStockDataList tmp  = db->getStockData(strCode);


        if(this->average ==0 )
            continue;

        if(tmp.length()< average)
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


        double sum=0;
        for(int i=0; i<average; i++)
        {
            sum+= tmp.at(i).close;
        }

        double dblAverage = sum / average;

        if((dblAverage>= tmp.first().open )
                && (dblAverage <= tmp.first().close)
                //                && (tmp.first().close >10)
                //                && (tmp.first().close <30)
                && ( tmp.first().hsl * 10000.0) > this->hsl*100.0)
            stockCode.append(strCode);

        showProgress(QString("现在进度 %1/%2.").arg(x).arg(codes.length()));
        qApp->processEvents();
    }

    showProgress(QString("查找已经完毕-共计 %1 股票").arg(stockCode.length()));

    return stockCode;
}

void clsMaStrategy::setCondition(QString condition)
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

