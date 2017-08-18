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
    lx = cn->lx;
    ly = cn->ly;
    high = cn->high;
    wide = cn->wide;
}
//-------------------------------
WidgetWrap::WidgetWrap(IoNode* in)
{
    category = "IO";
    identifier = "IO"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mIONode = in;
    lx = in->lx;
    ly = in->ly;
    high = in->high;
    wide = in->wide;
}
//-------------------------------
WidgetWrap::WidgetWrap(Rec* ln)
{
    category = "Logic";
    identifier = "Logic"; //控件类型
    controlsId = ln->controlsId;
    name = ln->name;
    mLogicNode = ln;
    lx = ln->lx;
    ly = ln->ly;
    high = ln->high;
    wide = ln->wide;
}
//---------Action--------------
WidgetWrap::WidgetWrap(TakeoffNode *ton)
{
    category = "Action";
    identifier = "TakeOff"; //控件类型
    controlsId = ton->controlsId;
    name = ton->name;
    mTakeOffNode = ton;
    lx = ton->lx;
    ly = ton->ly;
    high = ton->high;
    wide = ton->wide;
}

WidgetWrap::WidgetWrap(LandonNode *ln)
{
    category = "Action";
    identifier = "Land"; //控件类型
    controlsId = ln->controlsId;
    name = ln->name;
    mLandNode = ln;
    lx = ln->lx;
    ly = ln->ly;
    high = ln->high;
    wide = ln->wide;
}

WidgetWrap::WidgetWrap(TranslationNode *tn)
{
    category = "Action";
    identifier = "Go"; //控件类型
    controlsId = tn->controlsId;
    name = tn->name;
    mGoNode = tn;
    lx = tn->lx;
    ly = tn->ly;
    high = tn->high;
    wide = tn->wide;
}

WidgetWrap::WidgetWrap(TurnNode *tn)
{
    category = "Action";
    identifier = "Turn"; //控件类型
    controlsId = tn->controlsId;
    name = tn->name;
    mTurnNode = tn;
    lx = tn->lx;
    ly = tn->ly;
    high = tn->high;
    wide = tn->wide;
}

WidgetWrap::WidgetWrap(HoverNode *hn)
{
    category = "Action";
    identifier = "Hover"; //控件类型
    controlsId = hn->controlsId;
    name = hn->name;
    mHoverNode = hn;
    lx = hn->lx;
    ly = hn->ly;
    high = hn->high;
    wide = hn->wide;
}

WidgetWrap::WidgetWrap(DelayNode *dn)
{
    category = "Action";
    identifier = "Delay"; //控件类型
    controlsId = dn->controlsId;
    name = dn->name;
    mDelayNode = dn;
    lx = dn->lx;
    ly = dn->ly;
    high = dn->high;
    wide = dn->wide;
}
//-------------------------------
WidgetWrap::WidgetWrap(VardefNode* vdn)
{
    category = "VAR";
    identifier = "VarDef"; //控件类型
    controlsId = vdn->controlsId;
    name = vdn->name;
    mVarDefNode = vdn;
    lx = vdn->lx;
    ly = vdn->ly;
    high = vdn->high;
    wide = vdn->wide;
}

WidgetWrap::WidgetWrap(VarNode* vn)
{
    category = "VAR";
    identifier = "VarType"; //控件类型
    controlsId = vn->controlsId;
    name = vn->name;
    mVarTypeNode = vn;
    lx = vn->lx;
    ly = vn->ly;
    high = vn->high;
    wide = vn->wide;
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
//--------------------------------
triYuan* WidgetWrap::get_yuan_out()
{
    if(identifier == "TakeOff")
        return mTakeOffNode->yuan;
    if(identifier == "Land")
        return NULL;
    if(identifier == "Go")
        return mGoNode->yuan;
    if(identifier == "Turn")
        return mTurnNode->yuan;
    if(identifier == "Hover")
        return mHoverNode->yuan;
    if(identifier == "Delay")
        return mDelayNode->yuan;
    if(identifier == "VarType")
        return NULL;
    if(identifier == "VarDef")
        return mVarDefNode->yuan;
    if(identifier == "Compute")
        return mComputeNode->yuan;
    if(identifier == "IO")
        return mIONode->yuan;
    if(identifier == "Logic")
        return mLogicNode->yuan;
}
bool WidgetWrap::check_yuan_in()
{
    if(identifier == "TakeOff")
        return false;
    if(identifier == "Land")
        if(!mLandNode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "Go")
        if(!mGoNode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "Turn")
        if(!mTurnNode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "Hover")
        if(!mHoverNode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "Delay")
        if(!mDelayNode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "VarType")
        return false;
    if(identifier == "VarDef")
        if(!mVarDefNode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "Compute")
        if(mComputeNode->yuan2->myLinks.isEmpty() &&
                mComputeNode->yuan3->myLinks.isEmpty())
            return false;
        else return true;
    if(identifier == "IO")
        if(!mIONode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "Logic")
        if(!mLogicNode->yuan2->myLinks.isEmpty())
            return true;
        else return false;
    if(identifier == "Link")
            return true;
}

QPointF WidgetWrap::pos()
{
    if(identifier == "TakeOff")
        return mTakeOffNode->pos();
    if(identifier == "Land")
        return mLandNode->pos();
    if(identifier == "Go")
        return mGoNode->pos();
    if(identifier == "Turn")
        return mTurnNode->pos();
    if(identifier == "Hover")
        return mHoverNode->pos();
    if(identifier == "Delay")
        return mDelayNode->pos();
    if(identifier == "VarType")
        return mVarTypeNode->pos();
    if(identifier == "VarDef")
        return mVarDefNode->pos();
    if(identifier == "Compute")
        return mComputeNode->pos();
    if(identifier == "IO")
        return mIONode->pos();
    if(identifier == "Logic")
        return mLogicNode->pos();
}
