#ifndef CLSFAVORITEOP_H
#define CLSFAVORITEOP_H

#include "clsDBCreateTables.h"
#include "clsDBOp.h"
#include <QObject>
class clsFavoriteOp : public QObject
{
    Q_OBJECT
public:
    explicit clsFavoriteOp(QObject *parent =0);

    QStringList showFavorite(QString code="");
    bool addFavorite(QString code, QString comment);
    bool deleteFavorite(QString code);

private:
    clsDBCreateTables *db;
};

#endif // CLSFAVORITEOP_H
