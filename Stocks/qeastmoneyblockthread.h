#ifndef QEASTMONEYBLOCKTHREAD_H
#define QEASTMONEYBLOCKTHREAD_H

#include <QThread>
#include <QMap>
#include <QList>
#include "clsDBCreateTables.h"
#include "clsDBOp.h"


#include "BlockData.h"
typedef enum enBlockType
{
    BLOCK_DISTRICT = 1 ,
    BLOCK_INDUSTORY,
    BLOCK_CONCEPT,
}BLOCK_OPT_TYPE;

typedef enum enBlockDisplayRule
{
    BLOCK_DISPLAY_SORT_NONE = 0,
    BLOCK_DISPLAY_SORT_DESCEND,
    BLOCK_DISPLAY_SORT_ASCEND
}BLOCK_DISPLAY_RULE;

typedef QList<BlockData>        BlockDataList;

class QEastMoneyBlockThread : public QThread
{
    Q_OBJECT
public:
    explicit QEastMoneyBlockThread(QObject *parent = 0);
    ~QEastMoneyBlockThread();


protected:
    void run();
    void GetBlockShares();
    void GetShares(const QString &pBlockCode, QList<QString>& codelist);

signals:
    void    sendBlockDataList(const BlockDataList& list);
    void    updateBlockCodesFinished();
    void    sendStkinfoUpdateProgress(int cur, int total);
    void    signalUpdateMsg(const QString& msg);
public slots:


signals:
    void getBlockDataList(BlockDataMap x);

private:
    int         mSortRule;
    int         mOptType;
   BlockDataMap mBlockDataList;
    bool        mUpdateBlockCodes;

    QMap<QString, QStringList>  mShareBlockList;

    clsDBCreateTables *db;
};

#endif // QEASTMONEYBLOCKTHREAD_H
