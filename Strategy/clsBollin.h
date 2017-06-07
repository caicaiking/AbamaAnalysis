#ifndef CLSBOLLIN_H
#define CLSBOLLIN_H

#include "clsStrategy.h"
#include "memblock.h"
#include "clsDBCreateTables.h"
#include "clsSingleStockData.h"



class clsBollin : public clsStrategy
{
public:
    explicit clsBollin(QObject *parent=0);
    QStringList findStockCodes();
    void setCondition(QString condition);

private:
    void deepCopy(DoubleArray &dest, DoubleArray src);
private:
    double* m_timeStamps;
      double* m_highData;
      double* m_lowData;
      double* m_openData;
      double* m_closeData;
      double* m_volData;

    SingleStockDataList res;


    clsDBCreateTables *db;

    double hsl;
    int average;
    QStringList lastCode;
};

#endif // CLSBOLLIN_H
