#include <QtGui>

#include "view.h"
#include "yuan.h"
#include "link.h"

newview::newview()
{
    new_yuan=new Yuan;
}

newview::~newview()
{
   delete new_yuan;
}

void newview::mouseMoveEvent(QMouseEvent *event_one)
{
   if((this->scene()->selectedItems().count()==1)&&dynamic_cast<triYuan *>(this->scene()->selectedItems().first())!=0)
   {
         new_yuan->setPos(event_one->pos());

         if(new_yuan->myLinks.size()>0)
         {
             foreach (Link* link,new_yuan->myLinks)
                 delete link;
         }
        Link* new_link=new Link(dynamic_cast<triYuan *>(this->scene()->selectedItems(). first()),new_yuan);
        new_link->setZValue(100);
        this->scene()->addItem(new_link);
   update();
   }
    QGraphicsView::mouseMoveEvent(event_one);
}

void newview::mouseReleaseEvent(QMouseEvent* event_two)
{
  if(this->scene()->selectedItems().count()==1&&dynamic_cast<triYuan *>(this->scene()->selectedItems().first())!=0)
   {
//      QList<triYuan *> near_items=this->scene()->items (event_two->pos(),10, 10, Qt::IntersectsItemShape,Qt::AscendingOrder, );
      if(near_items.count()==0)
      {
           delete dynamic_cast<Link *>(this->scene()->selectedItems().last());
      }
      else if(dynamic_cast<triYuan *>(near_items.first())!=0)
      {
           delete dynamic_cast<Link *>(this->scene()->selectedItems().last());
           Link* new_link=new Link(dynamic_cast<triYuan *>(this->scene()->selectedItems().first()),
                                    dynamic_cast<triYuan *>(near_items.first()));
                 new_link->setZValue(100);
                 this->scene()->addItem(new_link);
      }
   }
/*
if(dynamic_cast<QGraphicsItem  *>(near_yuan.first())!=0)
{
  delete new_yuan;
  scene->clearSelection();
  dynamic_cast<QGraphicsItem *>(near_yuan.first())->setSelected(true);
  nnnyuan->setSelected(true);
  addLink();*/
    QGraphicsView::mouseMoveEvent(event_two);
}







