/********************************************************
 * File: WidgetMap.cpp
 * Author: Gao Jiefeng
 * Description:This file realizes widget pointer management like
 *    add, delete, and get. It rely on map_instrument.cpp.
*********************************************************/

#include "widgetmap.h"
#include "takeoffnode.h"
#include "widgetwrap.h"

#include <QDebug>

/*****************************************************
 * Function name: WidgetMap
 * Description: This is the constructor.
 * Calle: none
 * Input: QObject *parent - QT set it, don't need to care about.
 * Output: none
 *****************************************************/
WidgetMap::WidgetMap(QObject *parent) : QObject(parent)
{
    amount = 0;
}

/*****************************************************
 * Function name: add
 * Description: To add a widget pointer into map.
 * Calle: none
 * Input: widget* w - the widget to be added.
 * Output: none
 *****************************************************/
void WidgetMap::add(WidgetWrap& tmp)
{
    amount++;
    QString index = "none";
    if(tmp.identifier == "TakeOff" ){
        QString id = QString::number((tmp.mTakeoffNode)->controlsId,10);
        index = (tmp.mTakeoffNode)->identifier + id;
    }
    qDebug()<<index;
    map_instrument::map_insert(Store,index,tmp);
}

/*****************************************************
 * Function name: del
 * Description: To delete a widget pointer into map.
 * Calle: none
 * Input: widget* w - the widget to be deleted.
 * Output: none
 *****************************************************/
void WidgetMap::del(widget* w)
{
    amount--;
    QString index;
    if(w->identifier == "TakeOff"){
        index = (w->mTakeoffNode)->identifier + (w->mTakeoffNode)->controlsId;
    }
    map_instrument::del_from_map(Store, index);
}

/*****************************************************
 * Function name: get_map
 * Description: To get the map.
 * Calle: none
 * Input: noe
 * Output: Store - The map containing widget pointers.
 *****************************************************/
QMap<QString, widget>& WidgetMap::get_map()
{
    return Store;
}

/*****************************************************
 * Function name: clear
 * Description: To clear the map.
 * Calle: none
 * Input: noe
 * Output: bool - whether the map is empty.
 *****************************************************/
bool WidgetMap::clear()
{
    typename QMap<QString, widget>::iterator iter;

    for(iter=Store.begin(); iter!=Store.end();){   //遍历控件指针
        iter = Store.erase(iter);
    }
    return Store.isEmpty();
}

