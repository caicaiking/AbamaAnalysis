#include "clsMaStrategy.h"

clsMaStrategy::clsMaStrategy(QObject *parent): clsStrategy(parent)
{

}

QStringList clsMaStrategy::findStockCodes()
{
    return QStringList();
}

void clsMaStrategy::setCondition(QString condition)
{
    this->condition = condition;
}

