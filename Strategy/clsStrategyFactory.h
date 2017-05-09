#ifndef CLSSTRATEGYFACTORY_H
#define CLSSTRATEGYFACTORY_H

#include "clsStrategy.h"
#include "clsWeekMa.h"
#include "clsMaStrategy.h"
#include "clsAttact.h"

enum STRATEGY{
    DayMa=0,
    WeekMa=1,
    Attack=2
};

class clsStrategyFactory
{
public:
    static clsStrategy* getStrategy(STRATEGY st, QObject *parent =0)
    {
        switch (st) {
        case DayMa:
            return new clsMaStrategy(parent);
            break;
        case WeekMa:
            return new clsWeekMa(parent);
            break;
        case Attack:
            return new clsAttact(parent);
            break;
        default:
            return (clsStrategy*)NULL;
            break;
        }
    }
};

#endif // CLSSTRATEGYFACTORY_H
