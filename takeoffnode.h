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
class TakeOffNode:public Node
{
    Q_OBJECT
    Q_PROPERTY(double altitude READ myAltitude WRITE setAltitude)
public:
    enum {Type = TakeoffNodeType};
    TakeOffNode();

    int type() const { return Type; }


    double time;
    double altitude;

    void setTime(double t);
    double myTime();
    void setAltitude(double a);
    double myAltitude();


protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

QDataStream &operator <<(QDataStream &out,const TakeOffNode &node);
QDataStream &operator >>(QDataStream &in,TakeOffNode &node);

/*******************************************************************
 * Class name: LandonNode
 * Base class: NewNode
 * Description: This is the declaration of class LandNode.
 *       LandNode is an action node stands for takeoff.
******************************************************************/
class LandNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double time READ myTime WRITE setTime)
public:
    enum {Type = LandonNodeType};
    LandNode();

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
class GoNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double groundspeed READ myGroundSpeed WRITE setGroundSpeed)
public:
    enum {Type = TranslationNodeType};
    GoNode();
    ~GoNode();

    int type() const { return Type; }

    double time;
    double groundspeed;
    QString direction;
    QComboBox *box;

    QGraphicsItem *item;

    void setTime(double t);
    double myTime();
    void setGroundSpeed(double s);
    double myGroundSpeed();


    static int riseNodeNum;     static int fallNodeNum;
    static int advanceNodeNum;  static int backNodeNum;
    static int rightNodeNum;    static int leftNodeNum;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);

private slots:

public slots:
    void setDirection();
};

/*******************************************************************
 * Class name: TurnNode
 * Base class: NewNode
 * Description: This is a class for developers, and determines some
 *     to follow. This class represents some actions including
 *     turnleft, turnright.
******************************************************************/
class TurnNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double speed READ mySpeed WRITE setSpeed)
public:
    enum {Type = SomeNodeType};
    TurnNode();
    ~TurnNode();

    int type() const { return Type; }

    double time,speed,angel;
    QString direction;
    QComboBox*box;
    QGraphicsItem *item;

    void setTime(double t);
    double myTime();
    void setSpeed(double s);
    double mySpeed();
    void setAngel(double a);
    double myAngel();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
private slots:
    void setNewText();

public slots:
    void setDirection();
};

/*******************************************************************
 * Class name: HoverNode
 * Base class: NewNode
 * Description: This is a class for developers, and determines some
 *     to follow. This class represents some actions including
 *     turnleft, turnright.
******************************************************************/
class HoverNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double time READ myTime WRITE setTime)
public:
    enum {Type = SomeNodeType};
    HoverNode();
    ~HoverNode();

    int type() const { return Type; }

    double time,speed,angel;
    //QGraphicsItem *item;

    void setTime(double t);
    double myTime();
    void setSpeed(double s);
    double mySpeed();
    void setAngel(double a);
    double myAngel();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
private slots:
    void setNewText();
};

/*******************************************************************
 * Class name: DelayNode
 * Base class: NewNode
 * Description: This is a class for developers, and determines some
 *     to follow. This class represents some actions including
 *     turnleft, turnright.
******************************************************************/
class DelayNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double time READ myTime WRITE setTime)
public:
    enum {Type = SomeNodeType};
    DelayNode();
    ~DelayNode();

    int type() const { return Type; }

    double time,speed,angel;
    //QGraphicsItem *item;

    void setTime(double t);
    double myTime();
    void setSpeed(double s);
    double mySpeed();
    void setAngel(double a);
    double myAngel();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
private slots:
    void setNewText();
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
