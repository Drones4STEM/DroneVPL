#ifndef VARDEFNODE_H
#define VARDEFNODE_H

#include "node.h"
#include "newnode.h"
#include <QtGui>
#include "itemtypes.h"

class VarNode;

class VardefNode:public NewNode
{
public:
    enum {Type = VardefNodeType};
    VardefNode();

    int type() const { return Type; }

    QPolygonF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    //用来存储变量的值
    QString varName;// 变量名

    int myInt;
    double myDouble;
    QString myString;

    //存储varnode
    VarNode* node;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
};

#endif // VARDEFNODE_H
