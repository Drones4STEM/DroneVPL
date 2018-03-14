#ifndef VarInstanceNODE_H
#define VarInstanceNODE_H

#include "node.h"
#include "newnode.h"
#include <QtGui>
#include "itemtypes.h"

class VarNode;

/*******************************************************************
 * Class name: VarInstanceNode
 * Base class: NewNode
 * Description: This is the declaration of class VarInstanceNode. VarInstanceNode
 *       is a definition of variables.
******************************************************************/
class VarInstanceNode:public NewNode
{
    Q_OBJECT

public:
    VarInstanceNode(QGraphicsItem *parent = 0);

    QRectF outlineRect()const;
    QRectF boundingRect()const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString varType;    //变量的类型
    double varValue;       //变量的值
    QString varName;       //变量名

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
};

#endif // VarInstanceNODE_H
