#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "yuan.h"
#include "newnode.h"
#include "widgetmap.h"

//class DiagramWindow;
/********************************************************************
* Class name: newscene
* Description: provide a new scene to do some mouse-events
********************************************************************/
class newscene:public QGraphicsScene
{
    Q_OBJECT
public:
    newscene(WidgetMap* m);
    ~newscene();
    specialYuan* new_yuan;

    void bringToFront();

    bool CreateWidgets();
    //从用户鼠标事件或xml文件创建控件
    bool CreateTakeOff(QPointF point, int id, bool user);
    bool CreateLand(QPointF point, int id, bool user);
    bool CreateGo(QPointF point, int id, bool user);
    bool CreateTurn(QPointF point, int id, bool user);
    bool CreateHover(QPointF point, int id, bool user);
    bool CreateDelay(QPointF point, int id, bool user);
    bool CreateVarType(QPointF point, int id, bool user);
    bool CreateVarDef(QPointF point, int id, bool user, VardefNode *vdncopy);
    bool CreateCompute(QPointF point, int id, bool user);
    bool CreateIO(QPointF point, int id, bool user);
    bool CreateLogic(QPointF point, int id, bool user);


    int need_to_set;
    int selected_Index;

    int VarTypeNodeNum;  //计数varNode,命名每个varNode,下同
    int VarDefNodeNum;

    int LogicNodeNum;

    int takeoffNodeNum;
    int landonNodeNum;
    int GoNodeNum;  //6个方向的直线运动控件都包含在内
    int TurnNodeNum;    //左右两个方向的转向控件
    int HoverNodeNum;
    int DelayNodeNum;

    int CompputeNodeNum;

    int IONodeNum;

    int linkNodeNum;

    WidgetMap* wm;
signals:
    void itemInserted(int index);
    void sig_bringtofront();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* new_event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // SCENE_H
