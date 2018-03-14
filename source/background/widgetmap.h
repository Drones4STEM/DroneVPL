#ifndef WIDGETMAP_H
#define WIDGETMAP_H

#include <QObject>
#include <QMap>
#include <QGraphicsObject>

#include "map_instrument.h"
#include "widgetwrap.h"

typedef WidgetWrap widget;

using namespace std;

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
    explicit WidgetMap(QObject *parent);
    WidgetMap(){amount = 0;}

    void add(WidgetWrap* tmp);    //用于在每次用户创建控件时添加该控件节点
    void del(widget w);    //删除控件
    QMap<QString, widget*>& get_map();
    bool set_map(QMap<QString, widget*> m){Store = m;return !Store.isEmpty();}

    bool clear();

    QMap<QString, widget*> Store;
private:

    int amount;

signals:

public slots:
    void link_deleted(QSet<Link *> links);
};

#endif // WIDGETMAP_H
