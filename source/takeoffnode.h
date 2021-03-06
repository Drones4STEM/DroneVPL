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
    ~TakeOffNode();

    int type() const { return Type; }

    double altitude;
    double myAltitude();
    QLineEdit *lineEdit;
    QGraphicsItem *lineItem;

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget);

signals:
    void altitudeChanged(double);
public slots:
    void setAltitude(double a);
    void setAltitude(QString str);//变量是str类型是为了响应自己的lineEdit发出的信号

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
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
//    Q_PROPERTY(double myTime READ myTime WRITE setTime)
public:
    enum {Type = LandonNodeType};
    LandNode();

    int type() const { return Type; }

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget);

signals:

public slots:



protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
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
    Q_PROPERTY(double myTime READ myTime WRITE setTime)
    Q_PROPERTY(QString myDirection READ myDirection)
public:
    enum {Type = TranslationNodeType};
    GoNode();
    ~GoNode();

    int type() const { return Type; }

    double Time;
    double groundspeed;
    QString direction;
    QComboBox *box;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit2;

    QGraphicsItem *item;
    QGraphicsItem *lineItem;
    QGraphicsItem *lineItem2;

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget);

    double myTime();
    double myGroundSpeed();
    QString myDirection() const {return direction;}

signals:
    void groundSpeedChanged(double s);
    void timeChanged(double s);
public slots:
    void setGroundSpeed(double s);
    void setTime(double t);
    void setGroundSpeed(QString str);
    void setTime(QString str);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

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
    Q_PROPERTY(double myAngel READ myAngel WRITE setAngel)
    Q_PROPERTY(QString myDirection READ myDirection)
public:
    enum {Type = SomeNodeType};
    TurnNode();
    ~TurnNode();

    int type() const { return Type; }

    double Angel;
    QString direction;
    QComboBox*box;
    QLineEdit *lineEdit;
    QGraphicsItem *item;
    QGraphicsItem *lineItem;

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget);

    void setAngel(double a);
    double myAngel();
    QString myDirection() const{return direction;}

signals:

public slots:
    void setAngel(QString str);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
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
    QLineEdit *lineEdit;
    QGraphicsItem *lineItem;

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget);

    double myTime();

signals:
    void timeChanged(double t);
public slots:
    void setTime(double t);
    void setTime(QString str);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
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
    double myTime();
    QLineEdit *lineEdit;
    QGraphicsItem *lineItem;

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,QWidget *widget);


signals:
    void timeChanged(double t);
public slots:
    void setTime(double t);
    void setTime(QString str);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
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
