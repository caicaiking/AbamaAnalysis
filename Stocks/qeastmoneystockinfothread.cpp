#include "qeastmoneystockinfothread.h"
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QJsonParseError>

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
    //下载 历史数据从2016年12月30开始
    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    foreach (QString code, mStockcodeList)
    {
        SingleStockDataList sdList;
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

       // qDebug()<<"Start date: "<< start.toString("yyyy-MM-dd")<<"\tStop date: "<< end.toString("yyyy-MM-dd");
        code = code.right(6);

        //http://q.stock.sohu.com/hisHq?code=cn_601766&start=20170101&end=20170422
        //使用搜狐的数据源， 发现163的不太好用。
        QString wkURL = QString ("http://q.stock.sohu.com/hisHq?"
                                 "code=cn_%1&start=%2&end=%3")
                .arg(code)
                .arg(start.toString("yyyyMMdd"))
                .arg(end.toString("yyyyMMdd"));


        //qDebug()<< "wkURL: \t" << wkURL;

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

        QByteArray bytes;

        bytes = reply->readAll();

        //qDebug()<<bytes;
        if(bytes.length() >0)
        {
            QString myData =QString(bytes);

            QJsonParseError error;
            QJsonDocument jsDocument = QJsonDocument::fromJson(myData.toUtf8(),&error);

            if(error.error == QJsonParseError::NoError)
            {
                if(!(jsDocument.isNull() || jsDocument.isEmpty()))
                {
                    if(jsDocument.isArray())
                    {
                        QJsonArray arr = jsDocument.array();

                        for(int i=0; i< arr.size(); i++)
                        {
                            int status;
                            QString code;
                            QJsonArray hq;
                            QJsonObject jv =arr.at(i).toObject();
                            status=jv["status"].toInt();
                            code =jv["code"].toString();
                            //qDebug()<<"Status "<< status << " code " <<code;

                            //qDebug()<<"Code "<<code;
                            hq =jv["hq"].toArray();
                            //qDebug()<<"hq "<<hq ;

                            for(int j=0; j< hq.size(); j++)
                            {
                                QJsonArray singleDay = hq.at(j).toArray();
                                if(singleDay.size()>=10)
                                {
                                    SingleStockData tmp;
                                    tmp.date = singleDay.at(0).toString();
                                    tmp.open = singleDay.at(1).toString().toDouble();
                                    tmp.close = singleDay.at(2).toString().toDouble();
                                    tmp.zd = singleDay.at(3).toString().toDouble();
                                    tmp.zdf= singleDay.at(4).toString().remove("%").toDouble()/100.00;
                                    tmp.low = singleDay.at(5).toString().toDouble();
                                    tmp.high = singleDay.at(6).toString().toDouble();
                                    tmp.cjl = singleDay.at(7).toString().toDouble();
                                    tmp.cje = singleDay.at(8).toString().toDouble();
                                    tmp.hsl = singleDay.at(9).toString().remove("%").toDouble()/100.0;
                                    sdList.append(tmp);
                                }
                            }
                        }
                    }
                }
            }

        }

        if(sdList.length()>0)
            db->fillCodeTable(sdCode, sdList);
        reply->deleteLater();
    }
   // qDebug()<<"stk info update finished";
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
    //qDebug()<<date;
    return date;
}
