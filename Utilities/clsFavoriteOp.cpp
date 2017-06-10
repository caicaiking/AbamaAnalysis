#include "clsFavoriteOp.h"

clsFavoriteOp::clsFavoriteOp(QObject *parent):QObject(parent)
{
    db = new clsDBCreateTables(this);

}

QStringList clsFavoriteOp::showFavorite(QString code)
{
    return db->getFavorite(code);
}

bool clsFavoriteOp::addFavorite(QString code, QString comment)
{
    return db->insertFavorite(code,comment);
}

bool clsFavoriteOp::deleteFavorite(QString code)
{
    return db->deleteFavorite(code);
}
