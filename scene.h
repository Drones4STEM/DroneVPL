#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "yuan.h"
#include "newnode.h"
#include "widgetmap.h"
#include "logic_help.h"

//class DiagramWindow;
/********************************************************************
* Class name: newscene
* Description: provide a new scene to do some mouse-events
********************************************************************/
class newscene:public QGraphicsScene
{
    Q_OBJECT
public:
    newscene(WidgetMap* m, QMap<QString,LOGIC_Help*>*L);
    ~newscene();
    specialYuan* new_yuan;

    void bringToFront();


    bool CreateWidgets();
    //从用户鼠标事件创建控件
    bool CreateTakeOff(QPointF point, int id);
    bool CreateLand(QPointF point, int id);
    bool CreateGo(QPointF point, int id);
    bool CreateTurn(QPointF point, int id);
    bool CreateHover(QPointF point, int id);
    bool CreateDelay(QPointF point, int id);
    bool CreateVarType(QPointF point, int id);
    bool CreateVarDef(QPointF point, int id);
    bool CreateCompute(QPointF point, int id);
    bool CreateIO(QPointF point, int id);
    bool CreateLogic(QPointF point, int id);
    bool CreateLink(QGraphicsSceneMouseEvent* event);

    //从xml文件创建控件
    bool CreateTakeOff(TakeoffNode* ton);
    bool CreateLand(LandonNode* ln);
    bool CreateGo(TranslationNode* gn);
    bool CreateTurn(TurnNode* tn);
    bool CreateHover(HoverNode* hn);
    bool CreateDelay(DelayNode* dn);
    bool CreateVarType(VarNode* vtn);
    bool CreateVarDef(VardefNode* vdn);
    bool CreateCompute(ComputeNode* cn);
    bool CreateIO(IoNode* ion);
    bool CreateLogic(Rec* ln);
    bool CreateLink(Link* link);

    bool CheckInLogic(WidgetWrap *tmp);


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

    int ComputeNodeNum;

    int IONodeNum;

    int linkNodeNum;

    WidgetMap* wm;
    QMap<QString,LOGIC_Help*>* LHM;

signals:
    void itemInserted(int index);
    void sig_bringtofront();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* new_event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
};

#endif // SCENE_H
