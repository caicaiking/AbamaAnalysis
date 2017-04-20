#include "clsDBCreateTables.h"
#include <QSqlQuery>
#include <QDebug>
#include "clsDBOp.h"
#include <QObject>
#include <QTime>

clsDBCreateTables::clsDBCreateTables(QObject *parent) :QObject(parent)
{

}

bool clsDBCreateTables::createStockTable()
{
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
            "blockName integer NOT NULL DEFAULT 0"
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

bool clsDBCreateTables::fillBlockTable()
{

}
