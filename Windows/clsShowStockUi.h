#ifndef CLSSHOWSTOCKUI_H
#define CLSSHOWSTOCKUI_H

#include "ui_clsShowStockUi.h"
#include "qchartviewer.h"
#include "clsSingleStockData.h"
#include "clsDBCreateTables.h"

class clsShowStockUi : public QDialog, private Ui::clsShowStockUi
{
    Q_OBJECT


public:
    explicit clsShowStockUi(QWidget *parent = 0);
    ~clsShowStockUi();
    void setStockCode(QString stockCode);
    void drawChart();

private:

    // TimeStamps, volume, high, low, open and close data
    QString m_tickerKey;
    double *m_timeStamps;
    double *m_volData;
    double *m_highData;
    double *m_lowData;
    double *m_openData;
    double *m_closeData;
    int m_noOfPoints;

    // An extra data series to compare with the close data
    QString m_compareKey;
    double *m_compareData;
    int m_compareDataLen;

    // The resolution of the data in seconds. 1 day = 86400 seconds.
    int m_resolution;

    // The moving average periods
    int m_avgPeriod1;
    int m_avgPeriod2;

    //用于显示图标
     QChartViewer *m_ChartViewer;

    // Routines to get data into the data arrays
    virtual bool getData(const QString &ticker, QDateTime startDate, QDateTime endDate,
            int durationInDays, int extraPoints);

    virtual void getDailyData(const QString &ticker, QDateTime startDate, QDateTime endDate);
    virtual void getWeeklyData(const QString &ticker, QDateTime startDate, QDateTime endDate);
    virtual void getMonthlyData(const QString &ticker, QDateTime startDate, QDateTime endDate);

    // Utilities to aggregate data if the data source do not already have aggregated data
    virtual void convertDailyToWeeklyData();
    virtual void convertDailyToMonthlyData();
    virtual void aggregateData(ArrayMath &aggregator);




     void initVariables();
     void initUi();

     void drawChart(QChartViewer *viewer);

     SingleStockDataList res;
     clsDBCreateTables *db;

protected:
     void resizeEvent(QResizeEvent *e);

private slots:
     void onComboBoxChanged(int);
     void onCheckBoxChanged();
     void onLineEditChanged();

};

#endif // CLSSHOWSTOCKUI_H
