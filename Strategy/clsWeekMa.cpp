#include "clsWeekMa.h"
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include "clsSingleStockData.h"
#include "clsGetLastWorkDay.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
clsWeekMa::clsWeekMa(QObject *parent)
{
    this->hsl =0;
    average = 20;

    db = new clsDBCreateTables(this);
}

QStringList clsWeekMa::findStockCodes()
{
    QStringList codes;
    if(lastCode.isEmpty())
        codes= db->getStockCodes();
    else
        codes = lastCode;
    QStringList stockCodes;

    showProgress(tr("正在获取最后一个交易日日期"));
    QString workDay =clsGetLastWorkDay::getLastWorkDate(
                QDateTime::currentDateTime().addSecs(-1*18*60*60).date()).toString("yyyy-MM-dd");

    int x=0;
    foreach (QString strCode, codes)
    {
        x++;
        //strCode ="sh603939";
        showProgress(QString("现在进度 %1/%2--找到：%3股票.")
                     .arg(x).arg(codes.length()).arg(stockCodes.length()));
       qApp->processEvents();
        SingleStockDataList tmp = db->getStockData(strCode);
        SingleStockDataList weekTmp = splitToWeek(tmp,QDate::fromString(workDay,"yyyy-MM-dd"));

        if(weekTmp.length()< average)
            continue;

        if(QDate::currentDate().toString("yyyy-MM-dd")> workDay)
        {
            if(weekTmp.first().date < workDay)
                continue;
        }
        else
        {
            if(weekTmp.first().date < QDateTime::currentDateTime().addSecs(-1*18*60*60).date().toString("yyyy-MM-dd"))
                continue;
        }

        double sum=0;
        for(int index=0;  index< average; index++)
        {
            sum+=weekTmp.at(index).close;
        }

        double dblAve = sum/average;

        if(dblAve > weekTmp.first().close)
            continue;
        if(dblAve < weekTmp.first().open)
            continue;

        if((weekTmp.first().close/weekTmp.first().open > 1.15) ||
                (weekTmp.first().close/weekTmp.first().open < 1.02))
            continue;




        stockCodes.append(strCode);

        qApp->processEvents();
    }

        showProgress(QString("查找已经完毕-共计 %1 股票").arg(stockCodes.length()));
    return stockCodes;

}


SingleStockDataList clsWeekMa::splitToWeek(SingleStockDataList tmp,
                                           QDate date)
{
    int i=1;
    i++;

    SingleStockDataList weekTmp;

    int dayOfWeek = date.dayOfWeek();
    QDate thisWeekEnd = date.addDays(7-dayOfWeek);
    QDate thisMonday = thisWeekEnd.addDays(-6);

    //    qDebug()<< thisWeekEnd;
    //    qDebug()<<thisMonday;

    SingleStockDataList data;
    for(int index = 0; index< tmp.length(); index++)
    {
        QString tmpDate = tmp.at(index).date;

        if((tmpDate >= thisMonday.toString("yyyy-MM-dd"))
                && (tmpDate <= thisWeekEnd.toString("yyyy-MM-dd")))
        {
            data.append(tmp.at(index));
        }
    }

    if(data.isEmpty())
        return weekTmp;

    SingleStockData weekData;
    weekData.open = data.last().open;
    weekData.close = data.first().close;
    weekData.date = data.first().date;
    weekData.zd = weekData.close - weekData.open;
    if(weekData.open!=0.0)
        weekData.zdf = weekData.close/weekData.open-1.0;
    double sum=0;
    for(int j =0; j< data.length(); j++)
    {
        sum+= data.at(j).cje;
    }
    weekData.cje = sum;

    weekTmp.append(weekData);

    qApp->processEvents();
    if(i > average)
        return weekTmp;
    else
    {
        SingleStockDataList tmpWeek=splitToWeek(tmp,thisMonday.addDays(-1));
        return weekTmp +tmpWeek;
    }




}

void clsWeekMa::setCondition(QString condition)
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
