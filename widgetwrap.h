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
#include "link.h"
#include "yuan.h"

class WidgetWrap
{
public:
    WidgetWrap();
    WidgetWrap(TakeoffNode* ton);
    WidgetWrap(LandonNode* ln);
    WidgetWrap(TranslationNode* tn);
    WidgetWrap(TurnNode *turnn);
    WidgetWrap(HoverNode *hn);
    WidgetWrap(DelayNode *dn);

    WidgetWrap(ComputeNode* cn);
    WidgetWrap(IoNode* in);
    WidgetWrap(Rec* ln);

    WidgetWrap(VardefNode* vdn);
    WidgetWrap(VarNode* vn);

    WidgetWrap(Link* link);

    triYuan* get_yuan_out();
    bool check_yuan_in();

    QString category;
    QString identifier; //控件型号
    int controlsId;
    QString name;   //identifier+controlsId

    ComputeNode* mComputeNode = NULL;

    IoNode* mIONode;

    Rec* mLogicNode;

    VardefNode* mVarDefNode;
    VarNode* mVarTypeNode;

    TakeoffNode* mTakeOffNode;
    LandonNode* mLandNode;
    TranslationNode* mGoNode;
    TurnNode* mTurnNode;
    HoverNode* mHoverNode;
    DelayNode* mDelayNode;
    Link* mLinkNode;

private:

};

#endif // WIDGETWRAP_H
