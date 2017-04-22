#include "qeastmoneystockinfothread.h"
#include <QDateTime>
#include <QNetworkAccessManager>

#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include "stkmktcap.h"
#include "stockdata.h"
#include "stkinfofilemanage.h"
#include "clsSingleStockData.h"
#include "clsDBCreateTables.h"

qeastmoneystockinfothread::qeastmoneystockinfothread(QThread *parent) : QThread(parent)
{

}

qeastmoneystockinfothread::~qeastmoneystockinfothread()
{

}

void qeastmoneystockinfothread::setStockCodeList(const QStringList &codes)
{
    mStockcodeList = codes;
}

void qeastmoneystockinfothread::run()
{
//    MktCapFile::instance()->setValue("Update", "time", QDateTime::currentDateTime().toString("yyyy-MM-dd"));


    //下载前10天的数据，如果数据 下载 历史数据从2016年12月30开始
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    foreach (QString code, mStockcodeList) {

        SDList sdList;
        QString sdCode = code;

        sdList.clear();

        emit sendUpdateProgress(1, 1);
        bool update = true;

         clsDBCreateTables * db = new clsDBCreateTables();

        //查询数据库，找到最近的一天
        QDate lastUpdate = db->getCodeLatestDate(sdCode);

        if(lastUpdate == lastActiveDay())
            update = false;

        if(update == false) continue;
        //开始更新

        QDateTime start = QDateTime(lastUpdate);
        QDateTime end = QDateTime::currentDateTime();

        qDebug()<<"Start date: "<< start.toString("yyyy-MM-dd")<<"\tStop date: "<< end.toString("yyyy-MM-dd");
        code = code.right(6);
        if(code.left(1) == "6" || code.left(1) == "5")
        {
            code = "0" + code;
        } else
        {
            code = "1" + code;
        }
        //http://quotes.money.163.com/service/chddata.html?code=1000002&start=19910129&end=20161006&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;PCHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP
        QString wkURL = QString("http://quotes.money.163.com/service/chddata.html?"
                                "code=%1&start=%2&end=%3&"
                                "fields=TCLOSE;LCLOSE;VOTURNOVER;VATURNOVER;TCAP;MCAP;PCHG;TCLOSE;HIGH;LOW;TOPEN;LCLOSE")
                .arg(code).arg(start.toString("yyyyMMdd")).arg(end.toString("yyyyMMdd"));

        qDebug()<< "wkURL: \t" << wkURL;
        QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));
        if(!reply)
        {
            continue;
        }
        QEventLoop loop; // 使用事件循环使得网络通讯同步进行
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
        if(reply->error())
        {

            reply->deleteLater();
            continue;
        }
        //开始解析数据
        int index = -1;
        QByteArray bytes;


        while((bytes = reply->readLine()).length())
        {
            index++;
            if(index == 0) continue;
            QString result = QString::fromLocal8Bit(bytes.data());
            if( !result.isEmpty() )
            {
                QStringList blockInfoList = result.split(QRegExp("[,\"'\s\r\t]"), QString::SkipEmptyParts);

                if(blockInfoList.length() >=15)
                {

                    QString wkDate = blockInfoList[0];

                    clsSingleStockData tmpData;
                    tmpData.StNumber =wkDate;
                    tmpData.c=blockInfoList[10].toDouble();
                    tmpData.cje = blockInfoList[6].toDouble();
                    tmpData.cjl = blockInfoList[5].toDouble();
                    tmpData.Hi = blockInfoList[11].toDouble();
                    tmpData.lc = blockInfoList[14].toDouble();
                    tmpData.Lo = blockInfoList[12].toDouble();
                    tmpData.ltsz = blockInfoList[8].toDouble();
                    tmpData.o = blockInfoList[13].toDouble();
                    tmpData.zdf = blockInfoList[9].toDouble();
                    tmpData.zsz = blockInfoList[7].toDouble();

                    sdList.append(tmpData);

                }
            }

        }


        db->fillCodeTable(sdCode, sdList);
        reply->deleteLater();
    }
    qDebug()<<"stk info update finished";
    mgr->deleteLater();

    this->exit();

}

bool qeastmoneystockinfothread::isActiveDay(QDate date)
{
    if(date.dayOfWeek() == 6 || date.dayOfWeek() == 7) return false;
    return true;
}

QDate qeastmoneystockinfothread::lastActiveDay()
{
    QDate date = QDate::currentDate().addDays(-1);
    while(!isActiveDay(date))
    {
        date = date.addDays(-1);
    }
    qDebug()<<date;
    return date;
}


