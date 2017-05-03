#include "clsMaStrategy.h"
#include "clsSingleStockData.h"
#include "clsDBCreateTables.h"
#include <QDateTime>
#include <QApplication>
#include <QStringList>
#include <QDebug>
clsMaStrategy::clsMaStrategy(QObject *parent): clsStrategy(parent)
{
    db = new clsDBCreateTables(this);

}

QStringList clsMaStrategy::findStockCodes()
{

    qDebug()<<this->condition;

    QStringList codes = db->getStockCodes();
    QStringList stockCode;
    int x=0;
    foreach (QString strCode , codes) {
        x++;
        SingleStockDataList tmp  = db->getStockData(strCode);


        if(this->average ==0 )
            continue;

        if(tmp.length()< average)
            continue;

        if(tmp.first().date <QDateTime::currentDateTime().addSecs(-1*18*60*60).date().toString("yyyy-MM-dd"))
            continue;


        double sum=0;
        for(int i=0; i<average; i++)
        {
            sum+= tmp.at(i).close;
        }

        double dblAverage = sum / average;

        if((dblAverage>= tmp.first().open )
                && (dblAverage <= tmp.first().close)
                && (tmp.first().close >10)
                && (tmp.first().close <30)
                && ( tmp.first().hsl * 10000.0) > this->hsl*100.0)
            stockCode.append(strCode);

        showProgress(QString("现在进度 %1/%2.").arg(x).arg(codes.length()));
        qApp->processEvents();
    }

    showProgress("查找Ma已经完毕");

    return stockCode;
}

void clsMaStrategy::setCondition(QString condition)
{
    this->condition = condition;

    QStringList strList = condition.split(",");
    this->average = strList.at(0).toInt();
    this->hsl = strList.at(1).toInt();

}

