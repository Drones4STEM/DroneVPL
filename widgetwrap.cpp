#include "widgetwrap.h"

WidgetWrap::WidgetWrap()
{
    identifier = "none";
}

WidgetWrap::WidgetWrap(ComputeNode* cn)
{
    identifier = "ComputeNode"; //控件类型
    controlsId = cn->controlsId;
    name = cn->name;
    mComputeNode = cn;
}

WidgetWrap::WidgetWrap(IoNode* in)
{
    identifier = "IoNode"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mIoNode = in;
}

WidgetWrap::WidgetWrap(Rec* ln)
{
    identifier = "LogicNode"; //控件类型
    controlsId = ln->controlsId;
    name = ln->name;
    mLogicNode = ln;
}

WidgetWrap::WidgetWrap(TakeoffNode* tn)
{
    identifier = "TakeOff"; //控件类型
    controlsId = tn->controlsId;
    name = tn->name;
    mTakeoffNode = tn;
}

WidgetWrap::WidgetWrap(VardefNode* vdn)
{
    identifier = "VardefNode"; //控件类型
    controlsId = vdn->controlsId;
    name = vdn->name;
    mVardefNode = vdn;
}

WidgetWrap::WidgetWrap(VarNode* vn)
{
    identifier = "VarNode"; //控件类型
    controlsId = vn->controlsId;
    name = vn->name;
    mVarNode = vn;
}





