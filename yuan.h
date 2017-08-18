#ifndef YUAN_H
#define YUAN_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include "itemtypes.h"
#include "widgetwrap.h"

class Link;
class Node;
class WidgetWrap;

/*******************************************************************
 * Class name: Yuan
 * Base class: QGraphicsObject
 * Desciption: This is the declaration of class Yuan. Yuan is an
 *       "in" node.
******************************************************************/
class Yuan:public QGraphicsObject
{
public:
    enum {Type = YuanType};
    Yuan();
    ~Yuan();

    int type() const { return Type; }

    void setOutlineColor(const QColor &color);
    QColor outlineColor() const;
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const;

    void addLink(Link *link);
    void removeLink(Link *link);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF outlineRect() const;

    QGraphicsItem* myNode() const;
    QSet<Link *> myLinks;

    //自定义属性
    int inout() const {return m_mode;}
    void setInout(int mode);

    //QGraphicsItem *node;
    WidgetWrap *master;
    QString name;
//private:

    int roundness(double size) const;

    QColor myBackgroundColor;
    QColor myOutlineColor;
    int m_mode;
};

/*******************************************************************
 * Class name: triYuan
 * Base class: Yuan
 * Description: This is the declaration of class triYuan. triYuan is
 *       an "out" node.
******************************************************************/
class triYuan:public Yuan
{
public:
    triYuan();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPolygonF outlineRect() const;
};

/*******************************************************************
 * Class name: specialYuan
 * Base class: Yuan
 * Description: this class is set for characteristic points
******************************************************************/
class specialYuan:public Yuan
{
public:
    specialYuan();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF outlineRect() const;
};
#endif // YUAN_H
