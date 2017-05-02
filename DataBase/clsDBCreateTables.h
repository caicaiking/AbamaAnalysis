#ifndef CLSDBCREATETABLES_H
#define CLSDBCREATETABLES_H

#include <QObject>
#include "BlockData.h"
#include "clsSingleStockData.h"
#include <QMutex>

struct DetailSTR
{
    int blockName;
    int id;
    QStringList codes;
};
class clsDBCreateTables: public QObject
{
    Q_OBJECT
public:
    explicit clsDBCreateTables(QObject *parent =0);

      bool createStockTable();
      bool createBlockTable();
      void fillBlockTable(const QMap<QString, BlockData> data);

      bool createDetaiTable();
      void fillDetailTable(const QStringList mStkCodesList);
      void createCodesTable(QString code);
      void fillCodeTable(QString code, SingleStockDataList list);
      QDate getCodeLatestDate(QString code);
      static bool createUpdateTable();
      QString getLatestUpdate();
      void setUpdateTime(QDate t);
      void getBlockTable(QMap<QString, BlockData> &data);
      SingleStockDataList getStockData(QString strCode);
      QStringList getStockCodes();
signals:
    void showMessage(QString msg);
private:
      void fillStockTable();

      static QMutex m_operation;
      static QMutex m_detail;
};

#endif // CLSDBCREATETABLES_H
