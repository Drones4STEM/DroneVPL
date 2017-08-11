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
//---------Action--------------
WidgetWrap::WidgetWrap(TakeoffNode *ton)
{
    category = "Action";
    identifier = "TakeOff"; //控件类型
    controlsId = ton->controlsId;
    name = ton->name;
    mTakeoffNode = ton;
}

WidgetWrap::WidgetWrap(LandonNode *ln)
{
    category = "Action";
    identifier = "Land"; //控件类型
    controlsId = ln->controlsId;
    name = ln->name;
    mLandNode = ln;
}

WidgetWrap::WidgetWrap(TranslationNode *tn)
{
    category = "Action";
    identifier = "Go"; //控件类型
    controlsId = tn->controlsId;
    name = tn->name;
    mGoNode = tn;
}

WidgetWrap::WidgetWrap(TurnNode *tn)
{
    category = "Action";
    identifier = "Turn"; //控件类型
    controlsId = tn->controlsId;
    name = tn->name;
    mTurnNode = tn;
}

WidgetWrap::WidgetWrap(HoverNode *hn)
{
    category = "Action";
    identifier = "Hover"; //控件类型
    controlsId = hn->controlsId;
    name = hn->name;
    mHoverNode = hn;
}

WidgetWrap::WidgetWrap(DelayNode *dn)
{
    category = "Action";
    identifier = "Delay"; //控件类型
    controlsId = dn->controlsId;
    name = dn->name;
    mDelayNode = dn;
}
//-------------------------------
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





