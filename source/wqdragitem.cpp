#include "wqDragItem.h"

wqDragItem::wqDragItem( QGraphicsItem* parent /*= 0*/ )
 : QGraphicsObject(parent)
{
    setFlags(QGraphicsItem::ItemIsSelectable |
        QGraphicsItem::ItemSendsGeometryChanges |
        QGraphicsItem::ItemIsFocusable |
        QGraphicsItem::ItemIsMovable);

    setAcceptHoverEvents(true);
}

QVariant wqDragItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //if(change == ItemPositionChange && scene())
    //{
    //	emit sig_childMoved();
    //}

    if(change == ItemPositionHasChanged && scene())
    {
        emit sig_childMoved();

    }

    return QGraphicsItem::itemChange(change, value);
}

QRectF wqDragItem::boundingRect() const
{
    return QRectF(0,0,6,6);
}

void wqDragItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* wid )
{
    painter->save();

    painter->setBrush(Qt::white);
    if(isSelected())
    {
        painter->setBrush(Qt::red);
    }

    painter->drawRect(boundingRect());

    painter->restore();
}
