#include "clsBollin2.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include "clsGetLastWorkDay.h"
#include "clsShowStockUi.h"
#include "chartdir.h"
#include <QDebug>


clsBollin2::clsBollin2(QObject *parent): clsStrategy(parent)
{
    db = new clsDBCreateTables(this);

    m_timeStamps = new double[0];
    m_highData = new double[0];
    m_lowData = new double[0];
    m_openData = new double[0];
    m_closeData = new double[0];
    m_volData = new double[0];
}

QStringList clsBollin2::findStockCodes()
{
    showProgress(tr("正在获取最后一个交易日日期"));
    QString workDay =clsGetLastWorkDay::getLastWorkDate(
                QDateTime::currentDateTime().addSecs(-1*18*60*60).date()).toString("yyyy-MM-dd");


    QStringList codes;
    if(lastCode.isEmpty())
        codes= db->getStockCodes();
    else
        codes = lastCode;

    QStringList stockCodes;
    int i=0;
    foreach (QString strCode, codes) {
        i++;

        showProgress(QString("现在进度 %1/%2--找到：%3股票.")
                     .arg(i).arg(codes.length()).arg(stockCodes.length()));

        qApp->processEvents();

        SingleStockDataList res = db->getStockData(strCode);
        if(res.length()< 60)
            continue;

        if(QDate::currentDate().toString("yyyy-MM-dd")> workDay)
        {
            if(res.first().date < workDay)
                continue;
        }
        else
        {
            if(res.first().date < QDateTime::currentDateTime().addSecs(-1*18*60*60).date().toString("yyyy-MM-dd"))
                continue;
        }

        if(res.first().open > res.first().close)
            continue;

        delete[] m_timeStamps;
        delete[] m_highData;
        delete[] m_lowData;
        delete[] m_openData;
        delete[] m_closeData;
        delete[] m_volData;

        int length =60;
        m_timeStamps = new double[length];
        m_highData = new double[length];
        m_lowData = new double[length];
        m_openData = new double[length];
        m_closeData = new double[length];
        m_volData = new double[length];

        for(int j=0;j<length;j++)
        {
            m_timeStamps[j]=  clsShowStockUi::QDateTimeToChartTime(QDateTime(res[length-j-1].getDate()));
            m_highData[j] = res.at(length-j-1).high;
            m_lowData[j] = res.at(length-j-1).low;
            m_openData[j] = res.at(length-j-1).open;
            m_closeData[j] = res.at(length-j-1).close;
            m_volData[j] =  res.at(length-j-1).cjl;
        }


        DoubleArray closeData(m_closeData,length);
        ArrayMath stdDev = ArrayMath(closeData).movStdDev(20).mul(2.0);
        ArrayMath movAvg = ArrayMath(closeData).movAvg(20);

        DoubleArray daTmp;
        daTmp=ArrayMath(movAvg).add(stdDev);

        DoubleArray upperLine;

        deepCopy(upperLine,daTmp);
        DoubleArray lowerLine;
        daTmp= ArrayMath(movAvg).sub(stdDev).selectGTZ();
        deepCopy(lowerLine,daTmp);



        int ii = upperLine.len -1;
        if(ii >= lowerLine.len)
        {
            ii = lowerLine.len -1;
        }

        double lowValue = lowerLine[ii];
        double highValue = upperLine[ii];

        if((res.first().close < lowValue)
                &&(res.first().open < res.first().close)
                && res.first().hsl*100.0 >=this->hsl)
        {
            qDebug()<< res.first().hsl;

            stockCodes.append(strCode);
        }
        else
            continue;

        qApp->processEvents();

    }
    showProgress(QString("查找已经完毕-共计 %1 股票").arg(stockCodes.length()));

    return stockCodes;
}

void clsBollin2::setCondition(QString condition)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(condition.toUtf8(),
                                                &error);

    if(error.error == QJsonParseError::NoError)
    {
        if(doc.isObject())
        {
            QJsonObject obj = doc.object();

            this->average = obj.value("average").toInt();
            this->hsl = obj.value("hsl").toInt();
            this->lastCode = obj.value("stocks").toString().split(",", QString::SkipEmptyParts);
        }
    }
}

void clsBollin2::deepCopy(DoubleArray &dest, DoubleArray src)
{
    if (src.len > dest.len)
    {
        delete[] const_cast<double *>(dest.data);
        dest.data = new double[src.len];
    }
    memcpy(const_cast<double *>(dest.data), src.data, src.len * sizeof(double));
    dest.len = src.len;
}
