#include "clsWeekMa.h"
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include "clsSingleStockData.h"

clsWeekMa::clsWeekMa(QObject *parent)
{
    this->hsl =0;
    average = 20;

    db = new clsDBCreateTables(this);
}

QStringList clsWeekMa::findStockCodes()
{
    QStringList codes = db->getStockCodes();
    QStringList stockCodes;
    foreach (QString strCode, codes) {
        SingleStockDataList tmp = db->getStockData(strCode);
        SingleStockDataList weekTmp = splitToWeek(tmp,QDate::currentDate());


        qDebug()<<"Stop here!";




    }


    return stockCodes;

}


SingleStockDataList clsWeekMa::splitToWeek(SingleStockDataList tmp,
                                           QDate date)
{
    static int i=0;
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

    if(i> average)
        return weekTmp;
    else
    {
        SingleStockDataList xx=splitToWeek(tmp,thisMonday.addDays(-1));
        return weekTmp +xx;
    }


    return weekTmp;

}

void clsWeekMa::setCondition(QString condition)
{
    QStringList tmp = condition.split(",");
    this->hsl = tmp.at(0).toDouble();
    this->average = tmp.at(1).toInt();
}
