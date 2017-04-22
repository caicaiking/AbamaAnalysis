#ifndef CLSSINGLESTOCKDATA_H
#define CLSSINGLESTOCKDATA_H

#include <QMap>

class clsSingleStockData
{
public :
    double o,c,Hi,Lo,lc,cjl,cje,zsz,ltsz,zdf;
    QString StNumber;

};

typedef QList<clsSingleStockData> SDList;

#endif // CLSSINGLESTOCKDATA_H
