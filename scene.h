#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "yuan.h"
#include "newnode.h"

/********************************************************************
* Class name: newview
* Description: provide a new view to do some mouse-events
********************************************************************/
class newscene:public QGraphicsScene
{
    Q_OBJECT
public:
    newscene();
    ~newscene();
    specialYuan* new_yuan;

    void setnewCreationName();
    Node *selectedNode() const;
    NewNode *selectedNewNode() const;
    void addTranslationNode();
    void bringToFront();

public slots:
    void setDirty(bool on=true);
private:
    void setZValue(int z);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* new_event);

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // VIEW_H
