#include "clsWeekMa.h"
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include "clsSingleStockData.h"
#include "clsGetLastWorkDay.h"
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

    showProgress(tr("正在获取最后一个交易日日期"));
    QString workDay =clsGetLastWorkDay::getLastWorkDate(QDate::currentDate()).toString("yyyy-MM-dd");
    int x=0;
    foreach (QString strCode, codes)
    {
        x++;
        //strCode ="sh603939";
        showProgress(QString("现在进度 %1/%2--找到：%3股票.")
                     .arg(x).arg(codes.length()).arg(stockCodes.length()));
        SingleStockDataList tmp = db->getStockData(strCode);
        SingleStockDataList weekTmp = splitToWeek(tmp,QDate::currentDate());

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
    QStringList tmp = condition.split(",");
    this->hsl = tmp.at(0).toDouble();
    this->average = tmp.at(1).toInt();
}
