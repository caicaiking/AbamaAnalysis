#include "clsDBOp.h"
#include <QMutex>
#include <QMutexLocker>

clsDBOp * clsDBOp::mInstance =0;
clsDBOp::clsRmDBOp clsDBOp::rmDbOp;


clsDBOp::clsDBOp(QObject *parent) : QObject(parent)
{
    db =QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName("StockData.db");

}

clsDBOp::~clsDBOp()
{
    if(db.open())
        db.close();
}

clsDBOp *clsDBOp::instance()
{
    static QMutex mutex;

    if(mInstance ==0)
    {
        QMutexLocker locker(&mutex);
        if(mInstance ==0)
            mInstance = new clsDBOp();

    }

    return mInstance;
}

QSqlDatabase clsDBOp::getDb() const
{
    return db;
}
