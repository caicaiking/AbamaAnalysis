#include "clsMaStrategy.h"
#include "clsSingleStockData.h"
#include "clsDBCreateTables.h"
#include <QDateTime>
clsMaStrategy::clsMaStrategy(QObject *parent): clsStrategy(parent)
{
    db = new clsDBCreateTables(this);

}

QStringList clsMaStrategy::findStockCodes()
{

    QStringList codes = db->getStockCodes();
    QStringList stockCode;
    foreach (QString strCode , codes) {

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

        if(dblAverage>= tmp.first().open && dblAverage <= tmp.first().close)
            stockCode.append(strCode);
    }

    return stockCode;
}

void clsMaStrategy::setCondition(QString condition)
{
    this->condition = condition;
    this->average = this->condition.toInt();

}

