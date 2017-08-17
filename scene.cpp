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
#include "vardefnode.h"
#include "computenode.h"
#include "ionode.h"
#include "yuan.h"
#include "rec.h"
#include "propertiesdialog.h"
#include "itemtypes.h"
#include "map_instrument.h"

newscene::newscene(WidgetMap* m)
{
    new_yuan=new specialYuan;
    setSceneRect(QRectF(QPointF(0,0), QSize(1000, 1000)));

    need_to_set=0;
    selected_Index=1;

    VarTypeNodeNum = 0;  //计数varNode,命名每个varNode,下同
    VarDefNodeNum = 0;

    LogicNodeNum=0;

    takeoffNodeNum=0;
    landonNodeNum=0;
    GoNodeNum = 0;
    TurnNodeNum = 0;
    HoverNodeNum = 0;
    DelayNodeNum = 0;

    ComputeNodeNum = 0;

    IONodeNum = 0;

    linkNodeNum=0;

    wm=m;
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

    CreateLink(event);
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
    if(selected_Index==1&&need_to_set==1){
        emit itemInserted(selected_Index);
        this->takeoffNodeNum++;
        CreateTakeOff(new_event->scenePos(),this->takeoffNodeNum);

        need_to_set = 0;
}
    if(selected_Index==2&&need_to_set==1){
        emit itemInserted(selected_Index);
        this->landonNodeNum++;
        CreateLand(new_event->scenePos(),this->landonNodeNum);

        need_to_set = 0;
        //setCursor(Qt::ArrowCursor);
    }
    if((selected_Index>=3&&selected_Index<=9)&&need_to_set==1){
        emit itemInserted(selected_Index);
        this->GoNodeNum++;
        need_to_set = 0;
        CreateGo(new_event->scenePos(),this->GoNodeNum);
        //setCursor(Qt::ArrowCursor);
    }
    if(need_to_set==1&&selected_Index==10){qDebug()<<"10.";

    }
    if(need_to_set==1&&(selected_Index>=11&&selected_Index<=12)){qDebug()<<"11.12";
        emit itemInserted(selected_Index);
        this->TurnNodeNum++;
        need_to_set = 0;
        CreateTurn(new_event->scenePos(),this->TurnNodeNum);
    }
    if(need_to_set==1&&selected_Index==13){qDebug()<<"13.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->HoverNodeNum++;
        CreateHover(new_event->scenePos(),this->HoverNodeNum);
    }
    if(need_to_set==1&&selected_Index==14){qDebug()<<"14.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->DelayNodeNum++;
        CreateDelay(new_event->scenePos(),this->DelayNodeNum);
    }
    if(need_to_set==1&&selected_Index==15){qDebug()<<"15.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->VarTypeNodeNum++;
        CreateVarType(new_event->scenePos(),this->VarTypeNodeNum);
    }
    if(need_to_set==1&&selected_Index==16){qDebug()<<"16.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->VarDefNodeNum++;
        CreateVarDef(new_event->scenePos(),this->VarDefNodeNum);
    }
    if(need_to_set==1&&selected_Index==17){qDebug()<<"17.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->ComputeNodeNum++;
        CreateCompute(new_event->scenePos(),this->ComputeNodeNum);
    }
    if(need_to_set==1&&selected_Index==18){qDebug()<<"18.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->IONodeNum++;
        CreateIO(new_event->scenePos(),this->IONodeNum);
    }
    if(need_to_set==1&&selected_Index==19){qDebug()<<"19.";
        emit itemInserted(selected_Index);
        need_to_set = 0;
        this->LogicNodeNum++;
        CreateLogic(new_event->scenePos(),this->LogicNodeNum);
    }
    QGraphicsScene::mousePressEvent(new_event);
    //setCursor(Qt::ArrowCursor);
}

bool newscene::CreateTakeOff(QPointF point, int id)
{
    TakeoffNode *node=new TakeoffNode;
    node->setText(tr("take off\n %1 s").arg(node->time));

    node->setPos(point);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF((node->pos().x()),(node->pos().y() + node->outlineRect().height()/2)+node->yuan->boundingRect().height()/2));
    this->addItem(node->yuan);

    node->controlsId=id;
    node->identifier="TakeOff";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中

    node->yuan->master = tmp;
    node->yuan->name = "yuan";
}

bool newscene::CreateLand(QPointF point, int id)
{
    LandonNode *node=new LandonNode;
    node->setText(tr("Land on\n %1 s").arg(node->time));

    node->setPos(point);
    this->addItem(node);


    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan2->setPos(QPointF((node->pos().x()),
                       (node->pos().y() - node->outlineRect().height()/2)-node->yuan2->boundingRect().height()/2));
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
}

bool newscene::CreateGo(QPointF point, int id)
{
    TranslationNode *node=new TranslationNode;
    node->setText(tr(" %1 m/s \n %2 s").arg(node->speed).arg(node->time));
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;

    node->setPos(point);
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

    item->setPos(QPointF(node->pos().x()-40,
                 (node->pos().y() - node->outlineRect().height()/2 - node->item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("GoUp"));
    node->box->addItem(tr("GoDown"));
    node->box->addItem(tr("Forward"));
    node->box->addItem(tr("Backward"));
    node->box->addItem(tr("GoRight"));
    node->box->addItem(tr("GoLeft"));
    node->box->setCurrentIndex(0);

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


}

bool newscene::CreateTurn(QPointF point, int id)
{
    TurnNode *node=new TurnNode;

    node->setText(tr(" %1  \n %2 s").arg(node->angel).arg(node->time));
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;

    node->setPos(point);
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

    item->setPos(QPointF(node->pos().x()-40,
                 (node->pos().y() - node->outlineRect().height()/2 - node->item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("TurnLeft"));
    node->box->addItem(tr("TurnRight"));
    //node->box->addItem(tr("Hover"));
    //node->box->addItem(tr("Delay"));
    node->box->setCurrentIndex(0);

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

}

bool newscene::CreateHover(QPointF point, int id)
{
    HoverNode *node=new HoverNode;

    node->setText(tr(" Hover \n %2 s").arg(node->angel).arg(node->time));

    node->setPos(point);
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


}

bool newscene::CreateDelay(QPointF point, int id)
{
    DelayNode *node=new DelayNode;

    node->setText(tr(" Delay \n %2 s").arg(node->angel).arg(node->time));

    node->setPos(point);
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

}

bool newscene::CreateVarType(QPointF point, int id)
{
    VarNode* node=new VarNode;
    node->setText(tr("int"));

    node->setPos(point);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();
    //setCursor(Qt::ArrowCursor);

    node->controlsId=id;
    node->identifier="VarType";
    QString cid = QString::number(node->controlsId,10);
    node->name = node->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;

    WidgetWrap* tmp = new WidgetWrap(node);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
}

bool newscene::CreateVarDef(QPointF point, int id)
{   //先设定不论从哪里生成控件都会需要的公共的属性
    VardefNode* vdn=new VardefNode;
    vdn->setPos(point);

        QList<QGraphicsItem *> items = this->selectedItems();
        if(items.count()==0)
        {
            this->addItem(vdn);
            vdn->node=0;
            vdn->yuan2->setPos(vdn->pos().x(),
                               vdn->pos().y() - 16 - vdn->yuan2->boundingRect().height()/2);
            vdn->yuan->setPos(vdn->pos().x(),
                               vdn->pos().y() + 16 + vdn->yuan->boundingRect().height()/2);
            this->addItem(vdn->yuan);
            this->addItem(vdn->yuan2);
        }else if(items.count()==1)
        {
            VarNode* node=dynamic_cast<VarNode*>(this->selectedItems().first());
            if(!node)return false;

            int flag=0;
            while(node->flags[node->num])//这个位置已经有了vardefnode
            {
                if(flag==6)return false;
                node->num=node->num%6+1;
                flag++;
            }
            //计算添加的位置
            int i=node->num%3;
            int j;
            if(node->num==0||node->num==2)j=-17;
            else if(node->num==3||node->num==5)j=17;
            else if(node->num==1)j=-35;
            else j=35;

            node->array[node->num]->node=node;//使vardefnode知道它属于varnode

            int x = node->pos().x() + (1-i)*30;
            int y = node->pos().y() + j;
            (node->array[node->num])->setPos(x,y);
            vdn = node->array[node->num];    //在这里记录VarDef，最后包装、添加到map
            vdn->seq = node->num;
            node->flags[node->num]=true;
            this->addItem(node->array[node->num]);
            node->num=node->num%6+1;
        }

    vdn->controlsId=id;
    vdn->identifier="VarDef";
    QString cid = QString::number(vdn->controlsId,10);
    vdn->name = vdn->identifier + cid;
    qDebug()<<"Create():";
    qDebug()<<"name :"<<vdn->name;
    qDebug()<<"identifier :"<<vdn->identifier;
    qDebug()<<"controlsId :"<<vdn->controlsId;
    WidgetWrap* tmp = new WidgetWrap(vdn);   //包装节点
    wm->add(tmp);            //添加到widgetmap中
    vdn->yuan2->master = tmp;
    vdn->yuan2->name = "yuan2";
    vdn->yuan->master = tmp;
    vdn->yuan->name = "yuan";

}

bool newscene::CreateCompute(QPointF point, int id)
{
    ComputeNode *node=new ComputeNode;
    node->setText(tr("Compute"));
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;
    node->setPos(point);
    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();
    //setCursor(Qt::ArrowCursor);


    node->yuan->setPos(QPointF(node->pos().x(),
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                                node->pos().y()));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->outlineRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    item->setPos(QPointF(node->pos().x()- item->boundingRect().width()/2,
                 node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height()));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("+"));
    node->box->addItem(tr("-"));
    node->box->addItem(tr("*"));
    node->box->addItem(tr("/"));
    node->box->addItem(tr("cos"));
    node->box->addItem(tr("sin"));
    node->box->addItem(tr("tan"));
    node->box->addItem(tr("log"));
    node->box->addItem(tr("e"));
    node->box->addItem(tr("="));
    node->box->addItem(tr(">"));
    node->box->addItem(tr("<"));
    node->box->setCurrentIndex(0);

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

}

bool newscene::CreateIO(QPointF point, int id)
{
    IoNode* node=new IoNode;
    node->setText(tr("sensor"));
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

}

bool newscene::CreateLogic(QPointF point, int id)
{
    Rec *rec=new Rec;
    QGraphicsItem* item= this->addWidget(rec->box);
    rec->item=item;

    rec->setPos(point);
    this->addItem(rec);
    this->clearSelection();
    rec->setSelected(true);

    rec->yuan2->setPos(QPointF(rec->pos().x() - rec->outlineRect().height()/2 + item->boundingRect().width()/2,
                               rec->pos().y() - rec->outlineRect().height()/2 +item->boundingRect().height()*1.5));
    rec->yuan->setPos(QPointF(rec->pos().x(), rec->pos().y() + rec->outlineRect().height()*0.5));
    this->addItem(rec->yuan2);
    this->addItem(rec->yuan);

    item->setPos(QPointF(rec->pos().x()-rec->outlineRect().width()/2,
                         (rec->pos().y() - rec->outlineRect().height()/2)));
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

}

bool newscene::CreateLink(QGraphicsSceneMouseEvent* event)
{
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
                      Link* new_link=new Link(dynamic_cast<triYuan *>(this->selectedItems().first()),
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
}

//----------------从xml文件创建控件-----------------------
bool newscene::CreateTakeOff(TakeoffNode* node)
{
    node->setText(tr("take off\n %1 s").arg(node->time));

    node->setPos(node->lx,node->ly);
    this->addItem(node);

    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF((node->pos().x()),(node->pos().y() + node->outlineRect().height()/2)+node->yuan->boundingRect().height()/2));
    this->addItem(node->yuan);
    this->takeoffNodeNum++;
     //node->set_master(tmp);

}
bool newscene::CreateLand(LandonNode* node)
{
    node->setText(tr("Land on\n %1 s").arg(node->time));

    node->setPos(node->lx,node->ly);
    this->addItem(node);


    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan2->setPos(QPointF((node->pos().x()),
                       (node->pos().y() - node->outlineRect().height()/2)-node->yuan2->boundingRect().height()/2));
    this->addItem(node->yuan2);
     this->landonNodeNum++;
}
bool newscene::CreateGo(TranslationNode* node)
{
    node->setText(tr(" %1 m/s \n %2 s").arg(node->speed).arg(node->time));
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;

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

    item->setPos(QPointF(node->pos().x()-40,
                 (node->pos().y() - node->outlineRect().height()/2 - node->item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("GoUp"));
    node->box->addItem(tr("GoDown"));
    node->box->addItem(tr("Forward"));
    node->box->addItem(tr("Backward"));
    node->box->addItem(tr("GoRight"));
    node->box->addItem(tr("GoLeft"));
    node->box->setCurrentIndex(0);
    this->GoNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
}
bool newscene::CreateTurn(TurnNode* node)
{
    node->setText(tr(" %1  \n %2 s").arg(node->angel).arg(node->time));
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;

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

    item->setPos(QPointF(node->pos().x()-40,
                 (node->pos().y() - node->outlineRect().height()/2 - node->item->boundingRect().height())));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("TurnLeft"));
    node->box->addItem(tr("TurnRight"));
    node->box->setCurrentIndex(0);
    this->TurnNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
}
bool newscene::CreateHover(HoverNode* node)
{
    node->setText(tr(" Hover \n %2 s").arg(node->angel).arg(node->time));

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
}
bool newscene::CreateDelay(DelayNode *node)
{
    node->setText(tr(" Delay \n %2 s").arg(node->angel).arg(node->time));

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
}
bool newscene::CreateVarType(VarNode* node)
{
    node->setText(tr("int"));

    node->setPos(node->lx,node->ly);

    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();
    this->VarTypeNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
}
bool newscene::CreateVarDef(VardefNode* vdn)
{   //先设定不论从哪里生成控件都会需要的公共的属性
    vdn->setPos(vdn->lx,vdn->ly);
    this->addItem(vdn);
    if(vdn->node==0 && vdn->seq==-1){   //没有vartype
        vdn->node=0;
        vdn->seq=-1;
        vdn->yuan2->setPos(vdn->pos().x(),
                           vdn->pos().y() - 16 - vdn->yuan2->boundingRect().height()/2);
        vdn->yuan->setPos(vdn->pos().x(),
                           vdn->pos().y() + 16 + vdn->yuan->boundingRect().height()/2);
        this->addItem(vdn->yuan);
        this->addItem(vdn->yuan2);
    }
    this->VarDefNodeNum++;

    qDebug()<<"Create(xml):";
    qDebug()<<"name :"<<vdn->name;
    qDebug()<<"identifier :"<<vdn->identifier;
    qDebug()<<"controlsId :"<<vdn->controlsId;
}
bool newscene::CreateCompute(ComputeNode *node)
{
    node->setText(tr("Compute"));
    QGraphicsItem* item=this->addWidget(node->box);
    node->item=item;
    node->setPos(node->lx,node->ly);
    this->addItem(node);
    this->clearSelection();
    node->setSelected(true);
    bringToFront();

    node->yuan->setPos(QPointF(node->pos().x(),
                               node->pos().y() + node->outlineRect().height()/2 +node->yuan->boundingRect().height()/2));
    node->yuan2->setPos(QPointF(node->pos().x() - node->outlineRect().width()/2 - node->yuan2->outlineRect().width()/2,
                                node->pos().y()));
    node->yuan3->setPos(QPointF(node->pos().x() + node->outlineRect().width()/2 + node->yuan3->outlineRect().width()/2,
                                node->pos().y()));
    this->addItem(node->yuan);
    this->addItem(node->yuan2);
    this->addItem(node->yuan3);

    item->setPos(QPointF(node->pos().x()- item->boundingRect().width()/2,
                 node->pos().y() - node->outlineRect().height()/2 - item->boundingRect().height()));
    item->setZValue(node->zValue()+1);
    node->box->addItem(tr("+"));
    node->box->addItem(tr("-"));
    node->box->addItem(tr("*"));
    node->box->addItem(tr("/"));
    node->box->addItem(tr("cos"));
    node->box->addItem(tr("sin"));
    node->box->addItem(tr("tan"));
    node->box->addItem(tr("log"));
    node->box->addItem(tr("e"));
    node->box->addItem(tr("="));
    node->box->addItem(tr(">"));
    node->box->addItem(tr("<"));
    this->ComputeNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<node->name;
    qDebug()<<"identifier :"<<node->identifier;
    qDebug()<<"controlsId :"<<node->controlsId;
}
bool newscene::CreateIO(IoNode* node)
{
    node->setText(tr("sensor"));
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
}
bool newscene::CreateLogic(Rec *rec)
{
    QGraphicsItem* item= this->addWidget(rec->box);
    rec->item=item;

    rec->setPos(rec->lx,rec->ly);
    this->addItem(rec);
    this->clearSelection();
    rec->setSelected(true);

    this->clearSelection();
    rec->setSelected(true);

    rec->yuan2->setPos(QPointF(rec->pos().x() - rec->outlineRect().height()/2 + item->boundingRect().width()/2,
                               rec->pos().y() - rec->outlineRect().height()/2 +item->boundingRect().height()*1.5));
    this->addItem(rec->yuan2);

    item->setPos(QPointF(rec->pos().x()-rec->outlineRect().width()/2,
                         (rec->pos().y() - rec->outlineRect().height()/2)));
    item->setZValue(rec->zValue()+1);
    rec->box->addItem(tr("if"));
    rec->box->addItem(tr("else"));
    rec->box->addItem(tr("while"));
    this->LogicNodeNum++;

    qDebug()<<"Create():";
    qDebug()<<"name :"<<rec->name;
    qDebug()<<"identifier :"<<rec->identifier;
    qDebug()<<"controlsId :"<<rec->controlsId;
}
bool newscene::CreateWidgets()
{
    typename QMap<QString, widget>::iterator iter;
    QMap<QString,WidgetWrap> Map;
    Map = wm->get_map();

    for(iter=Map.begin();iter!=Map.end();iter++){
        if(iter->identifier=="TakeOff"){
            //QPointF point(iter->mTakeOffNode->lx,iter->mTakeOffNode->ly);
            CreateTakeOff(iter->mTakeOffNode);
        }
        if(iter->identifier=="Land"){
            //QPointF point(iter->mLandNode->lx,iter->mLandNode->ly);
            CreateLand(iter->mLandNode);
        }
        if(iter->identifier=="Go"){
            //QPointF point(iter->mGoNode->lx,iter->mGoNode->ly);
            CreateGo(iter->mGoNode);
        }
        if(iter->identifier=="Turn"){
            //QPointF point(iter->mTurnNode->lx,iter->mTurnNode->ly);
            CreateTurn(iter->mTurnNode);
        }
        if(iter->identifier=="Hover"){
            //QPointF point(iter->mHoverNode->lx,iter->mHoverNode->ly);
            CreateHover(iter->mHoverNode);
        }
        if(iter->identifier=="Delay"){
            //QPointF point(iter->mDelayNode->lx,iter->mDelayNode->ly);
            CreateDelay(iter->mDelayNode);
        }
        if(iter->identifier=="VarType"){
            //QPointF point(iter->mVarTypeNode->lx,iter->mVarTypeNode->ly);
            CreateVarType(iter->mVarTypeNode);
        }
        if(iter->identifier=="VarDef"){
            //QPointF point(iter->mVarDefNode->lx,iter->mVarDefNode->ly);
            CreateVarDef(iter->mVarDefNode);
        }
        if(iter->identifier=="Compute"){
            //QPointF point(iter->mComputeNode->lx,iter->mComputeNode->ly);
            CreateCompute(iter->mComputeNode);
        }
        if(iter->identifier=="IO"){
            //QPointF point(iter->mIONode->lx,iter->mIONode->ly);
            CreateIO(iter->mIONode);
        }
        if(iter->identifier=="Logic"){
            //QPointF point(iter->mLogicNode->lx,iter->mLogicNode->ly);
            CreateLogic(iter->mLogicNode);
        }
        if(iter->identifier=="Link"){
            CreateLink(iter->mLinkNode);
        }
    }
}
bool newscene::CreateLink(Link* link)
{
    link->setZValue(100);
    this->addItem(link);
    this->linkNodeNum++;
    return true;
}
