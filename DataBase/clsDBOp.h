#ifndef CLSDBOP_H
#define CLSDBOP_H

#include <QObject>
#include <QSqlDatabase>
class clsDBOp : public QObject
{
    Q_OBJECT

private:
    explicit clsDBOp(QObject *parent = 0);
public:
    ~clsDBOp();

    static clsDBOp *instance();
    bool isOpen() const;

    QSqlDatabase getDb() const;

signals:

public slots:

private:
   static clsDBOp *mInstance;

   class clsRmDBOp // 它的唯一工作就是在析构函数中删除CSingleton的实例
   {
   public:
       ~clsRmDBOp()
       {
           if (clsDBOp::mInstance)
               delete clsDBOp::mInstance;
       }
   };
   static clsRmDBOp rmDbOp; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

   QSqlDatabase db;


};

#endif // CLSDBOP_H
