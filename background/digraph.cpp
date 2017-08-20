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
#include "link.h"
#include "yuan.h"

/*****************************************
 * Function name: diagraph
 * Description: Constructor
 * Calle: none
 * Input: QMap<QString, widget *> - widgets exits now.
 * Output: none
 *****************************************/
digraph::digraph(QMap<QString, widget> *m)
{
    Map = m;
    typename QMap<QString, widget>::iterator it;
    for(it=Map->begin();it!=Map->end();it++){
        if(it->identifier != "Link")
            visited.insert(it.key(),0);
    }
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
std::stack<widget> digraph::get_topology()
{/*
    typename QMap<QString, widget>::iterator it;
    for(it=Map->begin();it!=Map->end();it++){
        tmp = &(it.value());
        if(tmp->identifier != "Link")
            DFS(tmp);
    }*/
    WidgetWrap *tmp;
    std::stack<widget> stk = get_nodes_without_IN();
    while(!stk.empty()){
        tmp = &(stk.top());
        stk.pop();
        if(tmp->identifier != "Link")
            DFS(tmp);
    }

    return Stack;
}

/*****************************************************
 * Function name: get_nodes_without_IN
 * Description: This is the function that gets
 *    the widgets of no out-link one by one.
 * Calle: none
 * Input: none
 * Output: stack<widget*> sta - The stack containing
 *            widgets with no out-link.
 *****************************************************/
std::stack<widget> digraph::get_nodes_without_IN()
{
    //遍历map，找节点
    std::stack<widget> stk;  //保存没有出度的节点，返回这个栈对象
    typename QMap<QString, widget>::iterator it;
    for(it=Map->begin(); it!=Map->end(); ++it){
        if(it->check_yuan_in()==false){     //没有link
            stk.push(it.value());
        }
    }
    return stk;
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
    if(visited.value(w->name)==0){
        visited[w->name]=1;
        if(w->identifier!="Logic"){
            if(w->get_yuan_out() != NULL){
                QSetIterator<Link*> it = (w->get_yuan_out()->myLinks);   //Qset的迭代器,传入迭代对象
                for(;it.hasNext();){      //遍历每个从当前控件节点指向的节点
                    //widget* ww = it.next()->mytoyuan->node;
                    DFS(it.next()->toYuan()->master);
                }
            }
        }else{

        }

        Stack.push(*w);  //加入结果集
    }
}











