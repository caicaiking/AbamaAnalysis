﻿#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QTextCodec>

class Profiles : public QObject
{
    Q_OBJECT
public:
    ~Profiles();
    static Profiles *instance();
    QStringList getAllSections();
private:
    explicit Profiles(QObject *parent = 0);
private:
    static Profiles     *minstance;
    class MGarbage // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~MGarbage()
        {
            if (Profiles::minstance)
                delete Profiles::minstance;
        }
    };
    static MGarbage Garbage; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

    QSettings           *configSettings;
    QSettings           *iniSettings;
public slots:
    void setValue( const QString & prefix, const QString & key, const QVariant & value );
    void setDefault( const QString & prefix, const QString & key, const QVariant & value );
    QVariant value( const QString & prefix,const QString &keys,const QVariant & defaultValue = QVariant() );

};
#endif // PROFILE_H
