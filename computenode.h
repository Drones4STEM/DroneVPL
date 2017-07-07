#ifndef COMPUTENODE_H
#define COMPUTENODE_H

class Yuan;
class QComboBox;
class QGraphicsItem;
#include "QObject"
#include "node.h"
#include "itemtypes.h"

/*******************************************************************
 * Class name: ComputeNode
 * Base class: Node
 * Description: This is the declaration of class ComputeNode.
 *       ComputeNode is a node which connect two varnodes (variables)
 *       and compute an outcome.
******************************************************************/
class ComputeNode:public Node
{
    Q_OBJECT;
public:
    enum {Type = ComputeNodeType};
    ComputeNode();
    ~ComputeNode();

    int type() const { return Type; }

    QRectF outlineRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

    Yuan* yuan2;
    Yuan* yuan3;
    QComboBox* box;
    QGraphicsItem* item;

    static int ComputeNodeAddNum; static int ComputeNodeSubNum;
    static int ComputeNodeMulNum; static int ComputeNodeDivNum;
    static int ComputeNodeCosNum; static int ComputeNodeSinNum;
    static int ComputeNodeTanNum; static int ComputeNodeLogNum;
    static int ComputeNodeENum; static int ComputeNodeEqualNum;
    static int ComputeNodeMoreNum; static int ComputeNodeLessNum;

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
private slots:
    void setNewIdentifier();
};

#endif // COMPUTENODE_H
