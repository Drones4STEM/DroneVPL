#ifndef IONODE_H
#define IONODE_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include <QtGui>

#include "node.h"
#include "newnode.h"
#include "itemtypes.h"

class IoSmallNode;

/*******************************************************************
 * Class name: IoNode
 * Base class: NewNode
 * Description: This is the declaration of class IoNode. IoNode is a
 *       node which represents I/O operations.
******************************************************************/
class IoNode:public NewNode
{
    Q_OBJECT

public:
    enum {Type = IoNodeType};
    IoNode();
    ~IoNode();

    int type() const { return Type; }

    QComboBox *box;
    IoSmallNode *node1, *node2, *node3;
    QGraphicsItem* item;

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
};

/*******************************************************************
 * Class name: IoSmallNode
 * Base class: Node
 * Desciption:This is a class for developers, and determines some
 *     to follow.
******************************************************************/
class IoSmallNode:public Node
{
    Q_OBJECT
public:
    enum {Type = IoSmallNodeType};
    IoSmallNode(int sizeFlag=1,QGraphicsItem *parent = 0);//根据这个flag来设置node的outlineRect大小
                                                      //当flag=1时，80*26，flag=2时，80*46

    int type() const { return Type; }

    void setIoType(QString &type);
    QString ioType();
    void setVar(QString &var);
    QString var();
    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);


private:
    QString myIoType;
    QString myVar;
    int sizeFlag;
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

/*******************************************************************
 * Class name: BatteryNode
 * Base class: NewNode
 * Description: This is the declaration of class IoNode. IoNode is a
 *       node which represents I/O operations.
******************************************************************/
class BatteryNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double voltage READ myVoltage WRITE setVoltage)
    Q_PROPERTY(double current READ myCurrent WRITE setCurrent)
    Q_PROPERTY(double level READ myLevel WRITE setLevel)
public:
   // enum {Type = BatteryNodeType};
    BatteryNode();
    ~BatteryNode();

    //int type() const { return Type;}

    QComboBox *box;
    IoSmallNode *node1, *node2, *node3;
    QGraphicsItem* item;
    double voltage,current,level;

    double myVoltage(){return voltage;}
    void setVoltage(double v){voltage = v;}
    double myCurrent(){return current;}
    void setCurrent(double c){current = c;}
    double myLevel(){return level;}
    void setLevel(double l){level = l;}

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

/*******************************************************************
 * Class name: GimbalNode
 * Base class: NewNode
 * Description: This is the declaration of class GimbalNode. GimbalNode
 *       is a node which represents I/O operations.
******************************************************************/
class GimbalNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double pitch READ myPitch WRITE setPitch)
    Q_PROPERTY(double roll READ myRoll WRITE setRoll)
    Q_PROPERTY(double yaw READ myYaw WRITE setYaw)
public:
    GimbalNode();
    ~GimbalNode();

    QComboBox *box;
    IoSmallNode *node1, *node2, *node3;
    QGraphicsItem* item;
    double pitch,roll,yaw;

    double myPitch(){return pitch;}
    void setPitch(double v){pitch = v;}
    double myYaw(){return yaw;}
    void setYaw(double c){yaw = c;}
    double myRoll(){return roll;}
    void setRoll(double l){roll = l;}

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

/*******************************************************************
 * Class name: RangeFinderNode
 * Base class: NewNode
 * Description: This is the declaration of class IoNode. IoNode is a
 *       node which represents I/O operations.
******************************************************************/
class AttitudeNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double roll READ myRoll WRITE setRoll)
    Q_PROPERTY(double pitch READ myPitch WRITE setPitch)
    Q_PROPERTY(double yaw READ myYaw WRITE setYaw)
public:
   // enum {Type = AttitudeNodeType};
    AttitudeNode();
    ~AttitudeNode();

    //int type() const { return Type;}

    QComboBox *box;
    IoSmallNode *node1, *node2, *node3;
    QGraphicsItem* item;
    double roll,pitch,yaw;

    double myRoll(){return roll;}
    void setRoll(double r){roll = r;}
    double myPitch(){return pitch;}
    void setPitch(double p){pitch = p;}
    double myYaw(){return yaw;}
    void setYaw(double y){yaw = y;}

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

/*******************************************************************
 * Class name: ChannelNode
 * Base class: NewNode
 * Description: This is the declaration of class IoNode. IoNode is a
 *       node which represents I/O operations.
******************************************************************/
class ChannelNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double one READ myOne WRITE setOne)
    Q_PROPERTY(double two READ myTwo WRITE setTwo)
    Q_PROPERTY(double three READ myThree WRITE setThree)
    Q_PROPERTY(double four READ myFour WRITE setFour)
    Q_PROPERTY(double five READ myFive WRITE setFive)
    Q_PROPERTY(double six READ mySix WRITE setSix)
    Q_PROPERTY(double seven READ mySeven WRITE setSeven)
    Q_PROPERTY(double eight READ myEight WRITE setEight)
    Q_PROPERTY(double nine READ myNine WRITE setNine)
public:
   // enum {Type = ChannelNodeType};
    ChannelNode();
    ~ChannelNode();

    //int type() const { return Type;}

    QComboBox *box;
    IoSmallNode *node1, *node2, *node3, *node4, *node5, *node6, *node7, *node8, *node9;
    QGraphicsItem* item;
    double one,two,three,four,five,six,seven,eight,nine;

    double myOne(){return one;}
    void setOne(double r){one = r;}
    double myTwo(){return two;}
    void setTwo(double p){two = p;}
    double myThree(){return three;}
    void setThree(double y){three = y;}
    double myFour(){return four;}
    void setFour(double r){four = r;}
    double myFive(){return five;}
    void setFive(double p){five = p;}
    double mySix(){return six;}
    void setSix(double y){six = y;}
    double mySeven(){return seven;}
    void setSeven(double r){seven = r;}
    double myEight(){return eight;}
    void setEight(double p){eight = p;}
    double myNine(){return nine;}
    void setNine(double y){nine = y;}

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

/*******************************************************************
 * Class name: RangeFinderNode
 * Base class: NewNode
 * Description: This is the declaration of class IoNode. IoNode is a
 *       node which represents I/O operations.
******************************************************************/
class RangeFinderNode:public NewNode
{
    Q_OBJECT
    Q_PROPERTY(double distance READ myRangeFinder WRITE setRangeFinder)

public:
   // enum {Type = RangeFinderNodeType};
    RangeFinderNode();
    ~RangeFinderNode();

    //int type() const { return Type;}

    QComboBox *box;
    IoSmallNode *node2;
    QGraphicsItem* item;
    double distance;

    double myRangeFinder(){return distance;}
    void setRangeFinder(double r){distance = r;}

    QRectF outlineRect() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // IONODE_H
