#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>
#include "itemtypes.h"
#include "rec.h"
#include "QObject"

class Node;
class Yuan;
class Rec;
class Link;

/*******************************************************************
 * Class name: Link
 * Base class: QGraphicsLineItem
 * Description: This is the declaration of class Link. Link creates
 *        a connection between two nodes.
******************************************************************/
class Link :public QGraphicsLineItem
{
public:
    enum {Type = LinkType};
    Link(Yuan *fromYuan=NULL, Yuan *toYuan=NULL);
    ~Link();

    int type() const { return Type; }

    Yuan *fromYuan() const;
    Yuan *toYuan() const;

    void setColor(const QColor &color);
    QColor color() const;

    void trackYuans();

    QString from_master_name();
    QString to_master_name();

    //控件名称与在同类控件中的编号
    QString identifier;
    int controlsId;
    QString name;
//下面两个量应该不用了才对
    Rec* toLogic;
    Rec* fromLogic;

private:
    Yuan *myFromYuan;
    Yuan *myToYuan;
    qreal arrowSize;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//signals:

};

#endif
