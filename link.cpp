#include <QtGui>

#include "link.h"
#include "node.h"
#include "yuan.h"

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;


Link::Link(Yuan *fromYuan, Yuan *toYuan)
{
    if(fromYuan->inout()==1)
    {
        myFromYuan=toYuan;
        myToYuan=fromYuan;
    }
    else
    {
        myFromYuan = fromYuan;
        myToYuan = toYuan;
    }

    arrowSize=10;

    myFromYuan->addLink(this);
    myToYuan->addLink(this);

    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(-1);

    setColor(Qt::darkRed);
    trackYuans();

    identifier="Link";
    controlsId=0;
}

Link::~Link()
{
    myFromYuan->removeLink(this);
    myToYuan->removeLink(this);
}

Yuan *Link::fromYuan() const
{
    return myFromYuan;
}

Yuan *Link::toYuan() const
{
    return myToYuan;
}

void Link::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor Link::color() const
{
    return pen().color();
}

void Link::trackYuans()
{
    setLine(QLineF(myFromYuan->pos(),myToYuan->pos()));
}

void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QLineF line(myFromYuan->pos(), myToYuan->pos());

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    //QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
    //                                              cos(angle + Pi / 3) * arrowSize);
    //QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
    //                                              cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = myToYuan->pos() + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = myToYuan->pos() + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    //painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
//! [6]
