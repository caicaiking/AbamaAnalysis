#include "clsGetLastWorkDay.h"
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

clsGetLastWorkDay::clsGetLastWorkDay(QObject *parent) : QObject(parent)
{

}

QDate clsGetLastWorkDay::getLastWorkDate(QDate day)
{
    QString strDay = day.toString("yyyyMMdd");
    QString wkURL = QString("http://www.easybots.cn/api/holiday.php?d=%1")
            .arg(strDay);

    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    QNetworkReply *reply  = mgr->get(QNetworkRequest(wkURL));

    if(!reply)
    {
        return day;
    }
    QEventLoop loop; // 使用事件循环使得网络通讯同步进行
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec(); // 进入事件循环， 直到reply的finished()信号发出， 这个语句才能退出
    if(reply->error())
    {

        reply->deleteLater();
        qDebug()<<"Get work day data erorr!";
        return day;
    }

    QByteArray bytes;

    bytes = reply->readAll();
    //qDebug()<<bytes;
    if(bytes.length() >0)
    {
        QString myData =QString(bytes);

        //qDebug()<< myData;
        QJsonParseError error;
        QJsonDocument jsDocument = QJsonDocument::fromJson(myData.toUtf8(),&error);

        if(error.error == QJsonParseError::NoError)
        {
            if(!(jsDocument.isNull() || jsDocument.isEmpty()))
            {
                if(jsDocument.isObject())
                {
                    QJsonObject obj = jsDocument.object();

                    int res = obj.value(strDay).toString().toInt();

                    if(res==0 && (day.dayOfWeek() != 6) && (day.dayOfWeek()!=7))
                        return day;
                    else
                        return getLastWorkDate(day.addDays(-1));

                }
            }

        }
    }

    return day;


}
