#include "clsDBCreateTables.h"
#include <QSqlQuery>
#include <QDebug>
#include "clsDBOp.h"
#include <QObject>
#include <QMapIterator>
#include <QTime>

clsDBCreateTables::clsDBCreateTables(QObject *parent) :QObject(parent)
{

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
    QSqlQuery q;
    QString sql=QString("CREATE TABLE IF NOT EXISTS %1 ("
                        "StNumber	NUMERIC NOT NULL UNIQUE,"
                        "Hi	NUMERIC,"
                        "Lo	NUMERIC,"
                        "O	NUMERIC,"
                        "C	NUMERIC,"
                        "LC	NUMERIC,"
                        "MA20	NUMERIC,"
                        "MA60 NUMERIC,"
                        "WK20	NUMERIC,"
                        "PRIMARY KEY(StNumber))").arg(code);

    q.exec(sql);
}

void clsDBCreateTables::fillCodeTable(QString code)
{

}
