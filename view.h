#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QPainter>
#include <QWheelEvent>
#include <cmath>


class View:public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent=0):QGraphicsView(parent)
    {
        setDragMode(RubberBandDrag);
        setRenderHints(QPainter::Antialiasing|
                       QPainter::TextAntialiasing);
    }
public slots:
    void zoomIn(){scaleBy(1.1);}
    void zoomOut(){scaleBy(1.0/1.1);}
protected:
    void wheelEvent(QWheelEvent *event)
    {scaleBy(std::pow(4.0/3.0,(event->delta()/240.0)));}

private:
    void scaleBy(double factor){scale(factor,factor);}
};

#endif // VIEW_H
