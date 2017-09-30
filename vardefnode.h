#ifndef VARDEFNODE_H
#define VARDEFNODE_H

#include "node.h"
#include "newnode.h"
#include <QtGui>
#include "itemtypes.h"

class VarNode;

/*******************************************************************
 * Class name: VardefNode
 * Base class: NewNode
 * Description: This is the declaration of class VardefNode. VardefNode
 *       is a definition of variables.
******************************************************************/
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
    void sethw(){wide = 40; high = 32;}
    //为了方便，直接在头文件里定死长宽，若修改outlineRect()中的六边形长宽，
    //则这里的数据也要改

    //用来存储变量的值
    QString varName;// 变量名

    int myInt;
    double myDouble;
    QString myString;
    QString myStringText;


    //存储varnode
    VarNode* node;
    int seq;//记录自己是node的第几个vardef节点

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
};

#endif // VARDEFNODE_H
