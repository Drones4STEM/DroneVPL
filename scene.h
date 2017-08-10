#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "yuan.h"
#include "newnode.h"

//class DiagramWindow;
/********************************************************************
* Class name: newscene
* Description: provide a new scene to do some mouse-events
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

    int need_to_set;
    int selected_Index;

signals:
    void itemInserted(int index);
private:
    void setZValue(int z);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* new_event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // SCENE_H
