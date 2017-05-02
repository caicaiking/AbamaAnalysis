#ifndef CLSMASTRATEGY_H
#define CLSMASTRATEGY_H

#include <QObject>
#include "clsStrategy.h"
#include <QStringList>

class clsMaStrategy : public clsStrategy
{
    Q_OBJECT
public:
   explicit clsMaStrategy(QObject *parent =0);

    QStringList findStockCodes();
    void setCondition(QString condition);

private:
    QString condition;
};

#endif // CLSMASTRATEGY_H
