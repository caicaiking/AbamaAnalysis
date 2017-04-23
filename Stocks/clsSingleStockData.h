#ifndef CLSSINGLESTOCKDATA_H
#define CLSSINGLESTOCKDATA_H

#include <QMap>

struct SingleStockData
{
   QString date;   //0 日期
   double open;    //1 今开
   double close;   //2 今收
   double zd;      //3 涨跌
   double zdf;     //4 涨跌幅
   double low;     //5 最低
   double high;    //6 最高
   double cjl;     //7 成交率
   double cje;     //8 成交额
   double hsl;     //9 换手率

};
typedef QList<SingleStockData> SingleStockDataList;
#endif // CLSSINGLESTOCKDATA_H
