#ifndef CLSMASTRATEGY_H
#define CLSMASTRATEGY_H

#include <QObject>
#include "clsStrategy.h"
#include <QStringList>
#include "clsDBCreateTables.h"
#include "FinanceChart.h"

///!
//! \brief The clsMaStrategy class
//!
//! 主要是完成了 Ma 的平均天数的收盘价，下面采用新的算法来计算。
//!
//!

class clsMaStrategy : public clsStrategy
{
    Q_OBJECT
public:
   explicit clsMaStrategy(QObject *parent =0);

    QStringList findStockCodes();
    void setCondition(QString condition);

private:
    QString condition;

    clsDBCreateTables * db;

    int average;
    int hsl;
    QStringList lastCode;
};

#endif // CLSMASTRATEGY_H
