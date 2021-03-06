#include "widgetwrap.h"

WidgetWrap::WidgetWrap()
{
    identifier = "none";
}

WidgetWrap::WidgetWrap(ComputeNode* cn)
{
    category = "Compute";
    identifier = cn->identifier; //控件类型
    controlsId = cn->controlsId;
    name = cn->name;
    mComputeNode = cn;
    lx = cn->lx;
    ly = cn->ly;
    high = cn->high;
    wide =  cn->wide;
}
WidgetWrap::WidgetWrap(eNode* cn)
{
    category = "Compute";
    identifier = cn->identifier; //控件类型
    controlsId = cn->controlsId;
    name = cn->name;
    mENode = cn;
    lx = cn->lx;
    ly = cn->ly;
    high = cn->high;
    wide = cn->wide;
}
WidgetWrap::WidgetWrap(sinNode* cn)
{
    category = "Compute";
    identifier = cn->identifier; //控件类型
    controlsId = cn->controlsId;
    name = cn->name;
    mSinNode = cn;
    lx = cn->lx;
    ly = cn->ly;
    high = cn->high;
    wide = cn->wide;
}
WidgetWrap::WidgetWrap(logNode* cn)
{
    category = "Compute";
    identifier = cn->identifier; //控件类型
    controlsId = cn->controlsId;
    name = cn->name;
    mLogNode = cn;
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
WidgetWrap::WidgetWrap(AttitudeNode* in)
{
    category = "IO";
    identifier = "Attitude"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mAttitudeNode = in;
    lx = in->lx;
    ly = in->ly;
    high = in->high;
    wide = in->wide;
}
WidgetWrap::WidgetWrap(ChannelNode* in)
{
    category = "IO";
    identifier = "Channel"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mChannelNode = in;
    lx = in->lx;
    ly = in->ly;
    high = in->high;
    wide = in->wide;
}
WidgetWrap::WidgetWrap(RangeFinderNode* in)
{
    category = "IO";
    identifier = "RangeFinder"; //控件类型
    controlsId = in->controlsId;
    name = in->name;
    mRangeFinderNode = in;
    lx = in->lx;
    ly = in->ly;
    high = in->high;
    wide = in->wide;
}
//-------------------------------
WidgetWrap::WidgetWrap(Rec* ln)
{
    category = "Logic";
    identifier = ln->identifier; //控件类型
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
WidgetWrap::WidgetWrap(VarInstanceNode* vdn)
{
    category = "VAR";
    identifier = "VarInstance"; //控件类型
    controlsId = vdn->controlsId;
    name = vdn->name;
    mVarInstanceNode = vdn;
    lx = vdn->lx;
    ly = vdn->ly;
    high = vdn->high;
    wide = vdn->wide;
}

WidgetWrap::WidgetWrap(VarNode* vn)
{
    category = "VAR";
    identifier = "Var"; //控件类型
    controlsId = vn->controlsId;
    name = vn->name;
    mVarNode = vn;
    lx = vn->lx;
    ly = vn->ly;
//    high = vn->high;
//    wide = vn->wide;
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
    if(identifier == "Var")
        return NULL;
    if(identifier == "VarInstance")
        return mVarInstanceNode->yuan;
    if(identifier == "Compute")
        return mComputeNode->yuan;
    if(identifier == "Log")
        return mComputeNode->yuan;
    if(identifier == "E")
        return mComputeNode->yuan;
    if(identifier == "Sin")
        return mComputeNode->yuan;
    if(identifier == "IO")
        return mIONode->yuan;
    if(identifier == "Battery")
        return mBatteryNode->yuan;
    if(identifier == "Attitude")
        return mAttitudeNode->yuan;
    if(identifier == "Channel")
        return mChannelNode->yuan;
    if(identifier == "RangeFinder")
        return mRangeFinderNode->yuan;
    if(identifier == "If"||
            identifier == "Else"||
            identifier == "While")
        return mLogicNode->yuan;
    return 0;
}
//这个函数只被另一个可以淘汰的函数调用，但目前仍在使用，且以后可能有用，故维护
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
    if(identifier == "Var")
        return false;
    if(identifier == "VarInstance")
        if(mVarInstanceNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mVarInstanceNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mVarInstanceNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Compute")
        if(mComputeNode->yuan2->myLinks.isEmpty() )
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
    if(identifier == "E")
        if(mENode->yuan2->myLinks.isEmpty() )
            return false;
        else{
            QList<Link*> links = mENode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mENode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Log")
        if(mLogNode->yuan2->myLinks.isEmpty() )
            return false;
        else{
            QList<Link*> links = mLogNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mLogNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Sin")
        if(mSinNode->yuan2->myLinks.isEmpty() )
            return false;
        else{
            QList<Link*> links = mSinNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mSinNode->rank)
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
    if(identifier == "Attitude")
        if(mAttitudeNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mAttitudeNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mAttitudeNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "Channel")
        if(mChannelNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mChannelNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mChannelNode->rank)
                    //级数大表示指向这个节点的上级节点在同一图或子图，即有入度
                    return true;
                else return false;
            }

        }
    if(identifier == "RangeFinder")
        if(mRangeFinderNode->yuan2->myLinks.isEmpty())
            return false;
        else{
            QList<Link*> links = mRangeFinderNode->yuan2->myLinks.values();
            for(int i=0;i<links.length();i++){
                if(links[i]->fromYuan()->master->rank()>=mRangeFinderNode->rank)
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
    if(identifier == "Var")
        return mVarNode->pos();
    if(identifier == "VarInstance")
        return mVarInstanceNode->pos();
    if(identifier == "Compute")
        return mComputeNode->pos();
    if(identifier == "E")
        return mENode->pos();
    if(identifier == "Log")
        return mLogNode->pos();
    if(identifier == "Sin")
        return mSinNode->pos();
    if(identifier == "IO")
        return mIONode->pos();
    if(identifier == "Battery")
        return mBatteryNode->pos();
    if(identifier == "Attitude")
        return mAttitudeNode->pos();
    if(identifier == "Channel")
        return mChannelNode->pos();
    if(identifier == "RangeFinder")
        return mRangeFinderNode->pos();
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
    if(identifier == "Var")
//        return mVarNode->rank;
    if(identifier == "VarInstance")
        return mVarInstanceNode->rank;
    if(identifier == "Compute")
        return mComputeNode->rank;
    if(identifier == "E")
        return mENode->rank;
    if(identifier == "Log")
        return mLogNode->rank;
    if(identifier == "Sin")
        return mSinNode->rank;
    if(identifier == "IO")
        return mIONode->rank;
    if(identifier == "Battery")
        return mBatteryNode->rank;
    if(identifier == "Attitude")
        return mAttitudeNode->rank;
    if(identifier == "Channel")
        return mChannelNode->rank;
    if(identifier == "RangeFinder")
        return mRangeFinderNode->rank;
    if(identifier == "While"||
            identifier == "If"||
            identifier == "Else")
        return mLogicNode->rank;
    return -1;
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
    if(identifier == "Var")
//        mVarNode->rank = r;
    if(identifier == "VarInstance")
        mVarInstanceNode->rank = r;
    if(identifier == "Compute")
        mComputeNode->rank = r;
    if(identifier == "E")
        mENode->rank = r;
    if(identifier == "Log")
        mLogNode->rank = r;
    if(identifier == "Sin")
        mSinNode->rank = r;
    if(identifier == "IO")
        mIONode->rank = r;
    if(identifier == "Battery")
        mBatteryNode->rank = r;
    if(identifier == "Attitude")
        mAttitudeNode->rank = r;
    if(identifier == "Channel")
        mChannelNode->rank = r;
    if(identifier == "RangeFinder")
        mRangeFinderNode->rank = r;
    if(identifier == "While" ||
            identifier == "If" ||
            identifier == "Else")
        mLogicNode->rank = r;
}

double WidgetWrap::width()
{
    if(identifier == "TakeOff")
        return mTakeOffNode->boundingRect().width();
    if(identifier == "Land")
        return mLandNode->boundingRect().width();
    if(identifier == "Go")
        return mGoNode->boundingRect().width();
    if(identifier == "Turn")
        return mTurnNode->boundingRect().width();
    if(identifier == "Hover")
        return mHoverNode->boundingRect().width();
    if(identifier == "Delay")
        return mDelayNode->boundingRect().width();
    if(identifier == "Var")
//        return mVarNode->boundingRect().width();
    if(identifier == "VarInstance")
        return mVarInstanceNode->boundingRect().width();
    if(identifier == "Compute")
        return mComputeNode->boundingRect().width();
    if(identifier == "E")
        return mENode->boundingRect().width();
    if(identifier == "Log")
        return mLogNode->boundingRect().width();
    if(identifier == "Sin")
        return mSinNode->boundingRect().width();
    if(identifier == "IO")
        return mIONode->boundingRect().width();
    if(identifier == "Battery")
        return mBatteryNode->boundingRect().width();
    if(identifier == "Attitude")
        return mAttitudeNode->boundingRect().width();
    if(identifier == "Channel")
        return mChannelNode->boundingRect().width();
    if(identifier == "RangeFinder")
        return mRangeFinderNode->boundingRect().width();
    if(identifier == "While" ||
            identifier == "If" ||
            identifier == "Else")
        return mLogicNode->boundingRect().width();
}

double WidgetWrap::height()
{
    if(identifier == "TakeOff")
        return mTakeOffNode->boundingRect().height();
    if(identifier == "Land")
        return mLandNode->boundingRect().height();
    if(identifier == "Go")
        return mGoNode->boundingRect().height();
    if(identifier == "Turn")
        return mTurnNode->boundingRect().height();
    if(identifier == "Hover")
        return mHoverNode->boundingRect().height();
    if(identifier == "Delay")
        return mDelayNode->boundingRect().height();
    if(identifier == "Var")
//        return mVarNode->boundingRect().height();
    if(identifier == "VarInstance")
        return mVarInstanceNode->boundingRect().height();
    if(identifier == "Compute")
        return mComputeNode->boundingRect().height();
    if(identifier == "E")
        return mENode->boundingRect().height();
    if(identifier == "Log")
        return mLogNode->boundingRect().height();
    if(identifier == "Sin")
        return mSinNode->boundingRect().height();
    if(identifier == "IO")
        return mIONode->boundingRect().height();
    if(identifier == "Battery")
        return mBatteryNode->boundingRect().height();
    if(identifier == "Attitude")
        return mAttitudeNode->boundingRect().height();
    if(identifier == "Channel")
        return mChannelNode->boundingRect().height();
    if(identifier == "RangeFinder")
        return mRangeFinderNode->boundingRect().height();
    if(identifier == "While" ||
            identifier == "If" ||
            identifier == "Else")
        return mLogicNode->boundingRect().height();
}
