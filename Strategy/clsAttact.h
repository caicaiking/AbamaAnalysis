#ifndef CLSATTACT_H
#define CLSATTACT_H

#include <QObject>
#include "clsStrategy.h"
#include "clsDBCreateTables.h"

class clsAttact : public clsStrategy
{
public:
    explicit clsAttact(QObject *parent=0);

    QStringList findStockCodes();
    void setCondition(QString condition);

private:

    double hsl;
    clsDBCreateTables * db;
    double getAverage(const SingleStockDataList &tmp, int average);
};

#endif // CLSATTACT_H
