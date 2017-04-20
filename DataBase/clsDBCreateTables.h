#ifndef CLSDBCREATETABLES_H
#define CLSDBCREATETABLES_H

#include <QObject>


class clsDBCreateTables: public QObject
{
    Q_OBJECT
public:
   explicit clsDBCreateTables(QObject *parent =0);

    bool createStockTable();

    bool createBlockTable();
    bool fillBlockTable();
signals:
    void showMessage(QString msg);
private:
    void fillStockTable();
};

#endif // CLSDBCREATETABLES_H
