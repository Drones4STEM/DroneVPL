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
    //cn->wrap = this;
}
//-------------------------------
WidgetWrap::WidgetWrap(IoNode* in)
{
    category = "IO";
    identifier = "IO"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mIONode = in;
    //in->wrap = this;
}
//-------------------------------
WidgetWrap::WidgetWrap(Rec* ln)
{
    category = "Logic";
    identifier = "Logic"; //控件类型
    controlsId = ln->controlsId;
    name = ln->name;
    mLogicNode = ln;
    //ln->wrap = this;
}
//---------Action--------------
WidgetWrap::WidgetWrap(TakeoffNode *ton)
{
    category = "Action";
    identifier = "TakeOff"; //控件类型
    controlsId = ton->controlsId;
    name = ton->name;
    mTakeOffNode = ton;
    //ton->wrap = this;
}

WidgetWrap::WidgetWrap(LandonNode *ln)
{
    category = "Action";
    identifier = "Land"; //控件类型
    controlsId = ln->controlsId;
    name = ln->name;
    mLandNode = ln;
    //ln->wrap = this;
}

WidgetWrap::WidgetWrap(TranslationNode *tn)
{
    category = "Action";
    identifier = "Go"; //控件类型
    controlsId = tn->controlsId;
    name = tn->name;
    mGoNode = tn;
   // tn->wrap = this;
}

WidgetWrap::WidgetWrap(TurnNode *tn)
{
    category = "Action";
    identifier = "Turn"; //控件类型
    controlsId = tn->controlsId;
    name = tn->name;
    mTurnNode = tn;
    //tn->wrap = this;
}

WidgetWrap::WidgetWrap(HoverNode *hn)
{
    category = "Action";
    identifier = "Hover"; //控件类型
    controlsId = hn->controlsId;
    name = hn->name;
    mHoverNode = hn;
    //hn->wrap = this;
}

WidgetWrap::WidgetWrap(DelayNode *dn)
{
    category = "Action";
    identifier = "Delay"; //控件类型
    controlsId = dn->controlsId;
    name = dn->name;
    mDelayNode = dn;
   // dn->wrap = this;
}
//-------------------------------
WidgetWrap::WidgetWrap(VardefNode* vdn)
{
    category = "VAR";
    identifier = "VarDef"; //控件类型
    controlsId = vdn->controlsId;
    name = vdn->name;
    mVarDefNode = vdn;
    //vdn->wrap = this;
}

WidgetWrap::WidgetWrap(VarNode* vn)
{
    category = "VAR";
    identifier = "VarType"; //控件类型
    controlsId = vn->controlsId;
    name = vn->name;
    mVarTypeNode = vn;
    //vn->wrap = this;
}
//--------------------------------

WidgetWrap::WidgetWrap(Link* link)
{
    mLinkNode = link;
    category = "Link";
    identifier = "Link";
    controlsId = link->controlsId;
    name = link->name;

}


