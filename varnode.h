#ifndef VARNODE_H
#define VARNODE_H

#include "node.h"
#include "newnode.h"
#include "vardefnode.h"
#include <QtGui>

/*******************************************************************
 * Class name: VarNode
 * Base class: Node
 * Description: This is the declaration of class VarNode. VarNode
 *       represents variables read and written by other actions.
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
    void sethw(){wide = 40; high = 32;}
    //为了方便，直接在头文件里定死长宽，若修改outlineRect()中的六边形长宽，
    //则这里的数据也要改

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
