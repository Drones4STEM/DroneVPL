#include <QtGui>

#include "node.h"
#include "yuan.h"
#include "link.h"
#include "newnode.h"
#include "rec.h"
#include "diagramwindow.h"

Rec::Rec()
{
    box=new QComboBox;

    setAcceptDrops(true);  //目前没什么用

    identifier="Rec";
    controlsId=0;

    connect(box,SIGNAL(currentIndexChanged(int)),this,SLOT(showYuan()));
}

Rec::~Rec()
{
    delete box;
    delete item;
}

QRectF Rec::outlineRect() const
{
    QRectF rect(0,0,400,400);
    rect.translate(-rect.center());
    return rect;
}

QRectF Rec::boundingRect() const
{
    const int Margin = 6;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath Rec::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void Rec::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */)
{
    QPen pen(outlineColor());
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(backgroundColor());

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));
}

QVariant Rec::itemChange(GraphicsItemChange change,
                    const QVariant &value)
{
    if (change & ItemPositionHasChanged) {
        yuan2->setPos(pos().x() - outlineRect().width()/2 + item->boundingRect().width()/2,
                     pos().y() - outlineRect().height()/2 + item->boundingRect().height()*1.5);
        foreach (Link *link, yuan->myLinks)
        {link->trackYuans();update();}

        item->setPos(QPointF(pos().x()-outlineRect().width()/2,
                     (pos().y() - outlineRect().height()/2)));
    }
    return QGraphicsItem::itemChange(change, value);
}

void Rec::showYuan()
{
    int i=box->currentIndex();
    switch (i) {
    case 0:
    case 2:
    {
        yuan2->setVisible(true);
        break;
    }
    case 1:
    {
        yuan2->setVisible(false);
        break;
    }
    default:
        break;
    }
}


