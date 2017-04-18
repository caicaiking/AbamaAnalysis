#ifndef USERFULFUNCTION_H
#define USERFULFUNCTION_H

#include <QtGui>
#include <QList>
class UsefulFunction
{
public:
    UsefulFunction();
    static QString getMD5(QString passwd);

    static QString getDataFilePath();
    static void setDataFilePath(QString path);

    static bool createConnection();
    static bool createUserTable();
    static bool addAdminCount();

    static bool testUserNameAndPasswd(const QString userName, const QString passwd);
    static bool updateLoginTime(const QString userName);
    static int getPriority(const QString userName);
    static QStringList getUserNames();

    static bool setPassword(QString userName, QString passwd);
    static bool testUserName(QString userName);
    static bool addUser(QString userName, QString passwd, int priority);
    static bool delUser(QString userName);
    static bool createCategoryTable();
    static bool creatCategoryDateilTable();
    static bool createMainSheet();

    static QMap<QString, int> getCategoryTable();
    static bool insertCategoryTable(QString name, int id);
    static bool delCategoryItem(QString catName);
    static bool delCategoryItem(int catId);
    static int getMaxCategoryTableId();
    static int getCategoryTableId(QString catName);
    static bool insertMainSheet(int catId, int catDetailId, double money, int userId, int inOrOut, QString spendDate, QString spendTime, QString recordDate, QString recordTime, QString mark, QString des);

    static bool insterCategoryDetail(QString detailName, int detailId,int catId);
    static QList<QString> getCategoryDetailNames();

    static QMap<QString,int> getCategoryDetailTableItems(QString categoryName);

    static QList<int> getCategoryDetailIds(int catId);
    static QStringList getCategoryTableNames();
    static int getCategoryDetailId(QString catDetail, int catId);
    static int getUserId(const QString userName);
    static bool delCategoryDetailIItem(QString catDetName, int catId);
    static QList<QStringList> fillMainTable(int userId);
    static QList<QString> getCategoryDetailNames(int cat);
    static QStringList listToStringList(QList<QString> i);
    static QList<QString> getDesc();
    static QList<QDate> getDateRange();
    static QList<QStringList> fillMainTable(QString sqlCondition);
    static bool deleteMainSheet(int id);
    static QStringList getMainSheetRecord(int id);
    static QString getCategoryTableName(int id);
    static QString getCategoryDetailName(int detailId);
    static bool updateMainSheet(int catId, int catDetailId, double myMoney, int inOrOut, QString spendDate, QString spendTime, QString recordDate, QString recordTime, QString myMark, QString myDesc, int id);
    static int getMaxCategoryDetailTableId();
    static double getSumMainSheet(int userId, int catId, QString startDate, QString stopDate);
    static double getSumMainSheet(int userId, int catId, int catDetailId, QString startDate, QString stopDate);
    static double getSumMainSheet(int userId, QString startDate, QString stopDate);
};

#endif // USERFULFUNCTION_H
