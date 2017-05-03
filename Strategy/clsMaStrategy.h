#ifndef CLSMASTRATEGY_H
#define CLSMASTRATEGY_H

#include <QObject>
#include "clsStrategy.h"
#include <QStringList>
#include "clsDBCreateTables.h"

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
};

#endif // CLSMASTRATEGY_H
