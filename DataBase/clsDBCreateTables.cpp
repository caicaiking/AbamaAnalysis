#include "clsDBCreateTables.h"
#include <QSqlQuery>
#include <QDebug>
#include "clsDBOp.h"
#include <QObject>
#include <QMapIterator>
#include <QTime>
#include <QMutex>
#include <QMutexLocker>
QMutex clsDBCreateTables::m_operation;
QMutex clsDBCreateTables::m_detail;

clsDBCreateTables::clsDBCreateTables(QObject *parent) :QObject(parent)
{

}


bool clsDBCreateTables::createUpdateTable()
{
    //emit showMessage(tr("创建更新表格"));

    QMutexLocker locker(&m_operation);

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
    QMutexLocker locker(&m_operation);
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
    QMutexLocker locker(&m_detail);
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

bool clsDBCreateTables::insertFavorite(QString code, QString detail)
{
    QMutexLocker locker(&m_detail);
    QStringList res = getFavorite(code);

    QString sql;

    if(res.length()==0)
    {
        sql = QString("insert into FAVORITE values('%1','%2')").arg(code).arg(detail);
    }
    else
    {
        sql = QString("update Favorite set comment ='%1' where codes ='%2'").arg(detail).arg(code);
    }

    QSqlQuery q;

    if(!clsDBOp::instance()->isOpen())
        return false;

    q.exec(sql);
    if(!q.isActive())
        return false;
    return true;

}

bool clsDBCreateTables::deleteFavorite(QString code)
{

    QMutexLocker locker(&m_detail);
    QStringList res = getFavorite(code);

    QString sql;

    if(res.length()==0)
    {
        return true;
    }
    else
    {
        sql = QString("delete from Favorite  where codes ='%2'").arg(code);
    }

    QSqlQuery q;

    if(!clsDBOp::instance()->isOpen())
        return false;

    q.exec(sql);
    if(!q.isActive())
        return false;
    return true;
}

QStringList clsDBCreateTables::getFavorite(QString code)
{
    QMutexLocker locker(&m_operation);

    QString sql;
    if(code.isEmpty())
        sql = "Select codes from favorite";

    else
        sql = QString("SELECT codes, comment FROM FAVORITE WHERE CODES = '%1' ").arg(code);

    QSqlQuery q;

    if(!clsDBOp::instance()->isOpen())
        return QStringList();

    q.exec(sql);
    if(q.isActive())
    {
        QStringList tmp;
        if(!code.isEmpty())
        {
            while(q.next())
            {
                tmp<<q.value(0).toString();
                tmp << q.value(1).toString();
            }
        }
        else
        {
            while(q.next())
            {
                tmp<<q.value(0).toString();
            }
        }

        return tmp;
    }
    return QStringList();
}

bool clsDBCreateTables::createFavoriteTable()
{
    QMutexLocker locker(&m_operation);

    QString sql = "CREATE TABLE IF NOT EXISTS favorite "
                  "("
                  "codes text  NOT NULL default '', "
                  "comment text NOT NULL DEFAULT '' "
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


bool clsDBCreateTables::createBlockTable()
{
    QMutexLocker locker(&m_operation);
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
    QMutexLocker locker(&m_operation);
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
    QMutexLocker locker(&m_operation);
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
    QMutexLocker locker(&m_operation);
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

//get all stock codes from database
QStringList clsDBCreateTables::getStockCodes()
{
    QMutexLocker locker(&m_operation);

    QString sql = QString("select * from details");
    QSqlQuery q;

    if(!q.exec(sql))
        return QStringList();

    QStringList tmp;
    while(q.next())
    {
        tmp.append(q.value(0).toString());
    }

    return tmp;
}



void clsDBCreateTables::fillDetailTable(const QStringList mStkCodesList)
{
    QMutexLocker locker(&m_operation);

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
    QMutexLocker locker(&m_detail);
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
    QMutexLocker locker(&m_detail);

    QString date = t.toString("yyyy-MM-dd");

    if(date  == getLatestUpdate())
        return ;
    QString sql = QString("INSERT INTO updateRecord  (updatetime) values ('%1')").arg(date);

    QSqlQuery q;
    q.exec(sql);
}

QString clsDBCreateTables::getLatestUpdate()
{
    QMutexLocker locker(&m_operation);

    QString sql = "select updateTime from updateRecord order by updateTime desc";
    QSqlQuery q;
    if(q.exec(sql))
    {
        if(q.next())
            return q.value(0).toString();
    }

    return "2010-12-31";
}

QDate clsDBCreateTables::getCodeLatestDate(QString code)
{
    QMutexLocker locker(&m_operation);

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
            return QDate(2010,12,31);
        }
    }
    else
    {
        return QDate(2010,12,31);
    }

}

//Get one stock data from the data base.
SingleStockDataList clsDBCreateTables::getStockData(QString strCode)
{
    QMutexLocker locker(&m_operation);

    SingleStockDataList data;
    data.clear();

    QSqlQuery q;

    QString strSql = QString("Select * from %1 order by STDATE desc").arg(strCode);


    if(q.exec(strSql))
    {
        while(q.next())
        {
            SingleStockData tmp;

            tmp.date = q.value(0).toString();
            tmp.open = q.value(1).toDouble();
            tmp.close = q.value(2).toDouble();
            tmp.zd = q.value(3).toDouble();
            tmp.zdf = q.value(4).toDouble();
            tmp.low = q.value(5).toDouble();
            tmp.high = q.value(6).toDouble();
            tmp.cjl = q.value(7).toDouble();
            tmp.cje = q.value(8).toDouble();
            tmp.hsl = q.value(9).toDouble();

            data.append(tmp);
        }
        return data;
    }
    else
    {
        return data;
    }
}

/*!
 * \brief fill data to the code sheet. with the content.
 * \param code stock code
 * \param list the data
 */
void clsDBCreateTables::fillCodeTable(QString code, SingleStockDataList list)
{
    QMutexLocker locker(&m_operation);

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

