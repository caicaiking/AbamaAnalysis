#include "stockdata.h"

StockData::StockData()
{
    qRegisterMetaType<StockDataList>("const StockDataList&");
    totalshare = 0;
    mutableshare = 0;
    index = false;
}

StockData::~StockData()
{

}

bool StockData::sortByPerDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.per > d2.per;
}

bool StockData::sortByPerAsc(const StockData &d1, const StockData &d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.per < d2.per;
}

bool StockData::sortByCurDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.cur > d2.cur;
}

bool StockData::sortByCurAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.cur < d2.cur;
}

bool StockData::sortByMonDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.money > d2.money;
}

bool StockData::sortByMonAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.money < d2.money;
}

bool StockData::sortByHslDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.hsl > d2.hsl;
}

bool StockData::sortByHslAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.hsl < d2.hsl;
}

bool StockData::sortByMonRatioDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.money_ratio > d2.money_ratio;
}

bool StockData::sortByMonRatioAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.money_ratio < d2.money_ratio;
}

bool StockData::sortBy3DayChgAsc(const StockData &d1, const StockData &d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.last_three_pers < d2.last_three_pers;
}

bool StockData::sortBy3DayChgDesc(const StockData &d1, const StockData &d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.last_three_pers > d2.last_three_pers;
}

bool StockData::sortBy3DayZjlxAsc(const StockData &d1, const StockData &d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.zjlx < d2.zjlx;
}

bool StockData::sortBy3DayZjlxDesc(const StockData &d1, const StockData &d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.zjlx > d2.zjlx;
}

bool StockData::sortByGxlDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.gxl> d2.gxl;
}

bool StockData::sortByGxlAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.gxl < d2.gxl;
}

bool StockData::sortBySzzblDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.szzbl > d2.szzbl;
}

bool StockData::sortBySzzblAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.szzbl < d2.szzbl;
}

bool StockData::sortByGqdjrDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.gqdjr > d2.gqdjr;
}

bool StockData::sortByGqdjrAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.gqdjr < d2.gqdjr;
}

bool StockData::sortByTcapDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.totalCap > d2.totalCap;
}

bool StockData::sortByTcapAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.totalCap < d2.totalCap;
}

bool StockData::sortByMcapDesc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return d1.index;
    return d1.mutalbleCap > d2.mutalbleCap;
}

bool StockData::sortByMcapAsc(const StockData& d1, const StockData& d2)
{
    if(d1.index ^ d2.index) return !d1.index;
    return d1.mutalbleCap < d2.mutalbleCap;
}
