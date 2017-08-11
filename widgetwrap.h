#ifndef WIDGETWRAP_H
#define WIDGETWRAP_H

#include <QString>

#include <iostream>

#include "computenode.h"
#include "ionode.h"
#include "rec.h"         //logic
#include "takeoffnode.h"
#include "vardefnode.h"
#include "varnode.h"

class WidgetWrap
{
public:
    WidgetWrap();
    WidgetWrap(ComputeNode* cn);
    WidgetWrap(IoNode* in);
    WidgetWrap(Rec* ln);
    WidgetWrap(TakeoffNode* tn);
    WidgetWrap(VardefNode* vdn);
    WidgetWrap(VarNode* vn);

    QString type;
    QString identifier; //控件型号
    int controlsId;
    QString name;   //identifier+controlsId

    ComputeNode* mComputeNode = NULL;
    IoNode* mIoNode;
    Rec* mLogicNode;
    TakeoffNode* mTakeoffNode;
    VardefNode* mVardefNode;
    VarNode* mVarNode;


private:

};

#endif // WIDGETWRAP_H
