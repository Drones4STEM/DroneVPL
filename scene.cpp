/*******************************************************************
 * File:scene.cpp
 * Author: Zhou Hengcheng
 * Description: set a new directed line when a triYuan is being clicked
******************************************************************/
#include <QtGui>

#include "scene.h"
#include "yuan.h"
#include "link.h"
#include <QString>
#include <QtGui>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include "qevent.h"
#include <string>
#include <QGraphicsView>
#include "aqp/aqp.hpp"
#include "aqp/alt_key.hpp"
#include "diagramwindow.h"
#include "link.h"
#include "node.h"
#include "newnode.h"
#include "takeoffnode.h"
#include "varnode.h"
#include "varinstancenode.h"
#include "computenode.h"
#include "ionode.h"
#include "yuan.h"
#include "rec.h"
#include "propertiesdialog.h"
#include "itemtypes.h"
#include "map_instrument.h"
#include "logic_help.h"

newscene::newscene(WidgetMap* m, QMap<QString, LOGIC_Help *> *L)
{
    new_yuan=new specialYuan;
    setSceneRect(QRectF(QPointF(0,0), QSize(1000, 1000)));

    need_to_set=0;
    selected_Index=1;

    VarNodeNum = 0;  //计数varNode,命名每个varNode,下同
    VarInstanceNodeNum = 0;

    LogicNodeNum=0;

    takeoffNodeNum=0;
    landonNodeNum=0;
    GoNodeNum = 0;
    TurnNodeNum = 0;
    HoverNodeNum = 0;
    DelayNodeNum = 0;

    ComputeNodeNum = 0;

    IONodeNum = 0;
    BatteryNodeNum = 0;
    GimbalNodeNum = 0;
    AttitudeNodeNum = 0;
    ChannelNodeNum = 0;
    RangeFinderNodeNum = 0;

    linkNodeNum=0;

    wm=m;
    LHM = L;

    qRegisterMetaType<QSet<Link*> >("QSet<Link*>");
    connect(this,SIGNAL(sig_link_deleted(QSet<Link*>)),wm,SLOT(link_deleted(QSet<Link*>)));
}

newscene::~newscene()
{
   delete new_yuan;
}

void newscene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if((this->selectedItems().count()==1)&&dynamic_cast<triYuan *>(this->selectedItems().first())!=0)
    {
          new_yuan->setPos(event->scenePos());

          if(new_yuan->myLinks.size()>0)
          {
              foreach (Link* link,new_yuan->myLinks)
                  delete link;
          }
         Link* new_link=new Link(dynamic_cast<triYuan *>(this->selectedItems(). first()),new_yuan);
         new_link->setZValue(100);
         this->addItem(new_link);
         this->addItem(new_yuan);

    update();
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void newscene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{

    Link* link = CreateLink(event);
//    CheckLinkOverLogic(link);   //检查link是否穿越logic
    //现在不存在穿越了，但是这个函数写一遍太累了，决定注释而不删
    //检查鼠标释放时生成/拖动的控件是否在Logic内
//    CheckInLogic();


    QGraphicsScene::mouseReleaseEvent(event);
}

void newscene::bringToFront()
{
    emit sig_bringtofront();
}

void newscene::mousePressEvent(QGraphicsSceneMouseEvent *new_event){

    if(need_to_set==1&&new_event->button()==Qt::RightButton){
        need_to_set = 0;
    }
    if(selected_Index==101&&need_to_set==1){
        emit itemInserted(selected_Index);
        this->takeoffNodeNum++;
        CreateTakeOff(new_event->scenePos(),this->takeoffNodeNum);
        need_to_set = 0;
    }
    if(selected_Index==102&&need_to_set==1){
        emit itemInserted(selected_Index);
        this->landonNodeNum++;
        CreateLand(new_event->scenePos(),this->landonNodeNum);

        need_to_set = 0;
    }
    if((selected_Index>=103&&selected_Index<=108)&&need_to_set==1){
        emit itemInserted(selected_Index);
        this->GoNodeNum++;
        need_to_set = 0;
        CreateGo(new_event->scenePos(),this->GoNodeNum,selected_Index);
    }
    if(need_to_set==1&&(selected_Index>=110&&selected_Index<=111)){
        emit itemInserted(selected_Index);
        this->TurnNodeNum++;
        need_to_set = 0;
        CreateTurn(new_event->scenePos(),this->TurnNodeNum,selected_Index);
    }
    if(need_to_set==1&&selected_Index==112){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->HoverNodeNum++;
        CreateHover(new_event->scenePos(),this->HoverNodeNum);
    }
    if(need_to_set==1&&selected_Index==113){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->DelayNodeNum++;
        CreateDelay(new_event->scenePos(),this->DelayNodeNum);
    }
    if(need_to_set==1&&selected_Index==201){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->VarNodeNum++;
        CreateVar(new_event->scenePos(),this->VarNodeNum);
    }
    if(need_to_set==1&&selected_Index==202){
//        emit itemInserted(selected_Index);
//        need_to_set = 0;
//        this->VarInstanceNodeNum++;
//        bool flag = CreateVarInstance(new_event->scenePos(),this->VarInstanceNodeNum);
//        if(flag == false)   this->VarInstanceNodeNum--;
    }
    if(need_to_set==1&&selected_Index>=301&&selected_Index<308){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->ComputeNodeNum++;
        CreateCompute(new_event->scenePos(),this->ComputeNodeNum,selected_Index);
    }
    if(need_to_set==1&&selected_Index==308)
    {
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->ComputeNodeNum++;
        CreateSin(new_event->scenePos(),this->ComputeNodeNum);
    }
    if(need_to_set==1&&selected_Index==311)
    {
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->ComputeNodeNum++;
        CreateLog(new_event->scenePos(),this->ComputeNodeNum);
    }
    if(need_to_set==1&&selected_Index==312)
    {
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->ComputeNodeNum++;
        CreateE(new_event->scenePos(),this->ComputeNodeNum);
    }
    if(need_to_set==1&&selected_Index==400){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->IONodeNum++;
        CreateIO(new_event->scenePos(),this->IONodeNum);
    }
    if(need_to_set==1&&selected_Index==401){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->BatteryNodeNum++;
        CreateBattery(new_event->scenePos(),this->BatteryNodeNum);
    }
    if(need_to_set==1&&selected_Index==402){qDebug()<<"402.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->GimbalNodeNum++;
        CreateGimbal(new_event->scenePos(),this->GimbalNodeNum);
    }
    if(need_to_set==1&&selected_Index==403){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->AttitudeNodeNum++;
        CreateAttitude(new_event->scenePos(),this->AttitudeNodeNum);
    }
    if(need_to_set==1&&selected_Index==404){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->ChannelNodeNum++;
        CreateChannel(new_event->scenePos(),this->ChannelNodeNum);
    }
    if(need_to_set==1&&selected_Index==405){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->RangeFinderNodeNum++;
        CreateRangeFinder(new_event->scenePos(),this->RangeFinderNodeNum);
    }
    if(need_to_set==1&&selected_Index==501){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->LogicNodeNum++;
        CreateLogic(new_event->scenePos(),this->LogicNodeNum);
    }
    if(need_to_set==1&&selected_Index==502){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->LogicNodeNum++;
        CreateIf(new_event->scenePos(),this->LogicNodeNum);
    }
    if(need_to_set==1&&selected_Index==503){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->LogicNodeNum++;
        CreateElse(new_event->scenePos(),this->LogicNodeNum);
    }
    if(need_to_set==1&&selected_Index==504){
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->LogicNodeNum++;
        CreateWhile(new_event->scenePos(),this->LogicNodeNum);
    }
    QGraphicsScene::mousePressEvent(new_event);
    //setCursor(Qt::ArrowCursor);
}


/*******************************************************************
 * Function name: del()
 * Description: This function delete the selected items.
 * Callee:
 * Inputs:
 * Outputs:
******************************************************************/
void newscene::del()
{
    QList<QGraphicsItem *> items = this->selectedItems();
    int itemsCount = items.count();
    int i=0;
    QList<Link*>itemLinks;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<Link*>(items[i]))
            itemLinks<<dynamic_cast<Link*>(items[i]);
    }
    QList<TakeOffNode*>itemTakeoffs;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<TakeOffNode*>(items[i]))
            itemTakeoffs<<dynamic_cast<TakeOffNode*>(items[i]);
    }
    QList<LandNode*>itemLandons;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<LandNode*>(items[i]))
            itemLandons<<dynamic_cast<LandNode*>(items[i]);
    }
    QList<GoNode*>itemTranslations;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<GoNode*>(items[i]))
            itemTranslations<<dynamic_cast<GoNode*>(items[i]);
    }
    QList<TurnNode*>itemTurn;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<TurnNode*>(items[i]))
            itemTurn<<dynamic_cast<TurnNode*>(items[i]);
    }
    QList<HoverNode*>itemHover;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<HoverNode*>(items[i]))
            itemHover<<dynamic_cast<HoverNode*>(items[i]);
    }
    QList<DelayNode*>itemDelay;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<DelayNode*>(items[i]))
            itemDelay<<dynamic_cast<DelayNode*>(items[i]);
    }
    QList<ComputeNode*>itemComputes;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<ComputeNode*>(items[i]))
            itemComputes<<dynamic_cast<ComputeNode*>(items[i]);
    }
    QList<IoNode*>itemIos;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<IoNode*>(items[i]))
            itemIos<<dynamic_cast<IoNode*>(items[i]);
    }
    QList<BatteryNode*>itemBry;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<BatteryNode*>(items[i]))
            itemBry<<dynamic_cast<BatteryNode*>(items[i]);
    }
    QList<GimbalNode*>itemGim;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<GimbalNode*>(items[i]))
            itemGim<<dynamic_cast<GimbalNode*>(items[i]);
    }
    QList<AttitudeNode*>itemAtd;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<AttitudeNode*>(items[i]))
            itemAtd<<dynamic_cast<AttitudeNode*>(items[i]);
    }
    QList<ChannelNode*>itemChn;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<ChannelNode*>(items[i]))
            itemChn<<dynamic_cast<ChannelNode*>(items[i]);
    }
    QList<RangeFinderNode*>itemRF;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<RangeFinderNode*>(items[i]))
            itemRF<<dynamic_cast<RangeFinderNode*>(items[i]);
    }
    QList<Rec*>itemRecs;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<Rec*>(items[i]))
            itemRecs<<dynamic_cast<Rec*>(items[i]);
    }
    QList<SomeNode*>itemSomes;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<SomeNode*>(items[i]))
            itemSomes<<dynamic_cast<SomeNode*>(items[i]);
    }
    QList<VarInstanceNode*>itemVarInstances;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<VarInstanceNode*>(items[i]))
            itemVarInstances<<dynamic_cast<VarInstanceNode*>(items[i]);
    }
    QList<VarNode*>itemVars;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<VarNode*>(items[i]))
            itemVars<<dynamic_cast<VarNode*>(items[i]);
    }
    QList<VarNode*>itemVariables;
    for(i=0;i<itemsCount;i++)
    {
        if(dynamic_cast<VarNode*>(items[i]))
            itemVariables<<dynamic_cast<VarNode*>(items[i]);
    }
    foreach (Link* item, itemLinks) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        delete item;
    }
    foreach (TakeOffNode* item, itemTakeoffs) {
        /*qDebug()<<"In del():\n"<<"TakeOff: ";
        qDebug()<<"type: "<<item->identifier;
        qDebug()<<"id: "<<item->controlsId;
        qDebug()<<"location_x: "<<item->pos().x();
        qDebug()<<"location_y: "<<item->pos().y();*/
        emit sig_link_deleted(item->yuan->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (LandNode* item, itemLandons) {
        emit sig_link_deleted(item->yuan2->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (GoNode* item, itemTranslations) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (TurnNode* item, itemTurn) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (HoverNode* item, itemHover) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (DelayNode* item, itemDelay) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (ComputeNode* item, itemComputes) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        emit sig_link_deleted(item->yuan3->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
//    foreach (IoNode* item, itemIos) {
//        WidgetWrap tmp(item);
//        wm->del(tmp);
//        this->check_in_Logic(&tmp,"del",0);
//        delete item;
//    }
    foreach (BatteryNode* item, itemBry) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        emit sig_link_deleted(item->node1->yuan->myLinks);
        emit sig_link_deleted(item->node2->yuan->myLinks);
        emit sig_link_deleted(item->node3->yuan->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (GimbalNode* item, itemGim) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        emit sig_link_deleted(item->node1->yuan->myLinks);
        emit sig_link_deleted(item->node2->yuan->myLinks);
        emit sig_link_deleted(item->node3->yuan->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (AttitudeNode* item, itemAtd) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        emit sig_link_deleted(item->node1->yuan->myLinks);
        emit sig_link_deleted(item->node2->yuan->myLinks);
        emit sig_link_deleted(item->node3->yuan->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (ChannelNode* item, itemChn) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        emit sig_link_deleted(item->node1->yuan->myLinks);
        emit sig_link_deleted(item->node2->yuan->myLinks);
        emit sig_link_deleted(item->node3->yuan->myLinks);
        emit sig_link_deleted(item->node4->yuan->myLinks);
        emit sig_link_deleted(item->node5->yuan->myLinks);
        emit sig_link_deleted(item->node6->yuan->myLinks);
        emit sig_link_deleted(item->node7->yuan->myLinks);
        emit sig_link_deleted(item->node8->yuan->myLinks);
        emit sig_link_deleted(item->node9->yuan->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (RangeFinderNode* item, itemRF) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        emit sig_link_deleted(item->node2->yuan->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (Rec* item, itemRecs) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        WidgetWrap tmp(item);
//        typename QMap<QString, LOGIC_Help*>::iterator iter;
//        LOGIC_Help* lh;
//        for(iter=LHM->begin();iter!=LHM->end();){
//            lh = iter.value();
//            if(lh->LOG->name==tmp.name){
//                iter++;     //因为删除以后就没法访问下一个元素，所以手动在删除前访问
//                LHM->remove(lh->LOG->name);
//            }else iter++;
//        }
        wm->del(tmp);
        delete item;
    }
    foreach (VarInstanceNode* item, itemVarInstances) {
        emit sig_link_deleted(item->yuan->myLinks);
        emit sig_link_deleted(item->yuan2->myLinks);
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
    foreach (VarNode* item, itemVars) {
        WidgetWrap tmp(item);
        wm->del(tmp);
        this->check_in_Logic(&tmp,"del",0);
        delete item;
    }
//    foreach (SomeNode* item, itemSomes) {

//        delete item;
//    }
}


bool newscene::CreateTakeOff(QPointF point, int id)
{
    TakeOffNode *node=new TakeOffNode;

    QGraphicsItem *lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(0,-10);

    node->setPos(point);
    node->setxy(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF((node->pos().x() - node->outlineRect().width()/2 +24),
                               (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2) +3));
    this->addItem(node->yuan);

    node->controlsId=id;
    node->identifier="TakeOff";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中

    node->yuan->master = tmp;
    node->yuan->name = "yuan";
  
    emit sig_connectItem(node);//发送连接item的信号到diagramwindow
    return true;
}

bool newscene::CreateLand(QPointF point, int id)
{
    LandNode *node=new LandNode;

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan2->setPos(QPointF((node->pos().x() - node->outlineRect().width()/2 + 24),
                       (node->pos().y() - node->outlineRect().height()/2)-node->yuan2->boundingRect().height()/2 - 3));
    this->addItem(node->yuan2);

    node->controlsId=id;
    node->identifier="Land";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
        wm->add(tmp);            //添加到widgetmap中
        if(wm->Store.isEmpty()==true) qDebug()<<"CreateTakeOff()   wm's QMap is empty";
        else qDebug()<<"CreateLand()   wm's QMap is not empty";
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";

    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateGo(QPointF point, int id, int index)
{
    GoNode *node=new GoNode;

    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-175,-16);

    QGraphicsItem* lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(-25,-10);
    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(125,-10);

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 + 24,
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2 + 3)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 +24,
                      (node->pos().y() - node->outlineRect().height()/2 - node->yuan2->boundingRect().height()/2 - 3)));

    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->box->addItem(tr("向上"));
    node->box->addItem(tr("向下"));
    node->box->addItem(tr("前进"));
    node->box->addItem(tr("后退"));
    node->box->addItem(tr("向右"));
    node->box->addItem(tr("向左"));
    node->box->setCurrentIndex(index-103);
    node->setDirection();
    connect(node->box,SIGNAL(currentIndexChanged(int)),node,SLOT(setDirection()));

    node->controlsId=id;
    node->identifier="Go";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中

    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";

    emit sig_connectItem(node);
    return true;
}

bool newscene::CreateTurn(QPointF point, int id, int index)
{
    TurnNode *node=new TurnNode;
    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-100,-16);
    QGraphicsItem* lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(50,-10);

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 + 24,
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2 + 3)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 + 24,
                      (node->pos().y() - node->outlineRect().height()/2 - node->yuan2->boundingRect().height()/2 - 3 )));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("左转"));
    node->box->addItem(tr("右转"));
    node->box->setCurrentIndex(index-110);
    node->setDirection();
    connect(node->box,SIGNAL(currentIndexChanged(int)),node,SLOT(setDirection()));

    node->controlsId=id;
    node->identifier="Turn";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";

    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateHover(QPointF point, int id)
{
    HoverNode *node=new HoverNode;

    QGraphicsItem *lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(0,-10);

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 + 24,
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2 + 3)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 + 24,
                      (node->pos().y() - node->outlineRect().height()/2 - node->yuan2->boundingRect().height()/2 - 3)));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->controlsId=id;
    node->identifier="Hover";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";

    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateDelay(QPointF point, int id)
{
    DelayNode *node=new DelayNode;

    QGraphicsItem *lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(0,-10);

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 + 24,
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2 + 3)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 + 24,
                      (node->pos().y() - node->outlineRect().height()/2 - node->yuan2->boundingRect().height()/2 - 3)));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->controlsId=id;
    node->identifier="Delay";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";

    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateVar(QPointF point, int id)
{ 
    VarNode *node = new VarNode;
    node->setPos(point);
//实现点击后产生一行的方式是：先把四行全部创建，但是最开始生成node时，只显示第一行，隐藏其他三行；之后点击按钮，使下一行显示
    QGraphicsItem *typeItem[4],*nameItem[4],
                  *valueItem[4],*buttonItem[4];

    QGraphicsItem *buttonItem0 = this->addWidget(node->button0);
    buttonItem0->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->button0Item = buttonItem0;
    node->button0Item->setPos(-108,-30);

    typeItem[0] = this->addWidget(node->typeBox[0]);
    typeItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->typeItem[0] = typeItem[0];
    node->typeItem[0]->setPos(-110,-3);

    nameItem[0] = this->addWidget(node->nameEdit[0]);
    nameItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->nameItem[0] = nameItem[0];
    node->nameItem[0]->setPos(-15,-3);

    valueItem[0] = this->addWidget(node->valueEdit[0]);
    valueItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->valueItem[0] = valueItem[0];
    node->valueItem[0]->setPos(41,-3);

    buttonItem[0] = this->addWidget(node->button[0]);
    buttonItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->buttonItem[0] = buttonItem[0];
    node->buttonItem[0]->setPos(92,-3);

    for(int i=1;i<4;i++)
    {    
        typeItem[i] = this->addWidget(node->typeBox[i]);
        typeItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->typeItem[i] = typeItem[i];
        //node->typeItem[i]->setPos(-110,7);
        node->typeItem[i]->setVisible(false);

        nameItem[i] = this->addWidget(node->nameEdit[i]);
        nameItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->nameItem[i] = nameItem[i];
        //node->nameItem[i]->setPos(-30,7);
        node->nameItem[i]->setVisible(false);

        valueItem[i] = this->addWidget(node->valueEdit[i]);
        valueItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->valueItem[i] = valueItem[i];
        //node->valueItem[i]->setPos(60,7);
        node->valueItem[i]->setVisible(false);

        buttonItem[i] = this->addWidget(node->button[i]);
        buttonItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->buttonItem[i] = buttonItem[i];
        //node->buttonItem[i]->setPos(100,12);
        node->button[i]->setVisible(false);
        node->buttonItem[i]->setZValue(buttonItem[0]->zValue()-1);
    }

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    emit sig_connectItem(node);

    //给所有comboBox加入内容
    for(int i = 0;i<4;i++)
    {
        node->typeBox[i]->addItem("int");
        node->typeBox[i]->addItem("double");
        node->typeBox[i]->addItem("float");
    }

    connect(node,SIGNAL(addVarSignal(VarNode*,QString,QString,QString)),
            this,SLOT(CreateVarInstance(VarNode*,QString,QString,QString)));


    node->controlsId=id;
    node->identifier="Var";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中

    return true;


    /*
    emit sig_connectItem(node);//不是我写的，不是我注释的，不知道干嘛用 gjf
    */
}

void newscene::CreateVarInstance(VarNode *node, QString Var, QString varName, QString varValue)
{
    VarInstanceNode *InstanceNode = new VarInstanceNode;
    InstanceNode->setPos(node->pos().x(),node->pos().y()+150);
    InstanceNode->varType=Var;
    InstanceNode->varName=varName;
    qDebug()<<InstanceNode->varName;
    InstanceNode->varValue=varValue.toDouble();
    qDebug()<<1;
    this->addItem(InstanceNode);

    InstanceNode->yuan->setPos(InstanceNode->pos().x() + InstanceNode->outlineRect().width()/2 + InstanceNode->yuan2->boundingRect().width()/2,
                            InstanceNode->pos().y());
    InstanceNode->yuan2->setPos(InstanceNode->pos().x() - InstanceNode->outlineRect().width()/2 - InstanceNode->yuan2->boundingRect().width()/2,
                             InstanceNode->pos().y());
    this->addItem(InstanceNode->yuan);
    this->addItem(InstanceNode->yuan2);

    int id = ++(this->VarInstanceNodeNum);
    InstanceNode->controlsId=id;
    InstanceNode->identifier="VarInstance";
    QString cid = QString::number(InstanceNode->controlsId,10);
    InstanceNode->name = InstanceNode->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<InstanceNode->name;
    qDebug()<<"identifier :"<<InstanceNode->identifier;
    qDebug()<<"controlsId :"<<InstanceNode->controlsId;
    WidgetWrap* tmp = new WidgetWrap(InstanceNode);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    InstanceNode->yuan2->master=tmp;
    InstanceNode->yuan2->name = "yuan2";

    return;
}


//bool newscene::CreateVarInstance(QPointF point, int id)
//{
//    VarInstanceNode* vdn=new VarInstanceNode;
//    //vdn->setPos(point);

//        QList<QGraphicsItem *> items = this->selectedItems();
//        if(items.count()==0)
//        {
//            vdn->setPos(point);
//            this->addItem(vdn);
//            vdn->node=0;
//            vdn->yuan2->setPos(vdn->pos().x(),
//                               vdn->pos().y() - 16 - vdn->yuan2->boundingRect().height()/2);
//            vdn->yuan->setPos(vdn->pos().x(),
//                               vdn->pos().y() + 16 + vdn->yuan->boundingRect().height()/2);
//            this->addItem(vdn->yuan);
//            this->addItem(vdn->yuan2);
//        }
//        else if(items.count()==1&&dynamic_cast<VarNode*>(items.first()))
//        {
//            VarNode* node=dynamic_cast<VarNode*>(items.first());
//            if(!node)return false;
//            int flag=0;
//            while(node->flags[node->num])//这个位置已经有了VarInstancenode
//            {
//                if(flag==6)return false;
//                node->num=node->num%6+1;
//                flag++;
//            }
//            //计算添加的位置
//            int i=node->num%3;
//            int j;
//            if(node->num==0||node->num==2)j=-17;
//            else if(node->num==3||node->num==5)j=17;
//            else if(node->num==1)j=-35;
//            else j=35;

//            node->array[node->num]->node=node;//使VarInstancenode知道它属于varnode

//            int x = node->pos().x() + (1-i)*30;
//            int y = node->pos().y() + j;
//            /*(node->array[node->num])->setPos(x,y);
//            vdn = node->array[node->num];    //在这里记录VarInstance，最后包装、添加到map*/
//            vdn->setPos(x,y);
//            node->array[node->num] = vdn;
//            vdn->seq = node->num;
//            node->flags[node->num]=true;
//            //this->addItem(node->array[node->num]);
//            this->addItem(vdn);
//            node->num=node->num%6+1;
//            //vdn->setFlag(QGraphicsItem::ItemIsMovable,false);
//        }

//    vdn->controlsId=id;
//    vdn->identifier="VarInstance";
//    QString cid = QString::number(vdn->controlsId,10);
//    vdn->name = vdn->identifier + cid;
//    qDebug()<<"Create():";
//    qDebug()<<"name :"<<vdn->name;
//    qDebug()<<"identifier :"<<vdn->identifier;
//    qDebug()<<"controlsId :"<<vdn->controlsId;
//    WidgetWrap* tmp = new WidgetWrap(vdn);   //包装节点
//    wm->add(tmp);            //添加到widgetmap中
//    vdn->yuan2->master = tmp;
//    vdn->yuan2->name = "yuan2";
//    vdn->yuan->master = tmp;
//    vdn->yuan->name = "yuan";

//    emit sig_connectItem(vdn);

//    return true;
//}

bool newscene::CreateCompute(QPointF point, int id, int selected_Index)
{
    ComputeNode *node=new ComputeNode;
    node->setPos(point);

    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-22,-14);

    QGraphicsItem* lineItem1 = this->addWidget(node->lineEdit1);
    lineItem1->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem1 = lineItem1;
    node->lineItem1->setPos(-72,-10);
    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(36,-10);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 70,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 70,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    node->box->addItem(tr("+"));
    node->box->addItem(tr("-"));
    node->box->addItem(tr("*"));
    node->box->addItem(tr("/"));
    node->box->addItem(tr("="));
    node->box->addItem(tr(">"));
    node->box->addItem(tr("<"));
    node->box->setCurrentIndex(selected_Index-301);
    //node->setText(tr(c[selected_Index-301]));


    node->controlsId=id;
    node->identifier="Compute";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";


    emit sig_connectItem(node);
    return true;
}

bool newscene::CreateSin(QPointF point, int id)
{
    qDebug()<<"use this";
    sinNode *node=new sinNode;
    node->setPos(point);

    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-71,-14);

    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(3,-10);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 65,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 65,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    node->box->addItem(tr("cos"));
    node->box->addItem(tr("sin"));
    node->box->addItem(tr("tan"));
    node->box->setCurrentIndex(0);

    emit sig_connectItem(node);


    node->controlsId=id;
    node->identifier="Sin";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";

    return true;
}

bool newscene::CreateLog(QPointF point, int id)
{
    logNode *node=new logNode;
    node->setPos(point);

    QGraphicsItem* lineItem1 = this->addWidget(node->lineEdit1);
    lineItem1->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem1 = lineItem1;
    node->lineItem1->setPos(-23,-1);
    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(4,-10);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 60,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 60,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    emit sig_connectItem(node);


    node->controlsId=id;
    node->identifier="Log";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";

    return true;
}

bool newscene::CreateE(QPointF point, int id)
{
    eNode *node=new eNode;
    node->setPos(point);

    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(-11,-13);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 40,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 40,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    emit sig_connectItem(node);

    node->controlsId=id;
    node->identifier="E";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan3->master = tmp;
    node->yuan3->name = "yuan3";
    wm->add(tmp);            //添加到widgetmap中

    return true;
}


bool newscene::CreateIO(QPointF point, int id)
{
    IoNode* node=new IoNode;
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;


    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);


    item->setPos(QPointF(node->pos().x()-node->outlineRect().width()/2,
                 (node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("detection sensor"));
    node->box->addItem(tr("A sensor"));
    node->box->addItem(tr("B sensor"));
    node->box->addItem(tr("delay"));
    node->box->setCurrentIndex(0);

    node->controlsId=id;
    node->identifier="IO";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
    emit sig_connectItem(node);

    return true;

}

bool newscene::CreateBattery(QPointF point, int id)
{
    BatteryNode* node=new BatteryNode;

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x(),
                       (node->pos().y() - node->outlineRect().height()/2 - node->yuan->boundingRect().height()/2)));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    node->node1->setText(tr("电压"));
    node->node2->setText(tr("电流"));
    node->node3->setText(tr("剩余电量"));

//    item->setPos(QPointF(node->pos().x()-node->outlineRect().width()/2,
//                 (node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height())));
//    item->setZValue(node->zValue()+1);
    //node->box->addItem(tr("voltage"));
   // node->box->addItem(tr("current"));
    //node->box->addItem(tr("level"));
    //node->box->setCurrentIndex(0);

    node->controlsId=id;
    node->identifier="Battery";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
    emit sig_connectItem(node);

    return true;

}

bool newscene::CreateGimbal(QPointF point, int id)
{
    GimbalNode* node=new GimbalNode;

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x(),
                      (node->pos().y() - node->outlineRect().height()/2 - node->yuan->boundingRect().height()/2)));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());

    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    node->node1->setText(tr("俯仰"));
    node->node2->setText(tr("横滚"));
    node->node3->setText(tr("偏航"));


    node->controlsId=id;
    node->identifier="Gimbal";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
    emit sig_connectItem(node);

    return true;

}

bool newscene::CreateAttitude(QPointF point, int id)
{
    AttitudeNode* node=new AttitudeNode;

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x(),
                      (node->pos().y() - node->outlineRect().height()/2 - node->yuan->boundingRect().height()/2)));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    node->node1->setText(tr("横滚"));
    node->node2->setText(tr("俯仰"));
    node->node3->setText(tr("偏航"));

    node->controlsId=id;
    node->identifier="Attitude";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
    emit sig_connectItem(node);

    return true;

}

bool newscene::CreateChannel(QPointF point, int id)
{
    ChannelNode* node=new ChannelNode;

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x(),
                      (node->pos().y() - node->outlineRect().height()/2 - node->yuan->boundingRect().height()/2)));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node1->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node1->outlineRect().width()/2,
                        node->pos().y() - node->outlineRect().height()/2 + node->node1->outlineRect().height()/2);
    node->node2->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + node->node1->outlineRect().height());
    node->node3->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 2*node->node1->outlineRect().height());
    node->node4->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 3*node->node1->outlineRect().height());
    node->node5->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 4*node->node1->outlineRect().height());
    node->node6->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 5*node->node1->outlineRect().height());
    node->node7->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 6*node->node1->outlineRect().height());
    node->node8->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 7*node->node1->outlineRect().height());
    node->node9->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 8*node->node1->outlineRect().height());

    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node4);
    this->addItem(node->node5);
    this->addItem(node->node6);
    this->addItem(node->node7);
    this->addItem(node->node8);
    this->addItem(node->node9);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    this->addItem(node->node4->yuan);
    this->addItem(node->node5->yuan);
    this->addItem(node->node6->yuan);
    this->addItem(node->node7->yuan);
    this->addItem(node->node8->yuan);
    this->addItem(node->node9->yuan);
    node->node1->setText(tr("通道一"));
    node->node2->setText(tr("通道二"));
    node->node3->setText(tr("通道三"));
    node->node4->setText(tr("通道四"));
    node->node5->setText(tr("通道五"));
    node->node6->setText(tr("通道六"));
    node->node7->setText(tr("通道七"));
    node->node8->setText(tr("通道八"));
    node->node9->setText(tr("通道九"));

    node->controlsId=id;
    node->identifier="Channel";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node1->yuan->master = tmp;
    node->node1->yuan->name = "n1yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    node->node3->yuan->master = tmp;
    node->node3->yuan->name = "n3yuan";
    node->node4->yuan->master = tmp;
    node->node4->yuan->name = "n4yuan";
    node->node5->yuan->master = tmp;
    node->node5->yuan->name = "n5yuan";
    node->node6->yuan->master = tmp;
    node->node6->yuan->name = "n6yuan";
    node->node7->yuan->master = tmp;
    node->node7->yuan->name = "n7yuan";
    node->node8->yuan->master = tmp;
    node->node8->yuan->name = "n8yuan";
    node->node9->yuan->master = tmp;
    node->node9->yuan->name = "n9yuan";
    emit sig_connectItem(node);

    return true;

}

bool newscene::CreateRangeFinder(QPointF point, int id)
{
    RangeFinderNode* node=new RangeFinderNode;

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x(),
                       (node->pos().y() - node->outlineRect().height()/2 - node->yuan->boundingRect().height()/2)));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());

    this->addItem(node->node2);
    this->addItem(node->node2->yuan);
    node->node2->setText(tr("距离"));


    node->controlsId=id;
    node->identifier="RangeFinder";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    node->node2->yuan->master = tmp;
    node->node2->yuan->name = "n2yuan";
    emit sig_connectItem(node);

    return true;

}


bool newscene::CreateLogic(QPointF point, int id)
{
    /*Rec *rec=new Rec;
    QGraphicsItem* item= this->addWidget(rec->box);
    rec->item=item;

    rec->setPos(point);
    rec->setxy(point);
    this->addItem(rec);
    this->clearSelection();
    rec->setSelected(true);*/

    /*rec->yuan2->setPos(QPointF(rec->pos().x() - rec->outlineRect().height()/2 + item->boundingRect().width()/2,
                               rec->pos().y() - rec->outlineRect().height()/2 +item->boundingRect().height()*1.5));
    rec->yuan->setPos(QPointF(rec->pos().x(), rec->pos().y() + rec->outlineRect().height()*0.5));*/
    /*rec->yuan->setPos(QPointF(rec->pos().x(),rec->pos().y() + rec->boundingRect().height()*0.5));
    rec->yuan2->setPos(QPointF(rec->pos().x() - rec->boundingRect().height()/2 + item->boundingRect().width()/2,
                               rec->pos().y() - rec->boundingRect().height()/2 + item->boundingRect().height()*1.5));
    this->addItem(rec->yuan2);
    this->addItem(rec->yuan);

    item->setPos(QPointF(rec->pos().x()-rec->boundingRect().width()/2,
                         (rec->pos().y() - rec->boundingRect().height()/2)));
    item->setZValue(rec->zValue()+1);
    rec->box->addItem(tr("if"));
    rec->box->addItem(tr("else"));
    rec->box->addItem(tr("while"));
    rec->box->setCurrentIndex(0);

    rec->controlsId=id;
    rec->identifier="Logic";
    QString cid = QString::number(rec->controlsId,10);
    rec->name = rec->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<rec->name;
    qDebug()<<"identifier :"<<rec->identifier;
    qDebug()<<"controlsId :"<<rec->controlsId;
    WidgetWrap* tmp = new WidgetWrap(rec);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    rec->yuan2->master = tmp;
    rec->yuan2->name = "yuan2";

    emit sig_connectItem(rec);

    LOGIC_Help* lh = new LOGIC_Help(rec);   //创建对应的工具对象
    LHM->insert(rec->name,lh);  //添加到logic工具对象的map中
    return true;*/
    return true;
}

bool newscene::CreateIf(QPointF point, int id)
{
    Rec *rec=new Rec;
    rec->setLogicFlag(1);

    rec->setPos(point);
    rec->setxy(point);
    this->addItem(rec);
    this->clearSelection();
    rec->setSelected(true);

    rec->yuan->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                              rec->pos().y() + rec->boundingRect().height()/2));
    rec->yuan2->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                               rec->pos().y() - rec->boundingRect().height()/2));
    rec->yuan3->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                               rec->boundingRect().topLeft().y() + 51));
    rec->yuan4->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                               rec->boundingRect().topLeft().y() + 66));
    rec->yuan5->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                               rec->boundingRect().bottomLeft().y() - rec->yuan5->boundingRect().height()/2));
    rec->yuan7->setPos(QPointF(rec->boundingRect().topRight().x() + rec->yuan7->boundingRect().width()/2,
                               rec->boundingRect().topRight().y() + 20));
    this->addItem(rec->yuan2);
    this->addItem(rec->yuan);
    this->addItem(rec->yuan3);
    this->addItem(rec->yuan4);
    this->addItem(rec->yuan5);
    this->addItem(rec->yuan7);

    rec->controlsId=id;
    rec->identifier="If";
    QString cid = QString::number(rec->controlsId,10);
    rec->name = rec->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<rec->name;
    qDebug()<<"identifier :"<<rec->identifier;
    qDebug()<<"controlsId :"<<rec->controlsId;
    WidgetWrap* tmp = new WidgetWrap(rec);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    rec->yuan->master = tmp;
    rec->yuan->name = "yuan";
    rec->yuan2->master = tmp;
    rec->yuan2->name = "yuan2";
    rec->yuan3->master = tmp;
    rec->yuan3->name = "yuan3";
    rec->yuan4->master = tmp;
    rec->yuan4->name = "yuan4";
    rec->yuan5->master = tmp;
    rec->yuan5->name = "yuan5";
    rec->yuan6->master = tmp;
    rec->yuan6->name = "yuan6";
    rec->yuan7->master = tmp;
    rec->yuan7->name = "yuan7";

    emit sig_connectItem(rec);

    LOGIC_Help* lh = new LOGIC_Help(rec);   //创建对应的工具对象
    LHM->insert(rec->name,lh);  //添加到logic工具对象的map中
    return true;
}

bool newscene::CreateElse(QPointF point, int id)
{
    Rec *rec=new Rec;
    rec->setLogicFlag(2);

    rec->setPos(point);
    rec->setxy(point);
    this->addItem(rec);
    this->clearSelection();
    rec->setSelected(true);

    rec->yuan->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                              rec->pos().y() + rec->boundingRect().height()/2));
    rec->yuan2->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                               rec->pos().y() - rec->boundingRect().height()/2));
    rec->yuan3->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                               rec->boundingRect().topLeft().y() + 51));
    rec->yuan4->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                               rec->boundingRect().topLeft().y() + 66));
    rec->yuan5->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                               rec->boundingRect().bottomLeft().y() - rec->yuan5->boundingRect().height()/2));
    rec->yuan6->setPos(QPointF(rec->boundingRect().topRight().x() - rec->yuan7->boundingRect().width()/2,
                               rec->boundingRect().topRight().y() + 20));
    this->addItem(rec->yuan2);
    this->addItem(rec->yuan);
    this->addItem(rec->yuan3);
    this->addItem(rec->yuan4);
    this->addItem(rec->yuan5);
    this->addItem(rec->yuan6);


    rec->controlsId=id;
    rec->identifier="Else";
    QString cid = QString::number(rec->controlsId,10);
    rec->name = rec->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<rec->name;
    qDebug()<<"identifier :"<<rec->identifier;
    qDebug()<<"controlsId :"<<rec->controlsId;
    WidgetWrap* tmp = new WidgetWrap(rec);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    rec->yuan->master = tmp;
    rec->yuan->name = "yuan";
    rec->yuan2->master = tmp;
    rec->yuan2->name = "yuan2";
    rec->yuan3->master = tmp;
    rec->yuan3->name = "yuan3";
    rec->yuan4->master = tmp;
    rec->yuan4->name = "yuan4";
    rec->yuan5->master = tmp;
    rec->yuan5->name = "yuan5";
    rec->yuan6->master = tmp;
    rec->yuan6->name = "yuan6";
    rec->yuan7->master = tmp;
    rec->yuan7->name = "yuan7";

    emit sig_connectItem(rec);

    LOGIC_Help* lh = new LOGIC_Help(rec);   //创建对应的工具对象
    LHM->insert(rec->name,lh);  //添加到logic工具对象的map中
    return true;
}

bool newscene::CreateWhile(QPointF point, int id)
{
    Rec *rec=new Rec;
    rec->setLogicFlag(3);

    rec->setPos(point);
    rec->setxy(point);
    this->addItem(rec);
    this->clearSelection();
    rec->setSelected(true);

    rec->yuan->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                              rec->pos().y() + rec->boundingRect().height()/2));
    rec->yuan2->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                               rec->pos().y() - rec->boundingRect().height()/2));
    rec->yuan3->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                               rec->boundingRect().topLeft().y() + 51));
    rec->yuan4->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                               rec->boundingRect().topLeft().y() + 66));
    rec->yuan5->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                               rec->boundingRect().bottomLeft().y() - rec->yuan5->boundingRect().height()/2));
    this->addItem(rec->yuan2);
    this->addItem(rec->yuan);
    this->addItem(rec->yuan3);
    this->addItem(rec->yuan4);
    this->addItem(rec->yuan5);

    rec->controlsId=id;
    rec->identifier="While";
    QString cid = QString::number(rec->controlsId,10);
    rec->name = rec->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<rec->name;
    qDebug()<<"identifier :"<<rec->identifier;
    qDebug()<<"controlsId :"<<rec->controlsId;
    WidgetWrap* tmp = new WidgetWrap(rec);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    rec->yuan->master = tmp;
    rec->yuan->name = "yuan";
    rec->yuan2->master = tmp;
    rec->yuan2->name = "yuan2";
    rec->yuan3->master = tmp;
    rec->yuan3->name = "yuan3";
    rec->yuan4->master = tmp;
    rec->yuan4->name = "yuan4";
    rec->yuan5->master = tmp;
    rec->yuan5->name = "yuan5";
    rec->yuan6->master = tmp;
    rec->yuan6->name = "yuan6";
    rec->yuan7->master = tmp;
    rec->yuan7->name = "yuan7";

    emit sig_connectItem(rec);

    LOGIC_Help* lh = new LOGIC_Help(rec);   //创建对应的工具对象
    LHM->insert(rec->name,lh);  //添加到logic工具对象的map中
    return true;
}


Link* newscene::CreateLink(QGraphicsSceneMouseEvent* event)
{//生成link并包装，记录
    Link* new_link = 0;
    if(new_yuan->myLinks.size()!=0)
    {
        foreach (Link* link,new_yuan->myLinks)
         delete link;
       if((this->selectedItems().count()==1)&&dynamic_cast<triYuan *>(this->selectedItems()[0])!=0)
        {
           if(new_yuan->collidingItems().count()>=1)
           {
              for(int i=0;i<new_yuan->collidingItems().count();i++)
              {
                  if(dynamic_cast<Yuan *>(new_yuan->collidingItems()[i])!=0&&dynamic_cast<triYuan *>(new_yuan->collidingItems()[i])==0)
                  {
                      new_link=new Link(dynamic_cast<triYuan *>(this->selectedItems().first()),
                                        dynamic_cast<Yuan *>(new_yuan->collidingItems()[i]));
                      new_link->setZValue(100);
                      this->addItem(new_link);

                      this->linkNodeNum++;
                      new_link->controlsId = this->linkNodeNum;
                      new_link->identifier = "Link";
                      QString cid = QString::number(new_link->controlsId,10);
                      new_link->name = new_link->identifier + cid;
                      WidgetWrap* tmp = new WidgetWrap(new_link);   //包装节点
                      wm->add(tmp);
                      qDebug()<<"scene::mouseMoveEvent(): ";
                      qDebug()<<"type: "<<new_link->identifier;
                      qDebug()<<"id: "<<new_link->controlsId;
                      qDebug()<<"name: "<<new_link->name;
                      break;

                  }
              }
            }

        }
        new_yuan->setPos(0,0);
    }

    return new_link;
}

//----------------从xml文件创建控件-----------------------
bool newscene::CreateTakeOff(TakeOffNode* node)
{
    QGraphicsItem *lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(0,-10);

    QString tmp = QString::number(node->altitude,10,2);
    node->lineEdit->setText(tmp);
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF((node->pos().x()),(node->pos().y() + node->outlineRect().height()/2)+node->yuan->boundingRect().height()/2));
    this->addItem(node->yuan);
    this->takeoffNodeNum++;
     //node->set_master(tmp);

    emit sig_connectItem(node);

    return true;

}
bool newscene::CreateLand(LandNode* node)
{
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan2->setPos(QPointF((node->pos().x()),
                       (node->pos().y() - node->outlineRect().height()/2)-node->yuan2->boundingRect().height()/2));
    this->addItem(node->yuan2);
     this->landonNodeNum++;

    emit sig_connectItem(node);

    return true;
}
bool newscene::CreateGo(GoNode* node)
{
    //对node的图形属性（估计是box）初始化的时候，会导致direction也被初始化，所以先记录正确的direction以备之后还原
    QString direction = node->direction;

    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-175,-16);

    QGraphicsItem* lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(-25,-10);
    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(125,-10);

    QString tmp = QString::number(node->groundspeed,10,2);
    node->lineEdit->setText(tmp);
    tmp = QString::number(node->Time,10,2);
    node->lineEdit2->setText(tmp);

    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                      (node->pos().y() )));

    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->box->addItem(tr("向上"));
    node->box->addItem(tr("向下"));
    node->box->addItem(tr("前进"));
    node->box->addItem(tr("后退"));
    node->box->addItem(tr("向右"));
    node->box->addItem(tr("向左"));
    node->direction = direction;//还原正确的direction
    if(node->direction=="GoUp"){
        node->box->setCurrentIndex(0);
    }else if(node->direction=="GoDown"){
        node->box->setCurrentIndex(1);
    }else if(node->direction=="Forward"){
        node->box->setCurrentIndex(2);
    }else if(node->direction=="Backward"){
        node->box->setCurrentIndex(3);
    }else if(node->direction=="GoRight"){
        node->box->setCurrentIndex(4);
    }else if(node->direction=="GoLeft"){
        node->box->setCurrentIndex(5);
    }
    connect(node->box,SIGNAL(currentIndexChanged(int)),node,SLOT(setDirection()));
    this->GoNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    emit sig_connectItem(node);

    return true;
}
bool newscene::CreateTurn(TurnNode* node)
{
    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-100,-16);

    QGraphicsItem *lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(50,-10);

    QString tmp = QString::number(node->Angel,10,2);
    node->lineEdit->setText(tmp);

    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                      (node->pos().y() )));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("左转"));
    node->box->addItem(tr("右转"));
    if(node->direction=="TurnLeft"){
        node->box->setCurrentIndex(0);
    }else if(node->direction=="TurnRight"){
        node->box->setCurrentIndex(1);
    }
    connect(node->box,SIGNAL(currentIndexChanged(int)),node,SLOT(setDirection()));
    this->TurnNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    emit sig_connectItem(node);

    return true;
}
bool newscene::CreateHover(HoverNode* node)
{
    QGraphicsItem *lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(0,-10);

    QString tmp = QString::number(node->time,10,2);
    node->lineEdit->setText(tmp);
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                      (node->pos().y() )));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    this->HoverNodeNum++;

    emit sig_connectItem(node);

    return true;
}
bool newscene::CreateDelay(DelayNode *node)
{
    QGraphicsItem *lineItem = this->addWidget(node->lineEdit);
    lineItem->setParentItem(dynamic_cast<QGraphicsItem*>(node));//设置父对象，这样就可以自动跟随父对象改变位置
    node->lineItem = lineItem;
    node->lineItem->setPos(0,-10);

    QString tmp = QString::number(node->time,10,2);
    node->lineEdit->setText(tmp);
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                      (node->pos().y() )));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->DelayNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    emit sig_connectItem(node);

    return true;
}
bool newscene::CreateVar(VarNode* node)
{
    node->setPos(node->lx,node->ly);
//实现点击后产生一行的方式是：先把四行全部创建，但是最开始生成node时，只显示第一行，隐藏其他三行；之后点击按钮，使下一行显示
    QGraphicsItem *typeItem[4],*nameItem[4],
                  *valueItem[4],*buttonItem[4];

    QGraphicsItem *buttonItem0 = this->addWidget(node->button0);
    buttonItem0->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->button0Item = buttonItem0;
    node->button0Item->setPos(-108,-30);

    typeItem[0] = this->addWidget(node->typeBox[0]);
    typeItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->typeItem[0] = typeItem[0];
    node->typeItem[0]->setPos(-110,-3);

    nameItem[0] = this->addWidget(node->nameEdit[0]);
    nameItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->nameItem[0] = nameItem[0];
    node->nameItem[0]->setPos(-15,-3);

    valueItem[0] = this->addWidget(node->valueEdit[0]);
    valueItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->valueItem[0] = valueItem[0];
    node->valueItem[0]->setPos(41,-3);

    buttonItem[0] = this->addWidget(node->button[0]);
    buttonItem[0]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->buttonItem[0] = buttonItem[0];
    node->buttonItem[0]->setPos(92,-3);

    for(int i=1;i<4;i++)
    {
        typeItem[i] = this->addWidget(node->typeBox[i]);
        typeItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->typeItem[i] = typeItem[i];
        //node->typeItem[i]->setPos(-110,7);
        node->typeItem[i]->setVisible(false);

        nameItem[i] = this->addWidget(node->nameEdit[i]);
        nameItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->nameItem[i] = nameItem[i];
        //node->nameItem[i]->setPos(-30,7);
        node->nameItem[i]->setVisible(false);

        valueItem[i] = this->addWidget(node->valueEdit[i]);
        valueItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->valueItem[i] = valueItem[i];
        //node->valueItem[i]->setPos(60,7);
        node->valueItem[i]->setVisible(false);

        buttonItem[i] = this->addWidget(node->button[i]);
        buttonItem[i]->setParentItem(dynamic_cast<QGraphicsItem*>(node));
        node->buttonItem[i] = buttonItem[i];
        //node->buttonItem[i]->setPos(100,12);
        node->button[i]->setVisible(false);
        node->buttonItem[i]->setZValue(buttonItem[0]->zValue()-1);
    }

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    //给所有comboBox加入内容
    for(int i = 0;i<4;i++)
    {
        node->typeBox[i]->addItem("int");
        node->typeBox[i]->addItem("double");
        node->typeBox[i]->addItem("float");
    }

    connect(node,SIGNAL(addVarSignal(VarNode*,QString,QString,QString)),
            this,SLOT(CreateVarInstance(VarNode*,QString,QString,QString)));

    emit sig_connectItem(node);


    if(node->type[0]=="int")
        node->typeBox[0]->setCurrentIndex(0);
    if(node->type[0]=="double")
        node->typeBox[0]->setCurrentIndex(1);
    if(node->type[0]=="float")
        node->typeBox[0]->setCurrentIndex(2);
    node->nameEdit[0]->setText(node->vname[0]);
    node->valueEdit[0]->setText(node->value[0]);
    int amount = node->getvarnum();
    node->setvarnum(1);

    for(int i=1;i<amount;i++){
        node->button0->click();
        if(node->type[i]=="int")
            node->typeBox[i]->setCurrentIndex(0);
        if(node->type[i]=="double")
            node->typeBox[i]->setCurrentIndex(1);
        if(node->type[i]=="float")
            node->typeBox[i]->setCurrentIndex(2);
        node->nameEdit[i]->setText(node->vname[i]);
        node->valueEdit[i]->setText(node->value[i]);
//        node->button[i]->setVisible(true);
//        node->typeItem[i]->setVisible(true);
//        node->nameItem[i]->setVisible(true);
//        node->valueItem[i]->setVisible(true);
    }

    node->controlsId=node->controlsId;
    node->identifier="Var";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中

    return true;
}
bool newscene::CreateVarInstance(VarInstanceNode* vdn)
{   //先设定不论从哪里生成控件都会需要的公共的属性,从xml生成的create系列的思想
    vdn->setPos(vdn->lx,vdn->ly);
    this->addItem(vdn);
    vdn->yuan->setPos(vdn->pos().x() + vdn->outlineRect().width()/2 + vdn->yuan2->boundingRect().width()/2,
                            vdn->pos().y());
    vdn->yuan2->setPos(vdn->pos().x() - vdn->outlineRect().width()/2 - vdn->yuan2->boundingRect().width()/2,
                             vdn->pos().y());
    this->addItem(vdn->yuan);
    this->addItem(vdn->yuan2);
    vdn->varName = vdn->varName;
    this->VarInstanceNodeNum++;

    qDebug()<<"Create(xml):";
    qDebug()<<"name :"<<vdn->name;
    qDebug()<<"identifier :"<<vdn->identifier;
    qDebug()<<"controlsId :"<<vdn->controlsId;
//    emit sig_connectItem(vdn);

    return true;
}
bool newscene::CreateCompute(ComputeNode *node)
{
    node->setPos(node->lx,node->ly);

    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-22,-14);

    QGraphicsItem* lineItem1 = this->addWidget(node->lineEdit1);
    lineItem1->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem1 = lineItem1;
    node->lineItem1->setPos(-72,-10);
    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(36,-10);
    node->lineEdit1->setText(node->left);
    node->lineEdit2->setText(node->right);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 70,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 70,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    node->box->addItem(tr("+"));
    node->box->addItem(tr("-"));
    node->box->addItem(tr("*"));
    node->box->addItem(tr("/"));
    node->box->addItem(tr("="));
    node->box->addItem(tr(">"));
    node->box->addItem(tr("<"));
    if(node->oprt=="+")
        node->box->setCurrentIndex(0);
    if(node->oprt=="-")
        node->box->setCurrentIndex(1);
    if(node->oprt=="*")
        node->box->setCurrentIndex(2);
    if(node->oprt=="/")
        node->box->setCurrentIndex(3);
    if(node->oprt=="=")
        node->box->setCurrentIndex(4);
    if(node->oprt==">")
        node->box->setCurrentIndex(5);
    if(node->oprt=="<")
        node->box->setCurrentIndex(6);

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    this->ComputeNodeNum++;

    emit sig_connectItem(node);


//    node->setText(tr("Compute"));
//    QGraphicsItem* item=this->addWidget(node->box);
//    node->item=item;
//    node->setPos(node->lx,node->ly);
//    this->addItem(node);
//    this->clearSelection();
//    node->setSelected(true);
//    bringToFront();

//    node->yuan->setPos(QPointF(node->pos().x(),
//                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
//    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2
//                                - node->rect1->boundingRect().width(),
//                                node->pos().y()));
//    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->outlineRect().width()/2
//                                + node->rect2->boundingRect().width(),
//                                node->pos().y()));
//    this->addItem(node->yuan);
//    this->addItem(node->yuan2);
//    this->addItem(node->yuan3);

//    node->rect1->setPos(QPointF(node->pos().x() - node->boundingRect().width(),
//                                node->pos().y()));
//    node->rect2->setPos(QPointF(node->pos().x() + node->boundingRect().width(),
//                                node->pos().y()));
//    addItem(node->rect1);
//    addItem(node->rect2);

//    item->setPos(QPointF(node->pos().x()- item->boundingRect().width()/2,
//                 node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height()));
//    item->setZValue(node->zValue()+1);
//    node->box->addItem(tr("+"));
//    node->box->addItem(tr("-"));
//    node->box->addItem(tr("*"));
//    node->box->addItem(tr("/"));
//    node->box->addItem(tr("cos"));
//    node->box->addItem(tr("sin"));
//    node->box->addItem(tr("tan"));
//    node->box->addItem(tr("log"));
//    node->box->addItem(tr("e"));
//    node->box->addItem(tr("="));
//    node->box->addItem(tr(">"));
//    node->box->addItem(tr("<"));


//    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateE(eNode *node)
{
    node->setPos(node->lx,node->ly);

    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(-11,-13);
    node->lineEdit2->setText(node->right);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 40,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 40,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    this->ComputeNodeNum++;

    emit sig_connectItem(node);
    return true;
}

bool newscene::CreateLog(logNode *node)
{
    node->setPos(node->lx,node->ly);

    QGraphicsItem* lineItem1 = this->addWidget(node->lineEdit1);
    lineItem1->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem1 = lineItem1;
    node->lineItem1->setPos(-23,-1);
    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(4,-10);
    node->lineEdit1->setText(node->left);
    node->lineEdit2->setText(node->right);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 40,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 40,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    this->ComputeNodeNum++;

    emit sig_connectItem(node);
    return true;
}

bool newscene::CreateSin(sinNode *node)
{
    node->setPos(node->lx,node->ly);

    QGraphicsItem* item=this->addWidget(node->box);
    item->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->item=item;
    node->item->setPos(-71,-14);

    QGraphicsItem* lineItem2 = this->addWidget(node->lineEdit2);
    lineItem2->setParentItem(dynamic_cast<QGraphicsItem*>(node));
    node->lineItem2 = lineItem2;
    node->lineItem2->setPos(3,-10);
    node->lineEdit2->setText(node->right);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x() - 65,
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - 65,
                                node->pos().y() - node->outlineRect().height()/2 -node->yuan2->boundingRect().height()/2));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->boundingRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    node->box->addItem(tr("cos"));
    node->box->addItem(tr("sin"));
    node->box->addItem(tr("tan"));
    if(node->oprt=="cos")
        node->box->setCurrentIndex(0);
    if(node->oprt=="sin")
        node->box->setCurrentIndex(1);
    if(node->oprt=="tan")
        node->box->setCurrentIndex(2);

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    node->yuan2->master = tmp;
    node->yuan2->name = "yuan2";
    node->yuan->master = tmp;
    node->yuan->name = "yuan";
    this->ComputeNodeNum++;

    emit sig_connectItem(node);
    return true;
}

bool newscene::CreateIO(IoNode* node)
{
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;

    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    this->IONodeNum++;

    item->setPos(QPointF(node->pos().x()-node->outlineRect().width()/2,
                 (node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("detection sensor"));
    node->box->addItem(tr("A sensor"));
    node->box->addItem(tr("B sensor"));
    node->box->addItem(tr("delay"));
    node->box->setCurrentIndex(0);

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    emit sig_connectItem(node);

    return true;

}

bool newscene::CreateBattery(BatteryNode* node)
{
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    node->node1->setText(tr("电压"));
    node->node2->setText(tr("电流"));
    node->node3->setText(tr("剩余电量"));
    this->BatteryNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateAttitude(AttitudeNode* node)
{
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    node->node1->setText(tr("横滚"));
    node->node2->setText(tr("俯仰"));
    node->node3->setText(tr("偏航"));
    this->AttitudeNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateGimbal(GimbalNode* node)
{
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());
    node->node1->setPos(node->node2->pos().x(),
                        node->node2->pos().y() - node->node2->outlineRect().height());
    node->node3->setPos(node->node2->pos().x(),
                        node->node2->pos().y() + node->node2->outlineRect().height());
    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    node->node1->setText(tr("俯仰"));
    node->node2->setText(tr("横滚"));
    node->node3->setText(tr("偏航"));
    this->GimbalNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateChannel(ChannelNode* node)
{
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node1->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node1->outlineRect().width()/2,
                        node->pos().y() + node->outlineRect().height()/2);
    node->node2->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + node->node1->outlineRect().height());
    node->node3->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 2*node->node1->outlineRect().height());
    node->node4->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 3*node->node1->outlineRect().height());
    node->node5->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 4*node->node1->outlineRect().height());
    node->node6->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 5*node->node1->outlineRect().height());
    node->node7->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 6*node->node1->outlineRect().height());
    node->node8->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 7*node->node1->outlineRect().height());
    node->node9->setPos(node->node1->pos().x(),
                        node->node1->pos().y() + 8*node->node1->outlineRect().height());

    this->addItem(node->node2);
    this->addItem(node->node1);
    this->addItem(node->node3);
    this->addItem(node->node4);
    this->addItem(node->node5);
    this->addItem(node->node6);
    this->addItem(node->node7);
    this->addItem(node->node8);
    this->addItem(node->node9);
    this->addItem(node->node2->yuan);
    this->addItem(node->node1->yuan);
    this->addItem(node->node3->yuan);
    this->addItem(node->node4->yuan);
    this->addItem(node->node5->yuan);
    this->addItem(node->node6->yuan);
    this->addItem(node->node7->yuan);
    this->addItem(node->node8->yuan);
    this->addItem(node->node9->yuan);
    node->node1->setText(tr("通道一"));
    node->node2->setText(tr("通道二"));
    node->node3->setText(tr("通道三"));
    node->node4->setText(tr("通道四"));
    node->node5->setText(tr("通道五"));
    node->node6->setText(tr("通道六"));
    node->node7->setText(tr("通道七"));
    node->node8->setText(tr("通道八"));
    node->node9->setText(tr("通道九"));

    this->ChannelNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateRangeFinder(RangeFinderNode* node)
{
    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();


    node->yuan->setPos(QPointF(node->pos().x(),
                      (node->pos().y() + node->outlineRect().height()/2 + node->yuan->boundingRect().height()/2)));
    node->yuan2->setPos(QPointF(node->pos().x()- node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                       (node->pos().y())));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);

    node->node2->setPos(node->pos().x() + node->outlineRect().width()/2 + node->node2->outlineRect().width()/2,
                        node->pos().y());

    this->addItem(node->node2);
    this->addItem(node->node2->yuan);
    node->node2->setText(tr("距离"));
    this->RangeFinderNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
    emit sig_connectItem(node);

    return true;
}

bool newscene::CreateLogic(Rec *rec)
{
    rec->setBoundingRect(rec->wide,rec->high);
    rec->setPos(rec->lx,rec->ly);
    rec->setxy(rec->pos());    
    this->addItem(rec);
    this->clearSelection();
    rec->setSelected(true);

    if(rec->identifier=="If"){
        rec->yuan->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                  rec->pos().y() + rec->boundingRect().height()/2));
        rec->yuan2->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                                   rec->pos().y() - rec->boundingRect().height()/2));
        rec->yuan3->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                                   rec->boundingRect().topLeft().y() + 51));
        rec->yuan4->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                   rec->boundingRect().topLeft().y() + 66));
        rec->yuan5->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                   rec->boundingRect().bottomLeft().y() - rec->yuan5->boundingRect().height()/2));
        rec->yuan7->setPos(QPointF(rec->boundingRect().topRight().x() + rec->yuan7->boundingRect().width()/2,
                                   rec->boundingRect().topRight().y() + 20));
        this->addItem(rec->yuan2);
        this->addItem(rec->yuan);
        this->addItem(rec->yuan3);
        this->addItem(rec->yuan4);
        this->addItem(rec->yuan5);
        this->addItem(rec->yuan7);
        rec->setLogicFlag(1);
        rec->identifier="If";
    }
    if(rec->identifier=="Else"){
        rec->yuan->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                  rec->pos().y() + rec->boundingRect().height()/2));
        rec->yuan2->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                                   rec->pos().y() - rec->boundingRect().height()/2));
        rec->yuan3->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                                   rec->boundingRect().topLeft().y() + 51));
        rec->yuan4->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                   rec->boundingRect().topLeft().y() + 66));
        rec->yuan5->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                   rec->boundingRect().bottomLeft().y() - rec->yuan5->boundingRect().height()/2));
        rec->yuan6->setPos(QPointF(rec->boundingRect().topRight().x() - rec->yuan7->boundingRect().width()/2,
                                   rec->boundingRect().topRight().y() + 20));
        this->addItem(rec->yuan2);
        this->addItem(rec->yuan);
        this->addItem(rec->yuan3);
        this->addItem(rec->yuan4);
        this->addItem(rec->yuan5);
        this->addItem(rec->yuan6);
        rec->setLogicFlag(2);
        rec->identifier="Else";
    }
    if(rec->identifier=="While"){
        rec->yuan->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                  rec->pos().y() + rec->boundingRect().height()/2));
        rec->yuan2->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                                   rec->pos().y() - rec->boundingRect().height()/2));
        rec->yuan3->setPos(QPointF(rec->boundingRect().topLeft().x() + 67,
                                   rec->boundingRect().topLeft().y() + 51));
        rec->yuan4->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                   rec->boundingRect().topLeft().y() + 66));
        rec->yuan5->setPos(QPointF(rec->boundingRect().bottomLeft().x() + 67,
                                   rec->boundingRect().bottomLeft().y() - rec->yuan5->boundingRect().height()/2));
        this->addItem(rec->yuan2);
        this->addItem(rec->yuan);
        this->addItem(rec->yuan3);
        this->addItem(rec->yuan4);
        this->addItem(rec->yuan5);
        rec->setLogicFlag(3);
        rec->identifier="While";
    }
    //rec->setBoundingRect(rec->high,rec->wide);

    qDebug()<<"Create():";
    qDebug()<<"name :"<<rec->name;
    qDebug()<<"identifier :"<<rec->identifier;
    qDebug()<<"controlsId :"<<rec->controlsId;
    this->LogicNodeNum++;

    LOGIC_Help* lh = new LOGIC_Help(rec);   //创建对应的工具对象
    LHM->insert(rec->name,lh);  //添加到logic工具对象的map中

    emit sig_connectItem(rec);
    return true;
}
bool newscene::CreateWidgets()
{
    typename QMap<QString, widget*>::iterator iter;
    QMap<QString,WidgetWrap*> Map;
    WidgetWrap* ww;
    Map = wm->get_map();

    for(iter=Map.begin();iter!=Map.end();iter++){
        ww = iter.value();
        if(ww->identifier=="TakeOff"){
            //QPointF point(iter->mTakeOffNode->lx,iter->mTakeOffNode->ly);
            CreateTakeOff(ww->mTakeOffNode);
        }
        if(ww->identifier=="Land"){
            //QPointF point(iter->mLandNode->lx,iter->mLandNode->ly);
            CreateLand(ww->mLandNode);
        }
        if(ww->identifier=="Go"){
            CreateGo(ww->mGoNode);
        }
        if(ww->identifier=="Turn"){
            //QPointF point(iter->mTurnNode->lx,iter->mTurnNode->ly);
            CreateTurn(ww->mTurnNode);
        }
        if(ww->identifier=="Hover"){
            //QPointF point(iter->mHoverNode->lx,iter->mHoverNode->ly);
            CreateHover(ww->mHoverNode);
        }
        if(ww->identifier=="Delay"){
            //QPointF point(iter->mDelayNode->lx,iter->mDelayNode->ly);
            CreateDelay(ww->mDelayNode);
        }
        if(ww->identifier=="Var"){
            //QPointF point(iter->mVarNode->lx,iter->mVarNode->ly);
            CreateVar(ww->mVarNode);
        }
        if(ww->identifier=="VarInstance"){
            //QPointF point(iter->mVarInstanceNode->lx,iter->mVarInstanceNode->ly);
            CreateVarInstance(ww->mVarInstanceNode);
        }
        if(ww->identifier=="Compute"){
            //QPointF point(iter->mComputeNode->lx,iter->mComputeNode->ly);
            CreateCompute(ww->mComputeNode);
        }
        if(ww->identifier=="E"){
            CreateE(ww->mENode);
        }
        if(ww->identifier=="Log"){
            CreateLog(ww->mLogNode);
        }
        if(ww->identifier=="Sin"){
            CreateSin(ww->mSinNode);
        }
        if(ww->identifier=="IO"){
            //QPointF point(iter->mIONode->lx,iter->mIONode->ly);
            CreateIO(ww->mIONode);
        }
        if(ww->identifier=="Battery"){
            CreateBattery(ww->mBatteryNode);
        }
        if(ww->identifier=="Attitude"){
            CreateAttitude(ww->mAttitudeNode);
        }
        if(ww->identifier=="Gimbal"){
            CreateGimbal(ww->mGimbalNode);
        }
        if(ww->identifier=="Channel"){
            CreateChannel(ww->mChannelNode);
        }
        if(ww->identifier=="RangeFinder"){
            CreateRangeFinder(ww->mRangeFinderNode);
        }
        if(ww->identifier=="If" || ww->identifier=="Else" || ww->identifier=="While"){
            CreateLogic(ww->mLogicNode);
        }
        if(ww->identifier=="Link"){
            CreateLink(ww->mLinkNode);
        }
    }
    return true;
}
bool newscene::CreateLink(Link* link)
{
    link->setZValue(100);
    this->addItem(link);
    this->linkNodeNum++;
    return true;
}
//------------------------------------------------------

Rec* newscene::check_in_Logic(WidgetWrap* tmp, QString operate,int maxrank)
{//这个函数给新建的控件定义了rank，并让所有包含该控件的logic记住了它
    bool flag;
    typename QMap<QString, LOGIC_Help*>::iterator iter;
    LOGIC_Help* lh;
    LOGIC* l = 0;   int r=1000;
    for(iter=LHM->begin();iter!=LHM->end();iter++){
        lh = iter.value();
        if(lh->LOG->rank<=maxrank) continue;    //遇到高级的图就略过，在限定等级的图中遍历
        flag = lh->in_LOGIC(tmp);   //logic不被自己包含


        qDebug()<<"scene::check_in_Logic():";
        qDebug()<<operate<<" "<<flag;

        if(flag){
            if(operate=="add"){
                lh->put_in_Logic(tmp);  //记录内部的widget
                tmp->rank(lh->LOG->rank+1); //设置rank
            }else if(operate=="del")
                lh->WidgetsInLOGIC.remove(tmp->name);
            if(r>lh->LOG->rank){    //向rank更小的图更新
                l = lh->LOG;
                r = lh->LOG->rank;
            }
        }

    }
    if(l==0)    tmp->rank(maxrank+1);   //如果在maxrank的等级下没有搜到对应的父logic，说明这个控件至少是maxrank的等级
    return l;
}

bool newscene::CheckInLogic()
{//生成控件时检验控件被哪些logic包含（也可能生成的是logic），综合了情况分类的工作。
 //相应的check_in_logic只负责纯粹的检测父logic的最外层
    QList<QGraphicsItem *> items = this->selectedItems();
    typename QList<QGraphicsItem *>::iterator liter;
    typename QMap<QString, widget*>::iterator miter;
    QMap<QString, widget*>& m = wm->get_map();

    for(liter=items.begin();liter!=items.end();liter++){
        QGraphicsItem * t = *liter;
        NewNode* n1 = dynamic_cast<NewNode *>(t);
        Node* n2 = dynamic_cast<Node *>(t);
        if(n1!=0){
            if(n1->identifier=="While"||
                    n1->identifier=="If"||
                    n1->identifier=="Else"){
                for(miter=m.begin();miter!=m.end();miter++){
                    //这个函数会遍历所有Logic进行检查，换言之，在上面整个for里，n1既检查了别人，也被其他logic检查了。
                    check_in_Logic(miter.value(),"add",0);
                }
            }else{
                for(miter=m.begin();miter!=m.end();miter++){
                    if(n1->name == miter.value()->name){
                        check_in_Logic(miter.value(),"add",0);
                    }
                }
            }

        }else if(n2!=0){
            for(miter=m.begin();miter!=m.end();miter++){
                if(n2->name == miter.value()->name){
                    check_in_Logic(miter.value(),"add",0);
                }
            }
        }
    }
    return true;
}

//这个函数暂时用不到了，但是重写一遍很麻烦，所以先注释着
//bool newscene::CheckLinkOverLogic(Link *link)
//{//说明：设定上，找一个控件的父logic只能找到最外层的那个，而一个link是否跨越logic，
// //     依靠所连接的控件的父logic的指针是否相同来判断。这导致只要在同一个大logic里，
// //     跨越子logic的link无法知道自己跨越了子logic，而这是遍历多层级的图需要的。
// //     因此采用了不断识别共同父logic，并设置搜索的rank等级的办法。
//    if(link==0) return false;
//    //若是从compute指向logic的情况
//    if(link->toYuan()->master->identifier=="Logic"){
//        link->toLogic = link->toYuan()->master->mLogicNode;
//        //flink记录的是跨越logic的link，所以compute指向logic不算
//        //link->toLogic->flink<<link;
//    }else{
//        Rec* rec1 = check_in_Logic(link->fromYuan()->master,"none",0);
//        Rec* rec2 = check_in_Logic(link->toYuan()->master,"none",0);
//        while(rec1==rec2){
//            if(rec1==0&&rec2==0) break;
//            rec1 = check_in_Logic(link->fromYuan()->master,"none",rec1->rank);
//            rec2 = check_in_Logic(link->toYuan()->master,"none",rec2->rank);
//        }
//        if(rec1!=rec2){
//            link->toLogic = rec2;
//            link->fromLogic = rec1;   //fromLogic其实用不到，顺手写了
//            if(rec1!=0&&rec2!=0){
//                rec1->tlink<<link; //从logic指出
//                rec2->flink<<link;
//            }
//            if(rec1!=0&&rec2==0){
//                rec1->tlink<<link; //从logic指出
//            }
//            if(rec1==0&&rec2!=0){
//                rec2->flink<<link;  //向logic指入
//            }
//            //不可能同时等于0

//        }
//    }

//    return true;
//}
