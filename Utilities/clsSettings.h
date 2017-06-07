#ifndef CLSSETTINGS_H
#define CLSSETTINGS_H
#include <QSettings>
#include <QObject>

class clsSettings: public QObject
{
    Q_OBJECT
public:
    clsSettings(QObject *parent =0);

    bool FileExit();
    void writeSetting(QString strPath,QVariant value);
    void writeSetting(QString strPath, QList<double> value);
    void readSetting(QString strPath, QString &value);
    void readSetting(QString strPath, bool &value);
    void readSetting(QString strPath, QVariant &value);
    void readSetting(QString strPath, int &value);
    void readSetting(QString strPath, double &value);
    void readSetting(QString strPath, QList<double> &value);
    void readSetting(QString strPath, QStringList &value);
    void readSetting(QString strPath, QColor &value);

private:

    bool fileExit;
    QSettings *configIniWrite;
};

#endif // CLSSETTINGS_H
