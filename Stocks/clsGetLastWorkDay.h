#ifndef CLSGETLASTWORKDAY_H
#define CLSGETLASTWORKDAY_H

#include <QObject>

class clsGetLastWorkDay : public QObject
{
    Q_OBJECT
public:
    explicit clsGetLastWorkDay(QObject *parent = 0);

    static QDate getLastWorkDate(QDate day);

signals:

public slots:
};

#endif // CLSGETLASTWORKDAY_H
