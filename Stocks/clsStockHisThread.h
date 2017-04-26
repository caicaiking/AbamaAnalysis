#ifndef CLSSTOCKHISTHREAD_H
#define CLSSTOCKHISTHREAD_H

#include <QObject>
#include <QThread>
#include "clsDBCreateTables.h"
#include "BlockData.h"

class clsStockHisThread : public QThread
{
    Q_OBJECT
public:
    explicit clsStockHisThread(QObject *parent=0);
public slots:
    void setShareBlockCode(BlockDataMap x);

protected:
    void run();

signals:
    void signalUpdateMsg(QString msg);

private slots:
    void slotWorkThreadFinished();
    void slotStkCodeUpdateFinish(int cur, int total);
private:
    BlockDataMap mBlockDataList;
    clsDBCreateTables * db;
    QList<QThread*>     mWorkThreadList;
    int mTotalStkCount;
};

#endif // CLSSTOCKHISTHREAD_H
