#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "yuan.h"


class newview:public QGraphicsView
{
    Q_OBJECT
public:
    newview();
    ~newview();
    Yuan* new_yuan;
    //Link* new_link;
protected:
    void mouseMoveEvent(QMouseEvent* event_one);
    void mouseReleaseEvent(QMouseEvent* event_two);
};

#endif // VIEW_H
