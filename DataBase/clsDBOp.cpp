#include "clsDBOp.h"


clsDBOp * clsDBOp::mInstance =0;
clsDBOp::clsRmDBOp clsDBOp::rmDbOp;


clsDBOp::clsDBOp(QObject *parent) : QObject(parent)
{

}

clsDBOp *clsDBOp::instance()
{

    if(mInstance ==0)
    {
        mInstance = new clsDBOp();

    }
}
