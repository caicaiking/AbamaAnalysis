#ifndef CLSSTRATEGY_H
#define CLSSTRATEGY_H

#include <QObject>
#include <QStringList>

class clsStrategy: public QObject
{
    Q_OBJECT
public:
    explicit clsStrategy(QObject *parent =0);

    virtual QStringList findStockCodes()=0;
    virtual void setCondition(QString)=0;

signals:
    void showProgress(QString);

};

#endif // CLSSTRATEGY_H
