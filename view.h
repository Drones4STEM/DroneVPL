#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "yuan.h"

/********************************************************************
* Class name: newview
* Description: provide a new view to do some mouse-events
********************************************************************/
class newview:public QGraphicsView
{
    Q_OBJECT
public:
    newview();
    ~newview();
    Yuan* new_yuan;
    //Link* new_link;
protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
};

#endif // VIEW_H
