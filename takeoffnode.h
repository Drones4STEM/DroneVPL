#ifndef TAKEOFFNode_H
#define TAKEOFFNode_H

#include "node.h"
#include "newnode.h"
#include <QtGui>
#include "QObject"
#include "itemtypes.h"

/*******************************************************************
 * Class name: TakeoffNode
 * Base class: Node
 * Description: This is the declaration of class TakeoffNode.
 *       TakeoffNode is an action node stands for takeoff.
******************************************************************/
class TakeoffNode:public Node
{
public:
    enum {Type = TakeoffNodeType};
    TakeoffNode();

    int type() const { return Type; }

    double time;

    void setTime(double t);
    double myTime();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

QDataStream &operator <<(QDataStream &out,const TakeoffNode &node);
QDataStream &operator >>(QDataStream &in,TakeoffNode &node);

/*******************************************************************
 * Class name: LandonNode
 * Base class: NewNode
 * Description: This is the declaration of class LandNode.
 *       LandNode is an action node stands for takeoff.
******************************************************************/
class LandonNode:public NewNode
{
public:
    enum {Type = LandonNodeType};
    LandonNode();

    int type() const { return Type; }

    double time;

    void setTime(double t);
    double myTime();


protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
};

/*******************************************************************
 * Class name: TranslationNode
 * Base class: NewNode
 * Description: This is the declaration of class TranslationNode.
 *     TranslateionNode represents some motions of translation.
******************************************************************/
class TranslationNode:public NewNode
{
    Q_OBJECT
public:
    enum {Type = TranslationNodeType};
    TranslationNode();
    ~TranslationNode();

    int type() const { return Type; }

    double speed,time;
    QComboBox *box;

    QGraphicsItem *item;

    void setTime(double t);
    double myTime();
    void setSpeed(double s);
    double mySpeed();

    static int riseNodeNum;     static int fallNodeNum;
    static int advanceNodeNum;  static int backNodeNum;
    static int rightNodeNum;    static int leftNodeNum;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);

private slots:
    void setNewIdentifier();
};

/*******************************************************************
 * Class name: SomeNode
 * Base class: NewNode
 * Description: This is a class for developers, and determines some
 *     to follow. This class represents some actions including
 *     turnleft, turnright, hang and delay(hover).
******************************************************************/
class SomeNode:public NewNode
{
    Q_OBJECT
public:
    enum {Type = SomeNodeType};
    SomeNode();
    ~SomeNode();

    int type() const { return Type; }

    double time,speed,angel;
    QComboBox*box;
    QGraphicsItem *item;

    void setTime(double t);
    double myTime();
    void setSpeed(double s);
    double mySpeed();
    void setAngel(double a);
    double myAngel();

    static int turnLeftNodeNum;   static int turnRightNodeNum;
    static int hangingNodeNum;    static int delayNodeNum;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
private slots:
    void setNewText();
    void setNewIdentifier();
};

#endif
