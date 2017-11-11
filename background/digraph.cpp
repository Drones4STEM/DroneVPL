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
digraph::digraph(QMap<QString, widget*> *m,QMap<QString,LOGIC_Help*>* lhm)
{
    Map = m;
    LHM = lhm;
    typename QMap<QString, widget*>::iterator it;
    for(it=Map->begin();it!=Map->end();it++){
        if(it.value()->identifier != "Link")
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
std::stack<widget*>* digraph::get_topology(Logic *l)
{
    //确定图的等级并获得这个图的访问方式
    //获得这个等级的图的起始节点
    //在这个等级的图做深度遍历
    WidgetWrap *tmp;
    int rank = (l==0)?1:l->rank+1;  //l的子图的rank低一级(+1)
    std::stack<widget*> stk; //保存没有出度的起始节点
    std::stack<widget*>* stack = new std::stack<widget*>();   //得到该图的拓扑结构的保存栈
    if(l==0)
        stk = get_nodes_without_IN(0);  //其实合法的起始节点只有takeoff一个，但原来写的函数功能更强，所以也就不删减了
    else{
        for(int i=0;i<l->yuan4->myLinks.toList().size();i++){
            tmp = l->yuan4->myLinks.toList()[i]->toYuan()->master;
        }
        stk.push(tmp);
    }


    while(!stk.empty()){
        tmp = stk.top();
        stk.pop();
        DFS(tmp,rank,stack);
    }

    //py代码逻辑上的需要，在rank1，必须把Var控件放在拓扑结构的最开头。顺便规定只能在rank1放var
    typename QMap<QString, widget*>::iterator iter2;
    if(l==0){
        for(iter2=Map->begin();iter2!=Map->end();iter2++){
            if(iter2.value()->identifier=="Var") stack->push(iter2.value());
        }
    }
    return stack;

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
std::stack<widget*> digraph::get_nodes_without_IN(Logic* l)
{
    std::stack<widget*> stk;  //保存没有出度的节点，返回这个栈对象
    typename QMap<QString,LOGIC_Help*>::iterator iter;
    typename QMap<QString, widget*>::iterator it;
    QMap<QString, widget*>* map;
//    if(l==0){
        map = Map;
//    }else{
//        for(iter=LHM->begin();iter!=LHM->end();iter++){
//            if(iter.value()->LOG==l)    map = &(iter.value()->WidgetsInLOGIC);
//        }
//    }

    for(it=map->begin(); it!=map->end(); ++it){
        if(it.value()->identifier!="Link")
            if(it.value()->rank()==((l==0)?1:l->rank+1))
                if(it.value()->check_yuan_in()==false){     //没有入度
                    //如果是Logic条件判断式，则无视。之后在转py读取的时候访问
/*                    if(l!=0 && (l->yuan2->myLinks.toList()[0]->fromYuan()->master == it.value())){
                        continue;
                    }else*/ if(it.value()->identifier=="VarType" || it.value()->identifier=="VarDef"){
                        continue;
                    }else
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
void digraph::DFS(widget* w, int rank,std::stack<widget*>* stack)
{
    if(w->rank() != rank) return;   //非该级的节点视作无连接
    else if(visited.value(w->name)==0){
        visited[w->name]=1;
        if(w->get_yuan_out()!= NULL){
            QSetIterator<Link*> it = (w->get_yuan_out()->myLinks);   //Qset的迭代器,传入迭代对象
            for(;it.hasNext();){      //遍历每个从当前控件节点指向的节点
                DFS(it.peekNext()->toYuan()->master,rank,stack);
                it.next();
            }
        }
        stack->push(w);  //加入结果集
    }
}











