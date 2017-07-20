/********************************************************
 * File: WidgetMap.cpp
 * Author: Gao Jiefeng
 * Description:This file realizes widget pointer management like
 *    add, delete, and get. It rely on map_instrument.cpp.
*********************************************************/

#include "widgetmap.h"

/*****************************************************
 * Function name: WidgetMap
 * Description: This is the constructor.
 * Calle: none
 * Input: QObject *parent - QT set it, don't need to care about.
 * Output: none
 *****************************************************/
WidgetMap<widget>::WidgetMap(QObject *parent) : QObject(parent)
{
    Store = new QMap<QString, widget*>();
    amount = 0;
}

/*****************************************************
 * Function name: add
 * Description: To add a widget pointer into map.
 * Calle: none
 * Input: widget* w - the widget to be added.
 * Output: none
 *****************************************************/
void WidgetMap::add(widget* w)
{
    amount++;
    map_instrument::map_insert(&Store, w->name, w);
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
    map_instrument<widget>::del_from_map(&Store, w->name);
}

/*****************************************************
 * Function name: get_map
 * Description: To get the map.
 * Calle: none
 * Input: noe
 * Output: Store - The map containing widget pointers.
 *****************************************************/
QMap<QString, widget*> WidgetMap::get_map()
{
    return Store;
}



