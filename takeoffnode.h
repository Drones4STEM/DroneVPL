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
    Q_PROPERTY(double myAltitude READ myAltitude WRITE setAltitude)
public:
    enum {Type = TakeoffNodeType};
    TakeOffNode();

    int type() const { return Type; }

    double altitude;
    double myAltitude();

signals:
    void altitudeChanged(double);
public slots:
    void setAltitude(double a);


protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

/*******************************************************************
 * Class name: LandonNode
 * Base class: NewNode
 * Description: This is the declaration of class LandNode.
 *       LandNode is an action node stands for takeoff.
******************************************************************/
class LandNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double myTime READ myTime WRITE setTime)
public:
    enum {Type = LandonNodeType};
    LandNode();

    int type() const { return Type; }

    double time;
    double myTime();

signals:
    void timeChanged(double t);
public slots:
    void setTime(double t);


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
    Q_PROPERTY(double myGroundSpeed READ myGroundSpeed WRITE setGroundSpeed)
    Q_PROPERTY(QString myDirection READ myDirection)
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
    double myGroundSpeed();
    QString myDirection() const {return direction;}


    static int riseNodeNum;     static int fallNodeNum;
    static int advanceNodeNum;  static int backNodeNum;
    static int rightNodeNum;    static int leftNodeNum;

signals:
    void groundSpeedChanged(double s);
public slots:
    void setGroundSpeed(double s);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);

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
    Q_PROPERTY(double mySpeed READ mySpeed WRITE setSpeed)
    Q_PROPERTY(QString myDirection READ myDirection)
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
    double mySpeed();
    void setAngel(double a);
    double myAngel();
    QString myDirection() const{return direction;}

signals:
    void speedChanged(double s);
public slots:
    void setSpeed(double s);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
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
    Q_PROPERTY(double myTime READ myTime WRITE setTime)
public:
    enum {Type = SomeNodeType};
    HoverNode();
    ~HoverNode();

    int type() const { return Type; }

    double time;
    //QGraphicsItem *item;

    double myTime();

signals:
    void timeChanged(double t);
public slots:
    void setTime(double t);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
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
    Q_PROPERTY(double myTime READ myTime WRITE setTime)
public:
    enum {Type = SomeNodeType};
    DelayNode();
    ~DelayNode();

    int type() const { return Type; }

    double time;
    //QGraphicsItem *item;

    double myTime();

signals:
    void timeChanged(double t);
public slots:
    void setTime(double t);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
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
