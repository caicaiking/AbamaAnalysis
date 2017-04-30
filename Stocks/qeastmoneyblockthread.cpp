#include "qeastmoneyblockthread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QEventLoop>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <QApplication>
#include "qeastmoneystockinfothread.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "qexchangedatamanage.h"
#include "clsDBCreateTables.h"
#include <QDateTime>


QString blockthread[4] = {"NONE", "DY", "HY", "GN"};

QEastMoneyBlockThread::QEastMoneyBlockThread(QObject *parent) : QThread(parent)
{
    mOptType = BLOCK_INDUSTORY;
    mSortRule = -1;
   // mUpdateBlockCodes = true;
    mBlockDataList.clear();
    //qRegisterMetaType<BlockDataList>("const BlockDataList&");
    qRegisterMetaType<BlockDataMap>("BlockDataMap");
    //设定分红送配的日期

    db = new clsDBCreateTables(this);
    connect(db,SIGNAL(showMessage(QString)),this,SIGNAL(signalUpdateMsg(QString)));
    db->createStockTable();
    db->createBlockTable();
}

QEastMoneyBlockThread::~QEastMoneyBlockThread()
{

}

void QEastMoneyBlockThread::GetShares(const QString &pBlockCode, QList<QString>& codelist)
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager;

    //QString wkURL = QString("http://hqdigi2.eastmoney.com/EM_Quote2010NumericApplication/index.aspx?type=s&sortType=D&sortRule=-1&pageSize=500&page=1&jsName=quote_123&style=%1").arg(blockCode);
    QString wkURL = QString("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C.BK0wkcode1&sty=FCOIATA&sortType=C&sortRule=-1&page=1&pageSize=1000&js=var%20quote_123%3d{rank:[(x)],pages:(pc)}&token=7bc05d0d4c3c22ef9fca8c2a912d779c&jsName=quote_123&_g=0.5186116359042079");
    wkURL.replace("wkcode", pBlockCode.right(3));

    QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
    if(!reply) return;
    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply->error())
    {
        qDebug()<<"err occured:"<<reply->errorString();
        reply->deleteLater();
        return;
    }
    //开始解析数据
    QByteArray bytes = reply->readAll();
    QString result = QString::fromUtf8(bytes.data());
    //qDebug()<<"result:"<<result;
    int startindex = result.indexOf("[") + 1;
    int endindex = result.indexOf("]");
    if(startindex < 0 || endindex < 0) return;
    QString hqStr = result.mid(startindex, endindex - startindex);
    if( !hqStr.isEmpty() )
    {
        QStringList stockInfoList = hqStr.split(QRegExp("\"|\","), QString::SkipEmptyParts);
        //qDebug()<<stockInfoList;
        foreach (QString stockInfo, stockInfoList) {
            QStringList detailList = stockInfo.split(",", QString::SkipEmptyParts);
            //qDebug()<<stockInfo;
            if(detailList.length() < 20) continue;
            QString code = detailList[1];
            mShareBlockList[code].append(pBlockCode);
            if(code.left(1) == "6" || code.left(1) == "5")
            {
                code  = "sh"+code;
            } else
            {
                code = "sz" + code;
            }
            codelist.append(code);
        }
    }
    reply->deleteLater();

    mgr->deleteLater();
}

void QEastMoneyBlockThread::GetBlockShares()
{
    mShareBlockList.clear();
    //取得上次更新的时间

    QString strLastUpdate = db->getLatestUpdate();

    qDebug()<<"last update :"<<strLastUpdate;
    QDateTime lastDate = QDateTime::fromString(strLastUpdate,"yyyy-MM-dd");
    emit this->signalUpdateMsg(tr("最近更新日期： %1").arg(strLastUpdate));

     db->getBlockTable(mBlockDataList);
 //每周一更新Block
     QList<int> updateDay;
     updateDay<<2<<4;

     //qDebug()<<QDateTime::currentDateTime().date().dayOfWeek();
    if((mBlockDataList.isEmpty())||updateDay.contains(QDateTime::currentDateTime().date().dayOfWeek()))
    {
        QNetworkAccessManager *mgr = new QNetworkAccessManager;
        QString url("http://nufm.dfcfw.com/EM_Finance2014NumericApplication/JS.aspx?type=CT&cmd=C._BK%1&sty=FCCS&st=c&p=1&ps=100&cb=&js=var%20BKtrade%20={Trade:[[(x)]]}&token=d0075ac6916d4aa6ec8495db9efe7ede&bklb=%2&jsName=BKtrade&sr=%3&dt=%4");
        for(int i = BLOCK_DISTRICT; i<=BLOCK_CONCEPT; i++ )
        {
            mOptType = i;
            QString wkURL = url.arg(blockthread[mOptType]).arg(mOptType).arg(mSortRule).arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
            //qDebug()<<"wkURL:"<<wkURL;
            QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
            if(!reply) continue;
            QEventLoop loop; // 使用事件循环使得网络通讯同步进行
            connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
            if(reply->error())
            {
                qDebug()<<"err occured:"<<reply->errorString();
                reply->deleteLater();
                continue;
            }
            //开始解析数据
            QByteArray bytes = reply->readAll();
            QString result = QString::fromUtf8(bytes.data());
            //qDebug()<<"result:"<<result;
            int startindex = -1, endindex = -1;
            startindex = result.indexOf("[[") + 2;
            endindex = result.indexOf("]]}");
            if(startindex < 0 || endindex < 0) continue;
            QString hqStr = result.mid(startindex, endindex - startindex);
            //qDebug()<<"blockstr:"<<hqStr;
            if( !hqStr.isEmpty() )
            {
                BlockDataList wklist;
                QStringList blockInfoList = hqStr.split(QRegExp("\"|\","), QString::SkipEmptyParts);
                //qDebug()<<stockInfoList;
                int total = blockInfoList.length();
                int index = 0;
                QString content = (i==BLOCK_DISTRICT? tr("地区") : i== BLOCK_CONCEPT? tr("概念"): tr("行业"));
                foreach (QString blockInfo, blockInfoList) {
                    index++;
                    qDebug()<<QString("%1%2:%3/%4").arg(tr("正在更新")).arg(content).arg(index).arg(total);
                    emit signalUpdateMsg(QString("%1%2:%3/%4").arg(tr("正在更新")).arg(content).arg(index).arg(total));
                    QStringList detailList = blockInfo.split(",", QString::SkipEmptyParts);
                    //qDebug()<<"line:"<<detailList;
                    if(detailList.length() < 11) continue;
                    BlockData data;
                    QString replaceStr = mOptType == BLOCK_CONCEPT ? "3" : mOptType == BLOCK_INDUSTORY ? "2" : "1";
                    data.code = detailList[1].replace("BK0", replaceStr);
                    data.name = detailList[2];
                    wklist.append(data);
                    //取得对应的所有code 并且获取每一个S
                    GetShares(data.code, data.stklist);

                    mBlockDataList[data.code] = data;

                }
            }

            reply->deleteLater();
        }

        //在这儿创建 板块数据库和股票代码数据库
        db->fillBlockTable(mBlockDataList);
        db->setUpdateTime(QDateTime::currentDateTime().addSecs(-1*18*60*60).date());
        mgr->deleteLater();
    }
    db->getBlockTable(mBlockDataList);
    emit getBlockDataList(this->mBlockDataList);
    emit updateBlockCodesFinished();
    this->quit();
}

void QEastMoneyBlockThread::run()
{
    GetBlockShares();
}



