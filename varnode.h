#ifndef VARNODE_H
#define VARNODE_H

#include "node.h"
#include "newnode.h"
#include "vardefnode.h"
#include <QtGui>

/*******************************************************************
 * Class name: VarNode
 * Base class: Node
 * Desciption:This is a class for developers, and determines some
 *     to follow. It defines variable.
******************************************************************/
class VarNode:public Node
{
public:
    enum {Type = VardefNodeType};
    VarNode();
    ~VarNode();

    int type() const { return Type; }

    QPolygonF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    QComboBox* box;
    QGraphicsItem* item;

    int num;
    bool flags[6];
    VardefNode* array[6];


protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
};

#endif // VARNODE_H
