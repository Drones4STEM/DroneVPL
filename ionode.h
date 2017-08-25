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
    IoSmallNode();

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
};
#endif // IONODE_H
