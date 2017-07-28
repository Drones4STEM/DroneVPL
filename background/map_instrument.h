#ifndef MAP_INSTRUMENT_H
#define MAP_INSTRUMENT_H

#include <QObject>
#include <QString>
#include <QMap>

/***************************************************
 * Class name: map_instrument
 * Base class: QObject
 * Description: This class is designed for help deal with
 *    map. It can put, delete, traverse and so on.
****************************************************/

//template <typename widget>
class map_instrument : public QObject
{
    Q_OBJECT

public:
    explicit map_instrument(QObject *parent = 0);

    //把一个元素放入map中，如果已经存在，会返回false，否则保存该元素并返回true
    static bool put_in_map(QMap<QString, widget*>* m, QString s, widget* w);

    //把一个元素从map中删除，如果本不存在，会返回false，否则删除该元素并返回true
    static bool del_from_map(QMap<QString, widget*>* m, QString s);

    static widget* find(QMap<QString, widget*>* m, QString name);

private:
    //包装map自带的插入函数以提高性能和兼容各种index及value的属性
    static void map_insert(QMap<QString, widget*>* m, QString index, widget* value);

signals:

public slots:
};

#endif // MAP_INSTRUMENT_H
