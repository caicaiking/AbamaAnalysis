#include "UserfulFunction.h"
#include <QtSql>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include "clsSettings.h"
#include <QFileDialog>
#include <QObject>
UsefulFunction::UsefulFunction()
{
}


//生成md5加密密码
QString UsefulFunction::getMD5(QString passwd)
{
    QString md5;
    QByteArray bb;
    bb = QCryptographicHash::hash ( passwd.toStdString().c_str(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    return md5;
}

QString UsefulFunction::getDataFilePath()
{
    clsSettings settings;
    QString strNode ="DataFile/";
    QString tmpString;
    settings.readSetting(strNode+"Path",tmpString);

    if(tmpString.isEmpty())
    {
        QString dataFile =QFileDialog::getOpenFileName(0,QObject::tr("打开数据库文件"),
                                                       "./",QObject::tr("数据文件 (*.dat)"));
        if(dataFile.isEmpty())
            qApp->quit();
        setDataFilePath(dataFile);
        return dataFile;
    }
    else
        return tmpString;
}

void UsefulFunction::setDataFilePath(QString path)
{
    clsSettings settings;
    QString strNode ="DataFile/";

    settings.writeSetting(strNode+"Path",path);
}

//创建数据库连接
bool UsefulFunction::createConnection()
{
    QString dataFileName = getDataFilePath();
    QFileInfo fi(dataFileName);
    bool exits =fi.exists ();

    if(!exits)
        qDebug()<<"数据库文件不存在";

    QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");
    db.setDatabaseName(dataFileName);
    if(!db.open ())
    {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;

    }
    return true;

}

//创建用户 userinfo 表格
bool UsefulFunction::createUserTable()
{
    QString sql ="CREATE TABLE IF NOT EXISTS userinfo "
                 "("
                 " id integer PRIMARY KEY NOT NULL , "
                 " username text  NOT NULL DEFAULT '' , "
                 " passwd text  NOT NULL DEFAULT '' ,"
                 " lastlogin text not null default '',"
                 " Priority integer not null default 5"
                 " ) ;";

    QSqlQuery q;
    q.exec(sql);
    if(!q.isActive())
    {
        qDebug()<<"Create UserInfo table failed!";
        return false;
    }
    // qDebug()<<"创建 UserInfo 表成功";

    addAdminCount();
    return true;

}

//第一次创建表格添加Admin账户
bool UsefulFunction::addAdminCount()
{
    QString sql ="Select * from userinfo";
    QSqlQuery q;


    if(!q.exec(sql))
        return false;

    if(!q.next())
    {
        sql =QString("INSERT INTO userinfo (  username ,  passwd ,  lastlogin,  Priority) "
                     "VALUES (  'admin' ,'%1' ,  '%2 %3',0 ) ").arg(getMD5("admin"))
                .arg(QDate::currentDate().toString("yyyy-MM-dd"))
                .arg(QTime::currentTime().toString("hh:mm:ss"));

        if(!q.exec(sql))
            return false;
        else
        {
            qDebug()<< "添加Admin账户密码成功！";
            return true;
        }
    }

    return true;
}

//验证用户名和密码
bool UsefulFunction::testUserNameAndPasswd(const QString userName, const QString passwd)
{
    QSqlQuery q;
    QString sql=QString("select passwd from userinfo where username ='%1';").arg(userName);

    if(!q.exec(sql))
        return false;

    if (q.next())
    {
        QString dbPasswd = q.value(0).toString();
        if(getMD5(passwd) == dbPasswd)
            return true;
        else
            return false;
    }
    else
        return false;

}

//更新登录时间
bool UsefulFunction::updateLoginTime(const QString userName)
{
    QSqlQuery q;
    QString sql=QString("update userinfo set lastlogin ='%1' where username ='%2'")
            .arg(QDate::currentDate().toString("yyyy-MM-dd")
                 +" "
                 + QTime::currentTime().toString("hh:mm:ss"))
            .arg(userName);

    if(!q.exec(sql))
        return false;
    else
        return true;

}

//获取用户权限
int UsefulFunction::getPriority(const QString userName)
{
    QSqlQuery q;
    QString sql=QString("select priority from userinfo where  username ='%1';").arg(userName);

    if(!q.exec(sql))
        return false;

    if (q.next())
    {
        bool ok;
        int priority = q.value(0).toInt(&ok);
        if(ok)
            return priority;
        else
            return 5;
    }
    return 5;
}

int UsefulFunction::getUserId(const QString userName)
{
    QSqlQuery q;
    QString sql=QString("select id from userinfo where  username ='%1';").arg(userName);

    if(!q.exec(sql))
        return 5;

    if (q.next())
    {
        bool ok;
        int id = q.value(0).toInt(&ok);
        if(ok)
            return id;
        else
            return 5;
    }
    return 5;
}


//获取用户名
QStringList UsefulFunction::getUserNames()
{
    QSqlQuery q;
    QString sql=QString("select username from userinfo;");

    if(!q.exec(sql))
        return QStringList();

    QStringList userName;
    while(q.next())
        userName.append(q.value(0).toString());

    return userName;
}

//设置用户名的密码
bool UsefulFunction::setPassword(QString userName, QString passwd)
{
    QSqlQuery q;
    QString sql=QString("update userinfo set passwd ='%1' where username ='%2';")
            .arg(getMD5(passwd)).arg(userName);

    if(!q.exec(sql))
        return false;
    return true;
}

//测试用户名称是否存在
bool UsefulFunction::testUserName(QString userName)
{

    QSqlQuery q;
    QString sql=QString("select id from userinfo where username ='%1';").arg(userName);

    if(!q.exec(sql))
        return true;

    if(q.next())
        return false;
    else
        return true;
}

//添加用户。
bool UsefulFunction::addUser(QString userName,QString passwd, int priority)
{
    QSqlQuery q;
    QString   sql =QString("INSERT INTO userinfo (  username ,  passwd ,  lastlogin,  Priority) "
                           "VALUES (  '%1' ,'%2' ,  '%3 %4',%5 ) ")
            .arg(userName)
            .arg(getMD5(passwd))
            .arg(QDate::currentDate().toString("yyyy-MM-dd"))
            .arg(QTime::currentTime().toString("hh:mm:ss"))
            .arg(priority);

    if(!q.exec(sql))
        return false;
    else
    {
        qDebug()<< "添加账户密码成功！";
        return true;
    }
    return true;
}

bool UsefulFunction::delUser(QString userName)
{
    QSqlQuery q;
    QString sql=QString("delete from userinfo where username ='%1';").arg(userName);

    if(!q.exec(sql))
        return false;
    else
        return true;
}

//创建类型表
bool UsefulFunction::createCategoryTable()
{
    QSqlQuery q;
    QString sql = QString("CREATE TABLE IF NOT EXISTS category "
                          "( "
                          "catName text PRIMARY KEY NOT NULL DEFAULT '' unique , "
                          "catid integer NOT NULL DEFAULT 0 unique "
                          " ) ;");

    if(!q.exec(sql))
        return false;
    else
        return true;
}

//创建详细类型表
bool UsefulFunction::creatCategoryDateilTable()
{
    QSqlQuery q;
    QString sql = QString("CREATE TABLE IF NOT EXISTS categoryDetail "
                          "( "
                          "catDetail text  NOT NULL DEFAULT '' , "
                          "detailId integer PRIMARY KEY  NOT NULL DEFAULT 0 , "
                          "catid integer NOT NULL DEFAULT 0 "
                          " ) ;");

    if(!q.exec(sql))
        return false;
    else
        return true;
}

//创建主表
bool UsefulFunction::createMainSheet()
{
    QSqlQuery q;
    QString sql=QString("CREATE TABLE IF NOT EXISTS mainsheet "
                        "( "
                        "id integer NOT NULL PRIMARY KEY , "
                        "catId integer NOT NULL DEFAULT 0 ,"
                        "catDetailId integer NOT NULL DEFAULT 0 ,"
                        "mymoney double NOT NULL DEFAULT 0.0 , "
                        " userId integer NOT NULL DEFAULT 0, "
                        "inorout integer NOT NULL DEFAULT 0 , "
                        "spenddate text NOT NULL DEFAULT '2014-01-01' , "
                        "spendtime text NOT NULL DEFAULT '12:00:00' , "
                        "recordDate text NOT NULL DEFAULT '2014-01-01' , "
                        "recordTime text NOT NULL DEFAULT '12:00:00' , "
                        "myMark text NOT NULL DEFAULT '' ,"
                        "myDesc text NOT NULL DEFAULT '' "
                        ") ;");

    if(!q.exec(sql))
        return false;
    else
        return true;
}

//插入主表
bool UsefulFunction::insertMainSheet(int catId, int catDetailId, double money, int userId, int inOrOut, QString spendDate, QString spendTime,
                                     QString recordDate, QString recordTime, QString mark, QString des)
{
    QSqlQuery q;
    QString sql= QString("INSERT INTO mainsheet ( catId,catDetailId , mymoney , userId , inorout , spenddate , spendtime , recorddate , recordtime ,mymark,mydesc ) "
                         "VALUES "
                         "(%0, %1,%2 ,'%3' ,%4,'%5' ,'%6' ,'%7' ,'%8' , '%9' , '%10') ;")
            .arg(catId)
            .arg(catDetailId).arg(money).arg(userId).arg(inOrOut).arg(spendDate)
            .arg(spendTime).arg(recordDate).arg(recordTime).arg(mark).arg(des);

    //qDebug()<< sql;

    if(!q.exec(sql))
        return false;
    else
        return true;
}

//插入CategoryDetail表
bool UsefulFunction::insterCategoryDetail(QString detailName, int detailId, int catId)
{
    QSqlQuery q;
    QString sql= QString("insert into categorydetail (catdetail, detailId,catid) values('%1',%2,%3);")
            .arg(detailName).arg(detailId).arg(catId);
    if(!q.exec(sql))
        return false;
    else
        return true;
}

//获取整个category 表
QMap<QString, int> UsefulFunction::getCategoryTable()
{
    QSqlQuery q;
    QString sql= QString("select catname, catid from category  order by catId;");

    QMap<QString,int> categoryTable;
    if(!q.exec(sql))
        return categoryTable;

    while(q.next())
    {
        QString catName = q.value("catName").toString();
        int catId = q.value("catid").toInt();
        categoryTable.insert(catName,catId);
    }
    return categoryTable;
}

//获取整个category 表
QStringList UsefulFunction::getCategoryTableNames()
{
    QSqlQuery q;
    QString sql= QString("select catname, catid from category  order by catId;");

    QStringList l;
    l.clear();
    if(!q.exec(sql))
        return l;

    while(q.next())
    {
        QString catName = q.value("catName").toString();
        l.append(catName);
    }
    return l;
}

//获取整个category 表
QString UsefulFunction::getCategoryTableName(int id)
{
    QSqlQuery q;
    QString sql= QString("select catname from category where catid=%1  order by catId;").arg(id);
    //qDebug()<<sql;
    QString l;
    if(!q.exec(sql))
        return l;

    while(q.next())
    {
        QString catName = q.value("catName").toString();
        return catName;
    }
    return l;
}

QString UsefulFunction::getCategoryDetailName(int detailId)
{
    QSqlQuery q;
    QString sql= QString("select catdetail from CATEGORYDETAIL where detailid=%1;").arg(detailId);
    //qDebug()<<sql;
    QString l;
    if(!q.exec(sql))
        return l;

    while(q.next())
    {
        QString catName = q.value(0).toString();
        return catName;
    }
    return l;
}






//插入类型表项目
bool UsefulFunction::insertCategoryTable(QString name, int id)
{
    QSqlQuery q;
    QString sql= QString("insert into category (catname, catid) values('%1',%2);").arg(name).arg(id);
    if(!q.exec(sql))
        return false;
    else
        return true;
}

//删除类型表项目
bool UsefulFunction::delCategoryItem(int catId)
{
    QSqlQuery q;
    QString sql= QString("delete from category where catid =%1;").arg(catId);
    if(!q.exec(sql))
        return false;
    else
        return true;
}

//删除类型表项目
bool UsefulFunction::delCategoryItem(QString  catName)
{
    QSqlQuery q;
    QString sql= QString("delete from category where catName ='%1';").arg(catName);
    if(!q.exec(sql))
        return false;
    else
        return true;
}

//获取Category表最大Id
int UsefulFunction::getMaxCategoryTableId()
{
    QSqlQuery q;
    QString sql= QString("select max(catid) from category;");
    if(!q.exec(sql))
        return 0;

    else
    {
        if(q.next())
            return q.value(0).toInt();

    }
    return 0;
}


//获取Category表最大Id
int UsefulFunction::getMaxCategoryDetailTableId()
{
    QSqlQuery q;
    QString sql= QString("select max(detailid) from categorydetail;");
    if(!q.exec(sql))
        return 0;

    else
    {
        if(q.next())
            return q.value(0).toInt();

    }
    return 0;
}


double UsefulFunction::getSumMainSheet(int userId, QString startDate, QString stopDate)
{
    QSqlQuery q;
    QString sql= QString("SELECT "
                         "sum( mymoney ) "
                         "FROM "
                         "mainsheet "
                         "WHERE"
                         " userid = %1 AND spenddate >= '%2' AND spenddate <= '%3';")
            .arg(userId).arg(startDate).arg(stopDate);

    // qDebug()<<sql;

    if(!q.exec(sql))
        return 0;
    else
    {
        if(q.next())
            return q.value(0).toDouble();

    }
    return 0;

}




double UsefulFunction::getSumMainSheet(int userId, int catId, QString startDate, QString stopDate)
{
    QSqlQuery q;
    QString sql= QString("SELECT "
                         "sum( mymoney ) "
                         "FROM "
                         "mainsheet "
                         "WHERE"
                         " userid = %1 AND spenddate >= '%3' AND spenddate <= '%4' and catid =%2;")
            .arg(userId).arg(catId).arg(startDate).arg(stopDate);

    // qDebug()<<sql;

    if(!q.exec(sql))
        return 0;
    else
    {
        if(q.next())
            return q.value(0).toDouble();

    }
    return 0;

}


double UsefulFunction::getSumMainSheet(int userId, int catId, int catDetailId, QString startDate, QString stopDate)
{
    QSqlQuery q;
    QString sql= QString("SELECT "
                         "sum( mymoney ) "
                         "FROM "
                         "mainsheet "
                         "WHERE"
                         " userid = %1 AND spenddate >= '%3' AND spenddate <= '%4' and catid =%2 and catdetailid = %5;")
            .arg(userId).arg(catId).arg(startDate).arg(stopDate).arg(catDetailId);

    // qDebug()<<sql;

    if(!q.exec(sql))
        return 0;
    else
    {
        if(q.next())
            return q.value(0).toDouble();

    }
    return 0;

}


//查找Category catId
int UsefulFunction::getCategoryTableId(QString catName)
{
    QSqlQuery q;
    QString sql= QString("select catid from category where catName ='%1';").arg(catName);
    if(!q.exec(sql))
        return 0;

    else
    {
        if(q.next())
            return q.value(0).toInt();

    }
    return 0;
}





QList<int> UsefulFunction::getCategoryDetailIds(int catId)
{
    QSqlQuery q;
    QList<int> l;
    l.clear();

    QString sql= QString("select detailId from categorydetail where catid=%1;").arg(catId);
    if(!q.exec(sql))
        return l;

    else
    {
        while(q.next())
            l.append(q.value(0).toInt());

    }
    return l;
}

int UsefulFunction::getCategoryDetailId(QString catDetail,int catId)
{
    QSqlQuery q;



    QString sql= QString("select detailId from categorydetail where catid=%1 and catDetail='%2';")
            .arg(catId).arg(catDetail);
    if(!q.exec(sql))
        return 0;

    else
    {
        if(q.next())
            return(q.value(0).toInt());

    }
    return 0;
}


//获取CategoryDetail Name 依照catid
QList<QString> UsefulFunction::getCategoryDetailNames(int cat)
{
    QSqlQuery q;
    QList<QString> l;
    l.clear();

    QString sql= QString("select catdetail from categorydetail where catid ='%1';").arg(cat);

    if(!q.exec(sql))
        return l;

    else
    {
        while(q.next())
            l.append(q.value(0).toString());

    }

    return l;
}

//获取CategoryDetail Name 依照catid
QList<QString> UsefulFunction::getCategoryDetailNames()
{
    QSqlQuery q;
    QList<QString> l;
    l.clear();

    QString sql= QString("select catdetail from categorydetail ;");

    if(!q.exec(sql))
        return l;

    else
    {
        while(q.next())
            l.append(q.value(0).toString());

    }

    return l;
}




//获取CategoryDetail catdetail, detailid
QMap<QString, int> UsefulFunction::getCategoryDetailTableItems(QString categoryName)
{

    QSqlQuery q;

    QMap<QString,int> l;
    l.clear();
    QString sql= QString("select catdetail, detailid from categorydetail"
                         " where catid =(select catid from category where catname='%1')")
            .arg(categoryName);
    if(!q.exec(sql))
        return l ;

    while(q.next())
    {
        QString cateDetail = q.value(0).toString();
        int detailId = q.value(1).toInt();

        l.insert(cateDetail,detailId);
    }
    return l;

}

bool UsefulFunction::delCategoryDetailIItem(QString catDetName, int catId)
{
    QSqlQuery q;
    QString sql= QString("delete from categorydetail where catid =%1 and catdetail='%2';")
            .arg(catId).arg(catDetName);
    if(!q.exec(sql))
        return false;
    else
        return true;
}

bool UsefulFunction::updateMainSheet(int catId,int catDetailId,double myMoney,
                                     int inOrOut,QString spendDate, QString spendTime,
                                     QString recordDate,QString recordTime,QString myMark,
                                     QString myDesc,int id)
{
    QSqlQuery q;
    QString sql = QString("update mainsheet set catid=%1,"
                          "catdetailid=%2,"
                          "mymoney=%3,"
                          "inorout=%4,"
                          "spenddate='%5',"
                          "spendtime='%6',"
                          "recorddate='%7',"
                          "recordtime='%8',"
                          "mymark='%9',"
                          "mydesc='%10'  "
                          "where id =%11").arg(catId).arg(catDetailId).arg(myMoney).arg(inOrOut)
            .arg(spendDate).arg(spendTime).arg(recordDate)
            .arg(recordTime).arg(myMark).arg(myDesc).arg(id);


    if(!q.exec(sql))
        return false;
    else
        return true;
}

QList<QStringList> UsefulFunction::fillMainTable(QString sqlCondition)
{
    QSqlQuery q;
    QString sql= QString("SELECT "
                         "id , "
                         " catdetail , "
                         " mymoney , "
                         " spenddate , "
                         " catname , "
                         " inorout , "
                         " mydesc , "
                         " recorddate, "
                         " mymark "
                         " FROM "
                         " ( "
                         " mainsheet LEFT JOIN "
                         "categorydetail ON mainsheet.catdetailid = categorydetail.detailid "
                         ") AS t1 LEFT JOIN "
                         "category ON category.catid = t1.catid "
                         " WHERE "
                         " %1 order by spenddate DESC , id desc;").arg(sqlCondition);


    //qDebug()<< sql;
    QList<QStringList>l;
    l.clear();
    if(!q.exec(sql))
        return l;

    while(q.next())
    {
        QStringList st;
        for(int i=0;i<9;i++)
        {
            if(i!=5)
                st.append(q.value(i).toString());
            else
            {
                st.append(q.value(i).toInt()==0?QObject::tr("支出"):QObject::tr("收入"));
            }
        }
        //   qDebug()<<st;
        l.append(st);
    }
    return l;
}

bool UsefulFunction::deleteMainSheet(int id)
{
    QSqlQuery q;
    QString sql= QString("delete from mainsheet where id ='%1'").arg(id);
    if(!q.exec(sql))
        return false;
    else
        return true;
}

QList<QStringList> UsefulFunction::fillMainTable(int userId)
{
    QSqlQuery q;
    QString sql= QString("SELECT "
                         "id , "
                         " catdetail , "
                         " mymoney , "
                         " spenddate , "
                         " catname , "
                         " inorout , "
                         " mydesc , "
                         " recorddate, "
                         " mymark "
                         " FROM "
                         " ( "
                         " mainsheet LEFT JOIN "
                         "categorydetail ON mainsheet.catdetailid = categorydetail.detailid "
                         ") AS t1 LEFT JOIN "
                         "category ON category.catid = t1.catid "
                         " WHERE "
                         "userid = %1 order by spenddate DESC ,id DESC;").arg(userId);


    //qDebug()<< sql;
    QList<QStringList>l;
    l.clear();
    if(!q.exec(sql))
        return l;

    while(q.next())
    {
        QStringList st;
        for(int i=0;i<9;i++)
        {
            if(i!=5)
                st.append(q.value(i).toString());
            else
            {
                st.append(q.value(i).toInt()==0?QObject::tr("支出"):QObject::tr("收入"));
            }
        }
        //   qDebug()<<st;
        l.append(st);
    }
    return l;
}

//将QList 转换成QStringList
QStringList UsefulFunction::listToStringList(QList<QString> i)
{
    QStringList l;
    l.clear();

    QListIterator<QString> it(i);
    while(it.hasNext())
    {
        l.append(it.next());
    }

    return l;
}

//获取目的地
QList<QString> UsefulFunction::getDesc()
{
    QSqlQuery q;
    QList<QString> l;
    l.clear();

    QString sql= QString("select mydesc from mainsheet;");

    if(!q.exec(sql))
        return l;

    else
    {
        while(q.next())
            l.append(q.value(0).toString());

    }

    return l;
}

QList<QDate> UsefulFunction::getDateRange()
{
    QSqlQuery q;
    QString sql =QString(" SELECT "
                         " min( spenddate ) , "
                         "  max( spenddate ) "
                         " FROM "
                         " mainsheet ;" );
    QList<QDate> l;
    l.clear();

    if(!q.exec(sql))
        return l;
    else
    {
        if(q.next())
        {
            QDate minDate = q.value(0).toDate();
            QDate maxDate = q.value(1).toDate();
            l.append(minDate);
            l.append(maxDate);
        }
    }
    return l;
}

QStringList UsefulFunction::getMainSheetRecord(int id)
{
    QSqlQuery q;
    QStringList l;
    l.clear();

    QString sql= QString("select  catid, catdetailid,mymoney,"
                         "inorout,spenddate,mydesc,myMark ,spendtime from mainsheet where id=%1")
            .arg(id);


    if(!q.exec(sql))
        return l;
    else
    {
        if(q.next())
        {
            l.append(q.value(0).toString());
            l.append(q.value(1).toString());
            l.append(q.value(2).toString());
            l.append(q.value(3).toString());
            l.append(q.value(4).toString());
            l.append(q.value(5).toString());
            l.append(q.value(6).toString());
            l.append(q.value(7).toString());
        }
    }

    return l;


}
