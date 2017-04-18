#include "clsSettings.h"
#include <QDebug>
#include <QStringList>
clsSettings::clsSettings(QObject *parent) :
    QObject(parent)
{
    configIniWrite = new QSettings(settingsFilePath, QSettings::IniFormat);
}

 bool clsSettings::FileExit()
{
   fileExit=QFile::exists("./Settings.ini");
   return fileExit;
}

void clsSettings::writeSetting(QString strPath, QVariant value)
{
    configIniWrite->setValue(strPath,value);
}

void clsSettings::readSetting(QString strPath, QVariant & value)
{
    value =configIniWrite->value(strPath);
}

void clsSettings::readSetting(QString strPath, int & value)
{
    value =configIniWrite->value(strPath).toInt();
}

void clsSettings::readSetting(QString strPath, double & value)
{
    value =configIniWrite->value(strPath).toDouble();
}
void clsSettings::readSetting(QString strPath, QString & value)
{
    value =configIniWrite->value(strPath).toString();
}

void clsSettings::readSetting(QString strPath, bool & value)
{
    value =configIniWrite->value(strPath).toBool();
}
void clsSettings::readSetting(QString strPath, QStringList & value)
{
    value =configIniWrite->value(strPath).toStringList();
}


