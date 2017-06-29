#ifndef YUAN_H
#define YUAN_H

#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
#include "itemtypes.h"

class Link;
class Node;

class Yuan:public QGraphicsObject
{
public:
    enum {Type = YuanType};
    Yuan(QGraphicsItem *parent=0);
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

    //自定义属性。。。。。
    int inout() const {return m_mode;}
    void setInout(int mode);

    QGraphicsItem *node;
private:

    int roundness(double size) const;

    QColor myBackgroundColor;
    QColor myOutlineColor;
    int m_mode;
};

class triYuan:public Yuan
{
public:
    triYuan(QGraphicsItem *parent=0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPolygonF outlineRect() const;
};

#endif // YUAN_H
