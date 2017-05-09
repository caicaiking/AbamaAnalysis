#ifndef CLSWEEKMA_H
#define CLSWEEKMA_H

#include <QObject>
#include <clsStrategy.h>
#include "clsDBCreateTables.h"

class clsWeekMa : public clsStrategy
{
public:
   explicit clsWeekMa(QObject *parent=0);

    QStringList findStockCodes();
    void setCondition(QString condition);

private:
    QStringList lastCode;
    double hsl;
    int average;
    clsDBCreateTables *db;
    SingleStockDataList splitToWeek(SingleStockDataList tmp, QDate date);
};

#endif // CLSWEEKMA_H
