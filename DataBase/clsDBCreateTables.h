#ifndef CLSDBCREATETABLES_H
#define CLSDBCREATETABLES_H

#include <QObject>
#include "BlockData.h"

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
      void fillCodeTable(QString code);
signals:
    void showMessage(QString msg);
private:
      void fillStockTable();
};

#endif // CLSDBCREATETABLES_H
