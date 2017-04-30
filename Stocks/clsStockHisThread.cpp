#include "clsStockHisThread.h"
#include <QMapIterator>
#include "qeastmoneystockinfothread.h"
#include <QApplication>
#include <QMutex>
#include <QDateTime>
#include <QDebug>

clsStockHisThread::clsStockHisThread(QObject *parent):QThread(parent)
{
    db = new clsDBCreateTables(this);
}

void clsStockHisThread::setShareBlockCode(BlockDataMap x)
{
    this->mBlockDataList = x;
}

void clsStockHisThread::run()
{

    //开始取得所有股票代码
    QMap<QString, QStringList> allStkCode;
    QMapIterator<QString,BlockData> it(mBlockDataList);
    while(it.hasNext())
    {
        it.next();
        QString key = it.key();
        QStringList stCode = it.value().stklist;
        for(int i=0; i< stCode.length();i++)
        {
            allStkCode[stCode.at(i)].append(key);
        }
    }

    QStringList mStkCodesList = allStkCode.keys();
    mTotalStkCount= mStkCodesList.length();
    db->createDetaiTable();
    db->fillDetailTable(mStkCodesList);

    //开启线程将股本数据写入文件
    //开始启动线程然后获取信息,每个线程获取100个股票代码信息
        int nthread = (mStkCodesList.length()+99) / 100;

        qDebug()<<"thread count:"<<nthread;
        for(int i=0; i<nthread; i++)
        {
            QStringList wklist = mStkCodesList.mid(i*100, 100);
            qeastmoneystockinfothread *infothr = new qeastmoneystockinfothread;
            infothr->setStockCodeList(wklist);
            connect(infothr, SIGNAL(finished()), this, SLOT(slotWorkThreadFinished()));
            connect(infothr, SIGNAL(sendUpdateProgress(int,int)), this, SLOT(slotStkCodeUpdateFinish(int,int)));
            mWorkThreadList.append(infothr);
            infothr->start();
            qApp->processEvents();
        }

//    qeastmoneystockinfothread * infothr = new qeastmoneystockinfothread(this);
//    infothr->setStockCodeList(mStkCodesList);
//    connect(infothr, SIGNAL(finished()), this, SLOT(slotWorkThreadFinished()));
//    connect(infothr, SIGNAL(sendUpdateProgress(int,int)), this, SLOT(slotStkCodeUpdateFinish(int,int)));

//    mWorkThreadList.append(infothr);
//    infothr->start();

}

void clsStockHisThread::slotWorkThreadFinished()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QThread* wkthread = (QThread*) sender();
    //qDebug()<<"remove work thread:"<<wkthread;
    if(wkthread)
    {
        mWorkThreadList.removeAll(wkthread);
        wkthread->deleteLater();
        //emit sendStkinfoUpdateProgress();
    }
    QThread::msleep(1);
    //   qDebug()<<"Remain thread length: "<< mWorkThreadList.length();
    if(mWorkThreadList.length() == 0)
    {

        db->setUpdateTime(QDateTime::currentDateTime().addMSecs(-1*18*60*60).date());
        // qDebug()<< "update block codes finished!";
        this->exit(0);
    }
}

void clsStockHisThread::slotStkCodeUpdateFinish(int cur, int /*total*/)
{
    static int wkindex = 1;
    emit signalUpdateMsg(QString("%1:%2/%3").arg(tr("正在更新股票信息")).arg(wkindex).arg(mTotalStkCount));
    wkindex++;
}

