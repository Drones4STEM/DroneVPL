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
WidgetWrap::WidgetWrap(BatteryNode* in)
{
    category = "IO";
    identifier = "Battery"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mBatteryNode = in;
    lx = in->lx;
    ly = in->ly;
    high = in->high;
    wide = in->wide;
}
WidgetWrap::WidgetWrap(GimbalNode* in)
{
    category = "IO";
    identifier = "Gimbal"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mGimbalNode = in;
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
WidgetWrap::WidgetWrap(TakeOffNode *ton)
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

WidgetWrap::WidgetWrap(LandNode *ln)
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

WidgetWrap::WidgetWrap(GoNode *tn)
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
    if(identifier == "Battery")
        return mBatteryNode->yuan;
    if(identifier == "Logic")
        return mLogicNode->yuan;
    return 0;
}
bool WidgetWrap::check_yuan_in()
{
    if(identifier == "TakeOff")
        return false;
    if(identifier == "Land")
        if(mLandNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mLandNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mLandNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Go")
        if(mGoNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mGoNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mGoNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Turn")
        if(mTurnNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mTurnNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mTurnNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Hover")
        if(mHoverNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mHoverNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mHoverNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Delay")
        if(mDelayNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mDelayNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mDelayNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "VarType")
        return false;
    if(identifier == "VarDef")
        if(mVarDefNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mVarDefNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mVarDefNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Compute")
        if(mComputeNode->yuan2->myLinks.isEmpty() &&
                mComputeNode->yuan3->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mComputeNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mComputeNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Battery")
        if(mBatteryNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mBatteryNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mBatteryNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Logic")
        if(mLogicNode->flink.isEmpty())
            return false;
        else{
            QList<Link*> links = mLogicNode->flink;
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mLogicNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
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
    if(identifier == "Battery")
        return mBatteryNode->pos();
    if(identifier == "Logic")
        return mLogicNode->pos();
}

int WidgetWrap::rank()
{
    if(identifier == "TakeOff")
        return mTakeOffNode->rank;
    if(identifier == "Land")
        return mLandNode->rank;
    if(identifier == "Go")
        return mGoNode->rank;
    if(identifier == "Turn")
        return mTurnNode->rank;
    if(identifier == "Hover")
        return mHoverNode->rank;
    if(identifier == "Delay")
        return mDelayNode->rank;
    if(identifier == "VarType")
        return mVarTypeNode->rank;
    if(identifier == "VarDef")
        return mVarDefNode->rank;
    if(identifier == "Compute")
        return mComputeNode->rank;
    if(identifier == "IO")
        return mIONode->rank;
    if(identifier == "Battery")
        return mBatteryNode->rank;
    if(identifier == "Logic")
        return mLogicNode->rank;
}

void WidgetWrap::rank(int r)
{
    if(identifier == "TakeOff")
        mTakeOffNode->rank = r;
    if(identifier == "Land")
        mLandNode->rank = r;
    if(identifier == "Go")
        mGoNode->rank = r;
    if(identifier == "Turn")
        mTurnNode->rank = r;
    if(identifier == "Hover")
        mHoverNode->rank = r;
    if(identifier == "Delay")
        mDelayNode->rank = r;
    if(identifier == "VarType")
        mVarTypeNode->rank = r;
    if(identifier == "VarDef")
        mVarDefNode->rank = r;
    if(identifier == "Compute")
        mComputeNode->rank = r;
    if(identifier == "IO")
        mIONode->rank = r;
    if(identifier == "Battery")
        mBatteryNode->rank = r;
    if(identifier == "Logic")
        mLogicNode->rank = r;
}
