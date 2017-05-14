#ifndef CLSSHOWSTOCK_H
#define CLSSHOWSTOCK_H

#include "ui_clsShowStock.h"

#include "clsSingleStockData.h"
#include "clsDBCreateTables.h"

class clsShowStock : public QDialog, private Ui::clsShowStock
{
    Q_OBJECT




public:
    explicit clsShowStock(QWidget *parent = 0);


    enum ShowType
    {
        Day = 0, Week =1, Year=3
    };

    Q_ENUM(ShowType)

    void setStockCode(QString stockCode);


private slots:


    void showHq(int totle, ShowType type);
    SingleStockDataList splitToWeek(SingleStockDataList tmp);
    SingleStockDataList splitToMonth(SingleStockDataList tmp, QDate date);



    void on_btnWeek_clicked(bool checked);

private:
    QString stockCode;
    int average;
    clsDBCreateTables *db;
};

#endif // CLSSHOWSTOCK_H
