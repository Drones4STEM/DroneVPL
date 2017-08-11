#pragma once

#include <QtGui>
#include <QtCore>

class wqDragItem : public QGraphicsObject
{
    Q_OBJECT

public:
    wqDragItem(QGraphicsItem* parent = 0);
    virtual QRectF boundingRect() const;

signals:
    void sig_childMoved();

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* item, QWidget* wid);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
