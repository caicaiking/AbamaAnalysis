#include "clsShowStock.h"
#include "clsDBCreateTables.h"
#include <QDateTime>
#include <QDate>

clsShowStock::clsShowStock(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    db = new clsDBCreateTables();

    this->average =1;





}

void clsShowStock::setStockCode(QString stockCode)
{
    this->stockCode = stockCode;
}


void clsShowStock::showHq(int totle, ShowType type)
{
    if(this->stockCode.isEmpty())
        return ;

    SingleStockDataList data = db->getStockData(this->stockCode);

    SingleStockDataList weekData = splitToWeek(data);

    QCPFinancialDataContainer showData;

    for(int i=0; i< weekData.length(); i++)
    {
        QCPFinancialData d;

        d.key = weekData[i].getDateDbl();
        d.open = weekData[i].open;
        d.close = weekData[i].close;
        d.high = weekData[i].high;
        d.low = weekData[i].low;

        showData.add(d);

        if(i>50)
            break;
    }

    // create candlestick chart:
    QCPFinancial *candlesticks = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
    candlesticks->setName("Candlestick");
    candlesticks->setChartStyle(QCPFinancial::csCandlestick);
    candlesticks->data()->set(showData);
    candlesticks->setWidth(3600*24*7*0.75);
    candlesticks->setTwoColored(true);
    candlesticks->setBrushPositive(Qt::red);
    candlesticks->setBrushNegative(Qt::green);
    candlesticks->setPenPositive(QPen(Qt::red));
    candlesticks->setPenNegative(QPen(Qt::green));


//    volumeAxisRect->axis(QCPAxis::atBottom)->setTicker(dateTimeTicker);
//    volumeAxisRect->axis(QCPAxis::atBottom)->setTickLabelRotation(15);
    customPlot->xAxis->setBasePen(Qt::NoPen);
    customPlot->xAxis->setTickLabels(false);
    customPlot->xAxis->setTicks(false); // only want vertical grid in main axis rect, so hide xAxis backbone, ticks, and labels
    //customPlot->xAxis->setTicker(dateTimeTicker);
    customPlot->rescaleAxes();
    customPlot->xAxis->scaleRange(1.05, customPlot->xAxis->range().center());
    customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
    customPlot->replot();

}

SingleStockDataList clsShowStock::splitToMonth(SingleStockDataList tmp, QDate date)
{


}


SingleStockDataList clsShowStock::splitToWeek(SingleStockDataList tmp )
{

    if(tmp.isEmpty())
        return SingleStockDataList();

    QList<SingleStockDataList> data;

    SingleStockDataList tmpWeekData;
    QDate firstDay = tmp.first().getDate();
    QDate endOfWeek = firstDay.addDays(-1*tmp.first().getDate().dayOfWeek());
    for(int i=0; i< tmp.length(); i++)
    {
        if(tmp[i].getDate()> endOfWeek)
            tmpWeekData.append(tmp.at(i));
        else
        {
            data.append(tmpWeekData);
            tmpWeekData.clear();
            tmpWeekData.append(tmp.at(i));
            endOfWeek = endOfWeek.addDays(-7);
        }

    }

    if(data.isEmpty())
        return SingleStockDataList();

    SingleStockDataList returnData;

    foreach (SingleStockDataList tmpData, data)
    {
        if(tmpData.isEmpty())
            continue;
        SingleStockData weekTmp;

        weekTmp.open = tmpData.last().open;
        weekTmp.close = tmpData.first().close;
        weekTmp.date = tmpData.first().date;
        weekTmp.zd = weekTmp.close - weekTmp.open;
        if(weekTmp.open!=0.0)
            weekTmp.zdf = weekTmp.close/weekTmp.open-1.0;
        double sum=0;

        weekTmp.high = tmpData.at(0).high;
        weekTmp.low = tmpData.at(0).low;

        for(int j =0; j< tmpData.length(); j++)
        {
            sum+= tmpData.at(j).cje;

            if(weekTmp.high < tmpData.at(j).high)
                weekTmp.high = tmpData.at(j).high;

            if(weekTmp.low > tmpData.at(j).low)
                weekTmp.low = tmpData.at(j).low;

        }
        weekTmp.cje = sum;

        returnData.append(weekTmp);

    }

    returnData[0].setDate(returnData[0].getDate().addDays(5-firstDay.dayOfWeek()));

    return returnData;
}


void clsShowStock::on_btnWeek_clicked(bool checked)
{
    if(checked)
        showHq(1,Week);
}


void clsShowStock::on_btnMonth_clicked(bool checked)
{
    if(!checked)
        return;

    customPlot->removeGraph(customPlot->graph(0));
}

void clsShowStock::on_btnDay_clicked()
{

}
