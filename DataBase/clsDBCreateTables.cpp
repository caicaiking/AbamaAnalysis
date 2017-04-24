#include "clsDBCreateTables.h"
#include <QSqlQuery>
#include <QDebug>
#include "clsDBOp.h"
#include <QObject>
#include <QMapIterator>
#include <QTime>
#include <QMutex>
#include <QMutexLocker>
clsDBCreateTables::clsDBCreateTables(QObject *parent) :QObject(parent)
{

}



bool clsDBCreateTables::createUpdateTable()
{
    //emit showMessage(tr("创建更新表格"));

    QString sql = "CREATE TABLE IF NOT EXISTS updateRecord "
                  "("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "updateTime text NOT NULL DEFAULT '' "
                  ");";


    QSqlQuery q;

    if(!clsDBOp::instance()->isOpen())
        return false;

    q.exec(sql);
    if(!q.isActive())
    {
        // qDebug()<<"Create stocks table failed!";
        return false;
    }
    return true;
}

bool clsDBCreateTables::createStockTable()
{
    emit showMessage(tr("创建股票总表"));
    QString sql = "CREATE TABLE IF NOT EXISTS stocks "
                  "("
                  "id integer PRIMARY KEY NOT NULL, "
                  "blockName text NOT NULL DEFAULT '' "
                  ");";

    QSqlQuery q;

    if(!clsDBOp::instance()->isOpen())
        return false;

    q.exec(sql);
    if(!q.isActive())
    {
        // qDebug()<<"Create stocks table failed!";
        return false;
    }

    fillStockTable();
    return true;
}

void clsDBCreateTables::fillStockTable()
{
    QStringList stocks;
    QSqlQuery q;
    clsDBOp::instance()->getDb().transaction(); //开启事物处理

    stocks <<QObject::tr("地区")<<QObject::tr("行业")<<QObject::tr("概念");

    q.prepare("INSERT INTO stocks(id, blockName) "
              "VALUES(:id, :blockName)");

    for(int i=1; i<4; i++)
    {
        q.bindValue(":id",i);
        q.bindValue(":blockName",stocks.at(i-1));
        q.exec();
    }

    clsDBOp::instance()->getDb().commit(); //提交事物处理
}

bool clsDBCreateTables::createBlockTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS blocks "
                  "("
                  "id integer PRIMARY KEY NOT NULL, "
                  "details text NOT NULL DEFAULT '', "
                  "blockName integer NOT NULL DEFAULT 0, "
                  "codes text NOT NULL DEFAULT ''"
                  ");";

    QSqlQuery q;

    if(!clsDBOp::instance()->isOpen())
        return false;

    q.exec(sql);
    if(!q.isActive())
    {
        return false;
    }
    return true;
}

void clsDBCreateTables::getBlockTable( QMap<QString,BlockData>  &data)
{
    data.clear();

    QSqlQuery q;
    QString sql = "Select * from blocks order by id asc";

    if(!q.exec(sql))
    {
        return;
    }


    while(q.next())
    {
        BlockData tmp;
        tmp.code = q.value(0).toString();
        tmp.name =q.value(1).toString();
        tmp.stklist = q.value(3).toString().split(",",QString::SkipEmptyParts);
        data[tmp.code]= tmp;
    }



}


void clsDBCreateTables::fillBlockTable(const QMap<QString, BlockData> data)
{
    QMapIterator<QString, BlockData> it(data);
    QMap<QString, DetailSTR> data1;
    while(it.hasNext())
    {
        it.next();

        DetailSTR tmp;
        tmp.id = it.key().toInt();
        tmp.blockName = it.key().left(1).toInt();
        tmp.codes = it.value().stklist;
        data1[it.value().name] = tmp;
    }

    clsDBOp::instance()->getDb().transaction();
    QSqlQuery q;

    //查询已经在表中的记录
    q.exec("SELECT details from blocks");
    QStringList records;
    while (q.next()) {
        records.append(q.value(0).toString());
    }

    //准备插入语句

    emit showMessage(tr("写入股票板块数据"));

    QMapIterator<QString, DetailSTR> d(data1);
    while(d.hasNext())
    {
        d.next();

        if(!records.contains(d.key()))
        {
            q.prepare("INSERT INTO blocks (id, details, blockName, codes) "
                      "VALUES(:id,:de, :blockName, :codes) ");
            q.bindValue(":id",d.value().id);
            q.bindValue(":de",QString(d.key()));
            q.bindValue(":blockName",d.value().blockName);
            q.bindValue(":codes",QString(d.value().codes.join(",")));
            q.exec();

        }

        else
        {
            q.prepare("UPDATE blocks SET codes = :codes WHERE details = :details");
            q.bindValue(":codes",QString(d.value().codes.join(",")));
            q.bindValue(":details",QString(d.key()));
            q.exec();
        }

    }

    clsDBOp::instance()->getDb().commit();
}


bool clsDBCreateTables::createDetaiTable()
{
    emit showMessage(tr("创建股票代码一览表数据"));
    QString sql = "CREATE TABLE IF NOT EXISTS details "
                  "("
                  "codes text NOT NULL UNIQUE "
                  ");";


    QSqlQuery q;

    if(!clsDBOp::instance()->isOpen())
        return false;

    q.exec(sql);
    if(!q.isActive())
    {
        return false;
    }
    return true;
}

void clsDBCreateTables::fillDetailTable(const QStringList mStkCodesList)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    clsDBOp::instance()->getDb().transaction();

    QSqlQuery q;
    q.prepare("INSERT INTO details (codes) "
              "VALUES(:codes)");

    emit showMessage(tr("写入股票代码一览表数据"));
    for(int i=0; i< mStkCodesList.length(); i++)
    {
        q.bindValue(":codes",QString(mStkCodesList.at(i)));
        q.exec();
    }

    for(int i=0; i< mStkCodesList.length(); i++)
    {
        emit showMessage(tr("创建单独股票代码表：%1，%2/%3")
                         .arg(mStkCodesList.at(i))
                         .arg(i+1)
                         .arg(mStkCodesList.length()));
        createCodesTable(mStkCodesList.at(i));
    }

    clsDBOp::instance()->getDb().commit();
}

void clsDBCreateTables::createCodesTable(QString code)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    QSqlQuery q;
    QString sql=QString("CREATE TABLE IF NOT EXISTS %1 ("
                        "STDATE	TEXT NOT NULL UNIQUE,"
                        "O	NUMERIC,"
                        "C	NUMERIC,"
                        "ZD NUMERIC,"
                        "ZDF NUMERIC,"
                        "Lo	NUMERIC,"
                        "Hi	NUMERIC,"
                        "CJL	NUMERIC,"
                        "CJE NUMERIC,"
                        "HSL	NUMERIC,"
                        "PRIMARY KEY(STDATE))").arg(code);

    //qDebug()<< sql;
    q.exec(sql);
}

void clsDBCreateTables::setUpdateTime(QDate t)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString date = t.toString("yyyy-MM-dd");

    if(date  == getLatestUpdate())
        return ;
    QString sql = QString("INSERT INTO updateRecord  (updatetime) values ('%1')").arg(date);

    QSqlQuery q;
    q.exec(sql);
}

QString clsDBCreateTables::getLatestUpdate()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString sql = "select updateTime from updateRecord order by updateTime desc";
    QSqlQuery q;
    if(q.exec(sql))
    {
        if(q.next())
            return q.value(0).toString();
    }

    return "2015-12-31";
}

QDate clsDBCreateTables::getCodeLatestDate(QString code)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);

    QString sql = QString ("select STDATE from %1 order by STDATE  desc").arg(code);

    //qDebug()<< sql;
    QSqlQuery q;
    if(q.exec(sql))
    {
        if(q.next())
        {
            // qDebug()<<q.value(0).toString();
            return q.value(0).toDate();
        }
        else
        {
            return QDate(2015,12,31);
        }
    }
    else
    {
        return QDate(2015,12,31);
    }

}

void clsDBCreateTables::fillCodeTable(QString code, SingleStockDataList list)
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);


    clsDBOp::instance()->getDb().transaction();

    QSqlQuery q;

    QString sql = QString("INSERT INTO %1 (STDATE,O,C,ZD,ZDF,Lo,Hi,CJL,CJE,HSL) VALUES "
                          "('%2',%3,%4,%5,%6,%7,%8,%9,%10,%11)");

    for(int i =0; i<list.length(); i++)
    {
        QString sql2 = sql.arg(QString(code))
                .arg(QString(list.at(i).date))
                .arg( list.at(i).open)
                .arg(list.at(i).close)
                .arg(list.at(i).zd)
                .arg(list.at(i).zdf)
                .arg(list.at(i).low)
                .arg(list.at(i).high)
                .arg(list.at(i).cjl)
                .arg(list.at(i).cje)
                .arg(list.at(i).hsl);

        q.exec(sql2);
    }

    emit showMessage(tr("正在写表格：%1").arg(code));
    //qDebug()<<tr("正在写表格：%1").arg(code);
    clsDBOp::instance()->getDb().commit();

}

