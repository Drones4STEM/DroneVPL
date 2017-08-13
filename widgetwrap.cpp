#include "widgetwrap.h"

WidgetWrap::WidgetWrap()
{
    identifier = "none";
}

WidgetWrap::WidgetWrap(ComputeNode* cn)
{
    category = "Compute";
    identifier = "Compute"; //控件类型
    controlsId = cn->controlsId;
    name = cn->name;
    mComputeNode = cn;
}
//-------------------------------
WidgetWrap::WidgetWrap(IoNode* in)
{
    category = "IO";
    identifier = "IO"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mIONode = in;
}
//-------------------------------
WidgetWrap::WidgetWrap(Rec* ln)
{
    category = "Logic";
    identifier = "Logic"; //控件类型
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
    category = "VAR";
    identifier = "VarDef"; //控件类型
    controlsId = vdn->controlsId;
    name = vdn->name;
    mVarDefNode = vdn;
}

WidgetWrap::WidgetWrap(VarNode* vn)
{
    category = "VAR";
    identifier = "VarType"; //控件类型
    controlsId = vn->controlsId;
    name = vn->name;
    mVarTypeNode = vn;
}
//--------------------------------




