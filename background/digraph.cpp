/********************************************************
 * File: digraph.cpp
 * Author: Gao Jiefeng
 * Description:This file realizes some functions about digraph. It's main function is to
 *    organize digraph into topological sequence. Some turotial function are defined,like
 *    depth-first traversal and traversal of QMap.
 *    In my view, all widgets derived from the same parent class, so I can visit any
 *    widget by a pointer of this parent class. This digraph class is based on that idea.
 *    There may be some conflicts with reception code.
*********************************************************/

#include "digraph.h"
#include "E:dachuang/QT_project/workspace/diagram1/Link.h"

/*****************************************
 * Function name: diagraph
 * Description: Constructor
 * Calle: none
 * Input: QMap<QString, widget *> - widgets exits now.
 * Output: none
 *****************************************/
digraph::digraph(QObject *parent, QMap<QString, widget *> *m) : QObject(parent)
{
    Map = m;
}

/*****************************************************
 * Function name: get_topology
 * Description: This is the function that gets
 *    the topological sequence of widget digraph.
 * Calle: DFS()
 * Input: none
 * Output: stack<widget*> Stack - The stack containing
 *            widgets in topological sequence.
 *****************************************************/
std::stack<widget*> digraph::get_topology()
{
    typename QMap<QString, widget*>::iterator it;
    for(it=Map->begin();it!=Map->end();it++){
        DFS(it->value);
    }
    return Stack;
}

/*****************************************************
 * Function name: get_nodes_without_OUT
 * Description: This is the function that gets
 *    the widgets of no out-link one by one.
 * Calle: none
 * Input: none
 * Output: stack<widget*> sta - The stack containing
 *            widgets with no out-link.
 *****************************************************/
std::stack<widget*> digraph::get_nodes_without_OUT()
{
    //遍历map，找节点
    std::stack<widget*> sta;  //保存没有出度的节点，返回这个栈对象
    typename QMap<QString, widget*>::iterator it;
    for(it=Map->begin(); it!=Map->end(); ++it){
        if(it->value->yuan->myLinks->isEmpty()){     //没有link
            sta.push(it->value);
        }
    }
    return sta;
}

/*****************************************************
 * Function name: DFS
 * Description: This is the function that traverse
 *    the widgets digraph in depth-first mode.
 * Calle: DFS()
 * Input: widget* w - the widget to start the traverse
 * Output: none
 *****************************************************/
void digraph::DFS(widget* w)
{
    if(visited(w->type())==0){
        visited(w->type())=1;
        QSetIterator<Link*> it(w->yuan->myLinks);   //Qset的迭代器,传入迭代对象
        for(;it.hasNext();){      //遍历每个从当前控件节点指向的节点
            //widget* ww = it.next()->mytoyuan->node;
            DFS(it.next()->toYuan()->node);
        }
        Stack.push(w);  //加入结果集
    }
}











