#include "clsAttact.h"

#include "clsSingleStockData.h"
#include <QApplication>
#include <QDateTime>

clsAttact::clsAttact(QObject *parent):clsStrategy(parent)
{
    db = new clsDBCreateTables(this);
    hsl =0;
}

QStringList clsAttact::findStockCodes()
{
    QStringList codes = db->getStockCodes();

    QStringList stockCodes;
    int i=0;
    foreach (QString strCode, codes) {
        i++;
        SingleStockDataList tmp = db->getStockData(strCode);
        if(tmp.length()< 60)
            continue;

        if(tmp.first().date <QDateTime::currentDateTime().addSecs(-1*18*60*60).date().toString("yyyy-MM-dd"))
            continue;

         emit showProgress(QString("现在进度 %1/%2.").arg(i).arg(codes.length()));

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

    return stockCodes;
}

void clsAttact::setCondition(QString condition)
{
    this->hsl = condition.toDouble();
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
    return sum/average;

}
