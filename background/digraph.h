#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <QObject>
#include <QMap>
#include <QSetIterator>
#include <QGraphicsItem>
#include <stack>
typedef QGraphicsItem widget;

/***************************************************
 * Class name: digraph
 * Base class: QObject
 * Description: This class is designed for
 *    organizing widgets as digraph into topological
 *    sequence.
****************************************************/
//template<class widget>
class digraph : public QObject
{
    Q_OBJECT
public:
    explicit digraph(QObject *parent = 0, QMap<QString, widget*>* m = NULL);
    std::stack<widget *> get_topology();     //返回拓扑顺序的节点的容器


private:
    QMap<QString, widget*>* Map;     //指向保存控件的map
    QMap<int*,int> visited;          //保存每个控件是否被访问过。0没有，~0有,每个控件的名字是用数组存的
    std::stack<widget*> Stack;       //保存有向图的拓扑结构

    std::stack<widget*> get_nodes_without_OUT();   //返回没有指出箭头的节点的容器
    void DFS(widget* w);   //深度优先实现的拓扑排序

signals:

public slots:
};

#endif // DIGRAPH_H
