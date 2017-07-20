#ifndef LOGIC_INSTRUMENT_H
#define LOGIC_INSTRUMENT_H


#include <QObject>
#include <QMap>
#include <QString>
#include "map_instrument.h"
#include "E:dachuang/QT_project/workspace/diagram1/rec.h"
typedef Rec LOGIC;

/***************************************************
 * Class name: LOGIC_instrument
 * Base class: QObject
 * Description: This class is designed for help deal with
 *    LOGIC. It can do:
 *    -Judge what the LOGIC contains and save them.
 *     When a LOGIC is created, a corresponding LOGIC_instrument
 *     is created.
 *    -When a widget is created or moved, all LOGIC_instrument
 *     receive this signal and do the first funciton.
****************************************************/

//template <class widget>
class LOGIC_instrument : public QObject
{
    Q_OBJECT
public:

    explicit LOGIC_instrument(LOGIC* L, QObject *parent = 0);

    //判断是否某个控件是否在某个LOGIC内部。LOGIC是LOGIC类对象
    static bool in_LOGIC(widget* w, LOGIC* L);

private:

    QMap<QString, widget*> WidgetsInLOGIC;
    LOGIC* LOG;     //指向对应的LOGIC
    int LOGname[1]; //保存LOG的名字

signals:

public slots:
    bool widget_make_or_move(widget *w, LOGIC *L);
};

#endif // LOGIC_INSTRUMENT_H
