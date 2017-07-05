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
 * Desciption:This is a class for developers, and determines some
 *     to follow.
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
private slots:
    //void changeType();
};

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
private:
    QString myIoType;
    QString myVar;
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // IONODE_H
