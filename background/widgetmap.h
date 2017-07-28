#ifndef WIDGETMAP_H
#define WIDGETMAP_H

#include <QObject>
#include <QMap>
#include "map_instrument.h"

/***************************************************
 * Class name: WidgetMap
 * Base class: QObject
 * Description: This class is designed for save parent
 *    pointer of widgets, so I can visit all the existing
 *    widgets. To see why use parent pointer is used,
 *    refer to description of digraph.cpp.
****************************************************/

//template <class widget>
class WidgetMap : public QObject
{
    Q_OBJECT
public:
    explicit WidgetMap(QObject *parent = 0);

    //用于在每次用户创建控件时添加该控件节点，
    void add(widget* w);

    //删除控件
    void del(widget* w);

    QMap<QString, widget*> get_map();

private:
    QMap<QString, widget*> Store;
    int amount;

signals:

public slots:
};

#endif // WIDGETMAP_H
