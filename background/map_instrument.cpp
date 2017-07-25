/********************************************************
 * File: map_instrument.cpp
 * Author: Gao Jiefeng
 * Description:This file realizes map operation like insert,
 *    put, delete, find.
*********************************************************/

#include "map_instrument.h"
#include <string>

/*****************************************************
 * Function name: map_instrument
 * Description: This is the constructor.
 * Calle: none
 * Input: QObject *parent - QT set it, don't need to care about.
 * Output: none
 *****************************************************/
map_instrument::map_instrument(QObject *parent) : QObject(parent)
{
    ;
}

/*****************************************************
 * Function name: map_insert
 * Description: This is the funtion that insert widget
 *    into a certain map.
 * Calle: none
 * Input: QMap<QString, widget*>* m - the map to store widget
 *        QString index - the widget's index
 *        widget* value - the widget to be store
 * Output: none
 *****************************************************/
void map_instrument::map_insert(QMap<QString, widget*>* m, QString index, widget* value)
{
    m->insert(index,value);
}

/*****************************************************
 * Function name: put_in_map
 * Description: This is the funtion that insert widget
 *    into optional map when it's not inserted yet.
 * Calle: map_insert()
 * Input: QMap<QString, widget*>* m - the map to store widget
 *        QString s - the widget's index
 *        widget* w - the widget to be store
 * Output: true - if successfully inserted
 *         false - if not inserted successfully(already inserted one)
 *****************************************************/
bool map_instrument::put_in_map(QMap<QString, widget*>* m, QString s, widget* w)
{
    typename QMap<QString, widget*>::iterator iter;
    iter = m.find(s);

    if(iter == m.end()){       //如果元素未包含
        map_insert(&m, s, w);
        return true;
    }else
        return false;
}


/*****************************************************
 * Function name: del_from_map
 * Description: This is the funtion that delete widget
 *    in a map.
 * Calle: none
 * Input: QMap<QString, widget*>* m - the map to store widget
 *        QString s - the widget's index
 * Output: true - if successfully deleted
 *         false - if not deleted successfully(don't exist)
 *****************************************************/
bool map_instrument::del_from_map(QMap<QString, widget*>* m, QString s)
{
    typename QMap<QString, widget*>::iterator iter;
    iter = m.find(s);

    if(iter == m.end()){       //如果元素未包含
        return false;
    }else{
        m.erase(iter);
        return true;
    }

}

/*****************************************************
 * Function name: find
 * Description: This is the funtion that juedge if a
 *    certain widget is in a certain map. This function
 *    is not completed yet.
 *    in a map.
 * Calle: none
 * Input: QMap<QString, widget*>* m - the map to store widget
 *        QString name - the widget's name
 * Output: iter->value - the widget's name
 *         null - no such widget
 *****************************************************/
widget* map_instrument::find(QMap<QString, widget*>* m, QString name = "null")
{
    typename QMap<QString, widget*>::iterator iter;
    for(iter=m->begin();iter!=m->end();iter++){
        if( iter->key.compare(0,5,name)==0 ){  //字符串以"LOGIC"开头
            return iter->value;
        }else
            return NULL;
    }
}
