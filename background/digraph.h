#ifndef DIGRAPH_H
#define DIGRAPH_H

/*
 *Overrall Designation Of Widget Digraph
 *    Widgets are divided into some ranks.The outside widgets are at first rank,
 *  then the widgets in a Logic at first rank are at second rank, and so on.
 *    When traverse the digraph,from rank low to rank high.
 *    Logic can be created anywhere. But if move it, all widgets in it should move
 *  with it. Thus links into Logic will not be excluded.
*/

#include <QObject>
#include <QMap>
#include <QSetIterator>
#include <QGraphicsItem>
#include <stack>
#include "widgetwrap.h"
#include "logic_help.h"

typedef WidgetWrap widget;
typedef Rec Logic;

/***************************************************
 * Class name: digraph
 * Base class: QObject
 * Description: This class is designed for
 *    organizing widgets as digraph into topological
 *    sequence.
****************************************************/

class digraph
{
public:
    digraph(QMap<QString, widget*>* m = NULL,QMap<QString,LOGIC_Help*>* lhm = NULL);
    std::stack<widget *> *get_topology(Logic *l=0);     //返回拓扑顺序的节点的容器


private:
    QMap<QString,LOGIC_Help*>* LHM;
    QMap<QString, widget*>* Map;     //指向保存控件的map
    QMap<QString,int> visited;          //保存每个控件是否被访问过。0没有，~0有,每个控件的名字是用数组存的
    std::stack<widget*> Stack;       //保存有向图的拓扑结构

    std::stack<widget*> get_nodes_without_IN(Logic *l=0);   //返回没有指出箭头的节点的容器
    void DFS(widget* w, int rank, std::stack<widget*>* stack);   //深度优先实现的拓扑排序

signals:

public slots:
};

#endif // DIGRAPH_H
