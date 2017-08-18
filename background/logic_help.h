#ifndef LOGIC_INSTRUMENT_H
#define LOGIC_INSTRUMENT_H


#include <QObject>
#include <QMap>
#include <QString>
#include "map_instrument.h"
#include "rec.h"
typedef Rec LOGIC;
typedef WidgetWrap widget;

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

class LOGIC_Help
{
public:

    LOGIC_Help(LOGIC* L);

    //判断是否某个控件是否在某个LOGIC内部。LOGIC是LOGIC类对象
    bool in_LOGIC(widget* w);

private:

    QMap<QString, widget*> WidgetsInLOGIC;
    LOGIC* LOG;     //指向对应的LOGIC
    //int LOGname[1]; //保存LOG的名字

signals:

public slots:
    bool widget_make_or_move(widget *w, LOGIC *L);
};

#endif // LOGIC_INSTRUMENT_H
